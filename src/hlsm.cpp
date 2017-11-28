//
// Created by Joe Loubert on 11/22/2017.
//

#include "hlsm.h"

int hlsm::inop(int id) {                // This turns component id into inop = Wait state
    auto it = components.find(id);
    if (it == components.end()) {
        cout << "inop cannot be defined using id "<<id<<endl;
        return -1;
    }
    add_state(0);                       // Wait state = 0
    states[0]->start_cyc = 0;           // Wait starts at cycle 0
    states[0]->last_cyc = 0;            // Wait has last cycle 0
    states[0]->num_cyc = 1;             // Wait is one cycle thick
    add_branch("Start", 0, 1, 0);       // Wait branches to state 1 on start
    components[id]->state = 0;          // inop lives in state 0
    components[id]->start_cyc = 0;      // inop starts on cycle 0
    components[id]->num_cyc = 1;        // inop lasts 1 cycle
    components[id]->end_cyc = 1;        // inop ends on cycle 1
    components[id]->out_str = "";       // inop cannot be seen
    components[id]->is_scheduled = true;// inop is scheduled at time 0
    return 0;
}

int hlsm::add_state(int st) {   // Create a new state, numbered st
    auto it = states.find(st);
    if (it == states.end()) {
        statedef *s;
        s = new statedef(st);
        states[st] = s;
        return 0;
    }
    else {
        cout << "State " << st << " already exists!\n";
        return -1;
    }
}

int hlsm::add_component(int id, int state, const string &out_str) {
    cmap::iterator itr = components.find(id);
    if (itr == components.end()) {
        component *v;
        v = new component(id, state, out_str);
        components[id] = v;
        num_components++;
        return 0;
    }
    cout << "\nComponent already exists!";
    return -1;
}

int hlsm::add_wire(const string &name, bool sgn, int dw) {
    wmap::iterator itr = wires.find(name);
    if (itr == wires.end()) {
        wire *v;
        v = new wire(name, sgn, dw);
        wires[name] = v;
        return 0;
    }
    else {
        cout << "Wire already declared\n";
        return -1;
    }
}

int hlsm::wire_to_component(const string &w, int c) {
    auto itw = wires.find(w);
    if (itw == wires.end()) {
        cout << "\nWire " << w << " is not declared.";
        return -1;
    }

    auto itc = components.find(c);
    if (itc == components.end()) {
        cout << "\nComponent " << c << " does not exist!";
        return -1;
    }

    int parent = itw->second->from;
    auto itp = components.find(parent);
    if (itp == components.end()) {
        cout << "\nComponent " << parent << " does not exist!";
        return -1;
    }

    if (itp->second->state == itc->second->state){
        itc->second->ins.push_back(itp->second->id);
        itp->second->outs.push_back(itc->second->id);
    }
    itw->second->to.push_back(c);   // I think this is unused
    return 0;
}

int hlsm::wire_from_component(const string &w, int c){
    auto itw = wires.find(w);
    if (itw == wires.end()) {
        cout << "\nWire does not exist!";
        return -1;
    }

    auto itc = components.find(c);
    if (itc == components.end()) {
        cout << "\nComponent " << c << " does not exist!";
        return -1;
    }

    itw->second->from = c;
    return 0;
}

int hlsm::add_branch(const string &cond, int from_state, int if_state, int else_state) {
    auto it = states.find(from_state);
    if (it == states.end()) {
        cout << "State not found: "<<from_state<<endl;
        return -1;
    }
    it->second->sbranch.cond = cond;
    it->second->sbranch.if_state = if_state;
    it->second->sbranch.else_state = else_state;
    return 0;
}

int hlsm::set_child_state(int st, int cst) {
    auto it = states.find(st);
    if (it == states.end()) {
        cout << "Could not set child states for: "<<st<<endl;
        return -1;
    }

    if (states[st]->sbranch.if_state == -1){
        states[st]->sbranch.if_state = cst;
    }
    else if (states[st]->sbranch.if_state != cst) {
        set_child_state(states[st]->sbranch.if_state, cst);
    }

    if (states[st]->sbranch.else_state == -1) {
        states[st]->sbranch.else_state = cst;
    }
    else if (states[st]->sbranch.else_state != cst) {
        set_child_state(states[st]->sbranch.else_state, cst);
    }

    return 0;
}

int hlsm::clear(int st) {
    for (auto it : components)
        if (it.second->state == st) {
            it.second->is_scheduled = false;    // Mark every component in state 'st' as unscheduled
        }
    for (auto it : states[st]->cycmap)
        states[st]->cycmap[it.first].clear();   // Clear the list of scheduled operations

    return 0;
}

int hlsm::ASAP_me(int i) {
    if(components[i]->is_scheduled)         // If component 'i' has already been scheduled,
        return components[i]->end_cyc;      // return end cycle.

    int max_cyc = 0;                        // Latest scheduled predecessor end cycle
    int temp;
    for (auto it : components[i]->ins) {    // Iterate over predecessors of 'i'
        temp = ASAP_me(it);                 // Returns the end cycle of predecessor 'it'
        if (temp > max_cyc)
            max_cyc = temp;
    }

    components[i]->start_cyc = max_cyc;     // Schedule 'i' as early as possible
    components[i]->end_cyc = max_cyc + components[i]->num_cyc;  // Component 'i' lasts 'num_cyc' cycles
    components[i]->is_scheduled = true;     // Mark as scheduled
    if (components[i]->end_cyc > states[components[i]->state]->num_cyc)
        states[components[i]->state]->num_cyc = components[i]->end_cyc;
    return components[i]->end_cyc;
}

int hlsm::ASAP(int st){
    int max_cyc = 0;                        // Keep track of the number of cycles in state 'st'
    clear(st);                              // Unschedule every component in state 'st'
    for (auto it : components) {            // Iterate over every component 'it'
        if (it.second->state == st) {       // Check if state of 'it' is 'st'
            int temp = ASAP_me(it.first);   // ASAP schedule component 'it'
            if (temp > max_cyc)
                max_cyc = temp;
            int cyc = it.second->start_cyc;
            auto it2 = states[st]->cycmap.find(cyc);    // cycmap keeps track of all components in cycle 'cyc'
            if (it2 == states[st]->cycmap.end()){
                states[st]->cycmap[cyc].push_back(it.first);
            }
            else {
                it2->second.push_back(it.first);
            }
        }
    }
    states[st]->num_cyc = max_cyc;
    return 0;
}

int hlsm::ALAP_me(int i, int lat) {
    if(components[i]->is_scheduled)         // If component 'i' has already been scheduled,
        return components[i]->start_cyc;    // return start cycle.

    int min_cyc = lat;                      // Earliest scheduled successor start cycle
    int temp;
    for (auto it : components[i]->outs) {   // Iterate over successors of 'i'
        temp = ALAP_me(it, lat);            // Returns the start cycle of successor 'it'
        if (temp < min_cyc)
            min_cyc = temp;
    }

    components[i]->start_cyc = min_cyc - components[i]->num_cyc;    // Schedule 'i' as late as possible
    components[i]->end_cyc = min_cyc;                               // Operation ends just before min_cyc
    components[i]->is_scheduled = true;                             // Mark as scheduled

    return components[i]->start_cyc;
};

int hlsm::ALAP(int st, int lat) {
    clear(st);
    states[st]->num_cyc = lat;

    for (auto it : components) {
        if (it.second->state == st) {
            int temp = ALAP_me(it.first, lat);
            if (temp < 0)
                return -1;
            int cyc = it.second->start_cyc;
            auto it2 = states[st]->cycmap.find(cyc);    // cycmap keeps track of all components in cycle 'cyc'
            if (it2 == states[st]->cycmap.end()){
                states[st]->cycmap[cyc].push_back(it.first);
            }
            else {
                it2->second.push_back(it.first);
            }
        }
    }

    return 0;
}