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
    auto itr = components.find(id);
    if (itr == components.end()) {
        component *v;
        v = new component(id, state, out_str);
        components[id] = v;
        num_components++;
        return 0;
    }
    else {
        cout << "Component already exists: " << id << endl;
        return -1;
    }
}

int hlsm::add_wire(const string &name, bool sgn, int dw) {
    auto itr = wires.find(name);
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

int hlsm::unsched(int st) {
    states[st]->is_sched = false;
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
    components[i]->ASAP_lat = max_cyc;
    components[i]->end_cyc = max_cyc + components[i]->num_cyc;  // Component 'i' lasts 'num_cyc' cycles
    components[i]->is_scheduled = true;     // Mark as scheduled

    if (components[i]->end_cyc > states[components[i]->state]->num_cyc)
        states[components[i]->state]->num_cyc = components[i]->end_cyc;
    return components[i]->end_cyc;
}

int hlsm::ASAP(int st){
    int max_cyc = 0;                        // Keep track of the number of cycles in state 'st'
    unsched(st);                              // Unschedule every component in state 'st'
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
    states[st]->min_lat = max_cyc;
    states[st]->num_cyc = max_cyc;
    states[st]->is_sched = true;
    return 0;
}

int hlsm::min_succ_lat(int st) {            // Compute the minimum latency of all successor trees, including 'st'
    auto it = states.find(st);
    if (it == states.end()) {
        cout << "State not found: "<<st<<endl;
        return -1;
    }
    if (states[st]->msl_sched) {
        return states[st]->min_succ_lat + states[st]->min_lat;
    }

    int max = 0;
    if (states[st]->sbranch.if_state != -1) {
        int temp = min_succ_lat(states[st]->sbranch.if_state);
        if (temp > max){
            max = temp;
        }
    }

    if (states[st]->sbranch.else_state != -1) {
        int temp = min_succ_lat(states[st]->sbranch.else_state);
        if (temp > max){
            max = temp;
        }
    }

    states[st]->min_succ_lat = max;
    states[st]->msl_sched = true;
    return states[st]->min_lat + max;
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
    components[i]->ALAP_lat = components[i]->start_cyc;

    return components[i]->start_cyc;
};

int hlsm::ALAP(int st, int lat) {             // Absolutely run min_succ_lat before this
    unsched(st);                              // Unschedule the operations
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

    states[st]->is_sched = true;
    return 0;
}

int hlsm::unsched_all() {
    for (auto it : states) {
        unsched(it.first);
    }
    return 0;
}

int hlsm::ALAP_succ(int st, int lat) {  // Compute ALAP of st and all subsequent states
    auto it = states.find(st);
    if (it == states.end()) {
        cout << "State not found: " << st<<endl;
        return -1;
    }

    if (ALAP(st, lat - states[st]->min_succ_lat) < 0) {
        return -1;
    }

    int if_s = states[st]->sbranch.if_state;
    if ((if_s != -1) && (!states[if_s]->is_sched)) {
        if (ALAP_succ(states[st]->sbranch.if_state, states[st]->min_succ_lat) < 0) {
            return -1;
        }
    }

    int else_s = states[st]->sbranch.else_state;
    if ((else_s != -1) && (!states[else_s]->is_sched)) {
        if (ALAP_succ(states[st]->sbranch.else_state, states[st]->min_succ_lat) < 0) {
            return -1;
        }
    }

    return 0;
}

int hlsm::FDS_setup(int st) {   // Basically just update distribution graphs (DGs). Run after updating ASAP and ALAP.
    auto it = states.find(st);
    if (it == states.end()) {
        cout << "in FDS_setup: state "<<st<<" not found."<<endl;
        return -1;
    }

    for (int i = 0; i < states[st]->num_cyc; i++) {
        states[st]->DGADD[i] = 0.0;            // Clear the dang distribution graph
        states[st]->DGMUL[i] = 0.0;
        states[st]->DGDIV[i] = 0.0;
        states[st]->DGLOG[i] = 0.0;
    }

    for (auto it : components) {
        int j = it.first;
        if (components[j]->state == st) {
            for (int i = 0; i < states[st]->num_cyc; i++) {
                components[j]->FDS_prob[i] = 0;     // Clear each operation's probability list
            }
        }
    }

    for (auto it : components) {
        int j = it.first;                                       // j = id of component 'it'
        if (components[j]->state == st) {                       // Set DG probability of component j
            double prob = 1.0 / double(components[j]->ALAP_lat - components[j]->ASAP_lat + 1);  // to be 1/(ASAP - ALAP + 1).
            for (int i = components[j]->ASAP_lat; i <= components[j]->ALAP_lat; i++) {
                for (int k = 0; k < components[j]->num_cyc; k++) {  // Works for multi-cycle ops.
                    components[j]->FDS_prob[i+k] += prob;
                    switch (components[j]->op) {                    // Too hurried to figure out how to do this properly
                        case ADD :
                            states[st]->DGADD[i+k] += prob;
                            break;
                        case MUL :
                            states[st]->DGMUL[i+k] += prob;
                            break;
                        case DIV :
                            states[st]->DGDIV[i+k] += prob;
                            break;
                        case LOG :
                            states[st]->DGLOG[i+k] += prob;
                            break;
                        default : break;
                    }
                }
            }
        }
    }
    return 0;
}

int hlsm::FDS(int st) {
    bool all_ops_scheduled;
    bool first_sched;
    double temp;
    double min_force = 0;
    int min_comp = 0;
    int min_time = 0;
    while (true) {                          // Schedule FOREVER
        first_sched = true;
        all_ops_scheduled = true;
        FDS_setup(st);                      // Update distribution graphs
        for (auto it : components) {
            if ((it.second->FDS_scheduled) || (it.second->state != st)){
                continue;
            }
            else {
                all_ops_scheduled = false;
                int j = it.first;
                for (int i = components[j]->ASAP_lat; i <= components[j]->ALAP_lat; i++) {
                    temp = total_force(j, i);    // Total force of component j at time i;
                    if ((first_sched) || (temp < min_force)) {
                        min_force = temp;
                        min_comp = j;
                        min_time = i;
                        first_sched = false;
                    }
                }
            }
        }
        if (all_ops_scheduled) {
            break;
        }
        components[min_comp]->ASAP_lat = min_time;
        components[min_comp]->ALAP_lat = min_time;
        components[min_comp]->start_cyc = min_time;
        components[min_comp]->end_cyc = min_time + components[min_comp]->num_cyc;
        components[min_comp]->FDS_scheduled = true;
        int cyc = components[min_comp]->start_cyc;

        auto it = states[st]->cycmap.find(cyc);    // cycmap keeps track of all components in cycle 'cyc'
        if (it == states[st]->cycmap.end()){
            states[st]->cycmap[cyc].push_back(min_comp);
        }
        else {
            it->second.push_back(min_comp);
        }
        pred_ALAP(min_comp);
        succ_ASAP(min_comp);
    }
    return 0;
}

double hlsm::total_force(int comp, int time) {
    int st = components[comp]->state;
    double self = 0.0;
    double pred = 0.0;
    double succ = 0.0;
    double temp;
    for (int i=0; i<states[st]->num_cyc; i++) {
        if ((i >= time) && (i < time + components[comp]->num_cyc)) {
            temp = 1.0;     // Scheduled
        }
        else {
            temp = 0.0;     // Not scheduled
        }
        switch (components[comp]->op) {
            case ADD :
                self += (temp - components[comp]->FDS_prob[i]) * states[st]->DGADD[i];
                break;
            case MUL :
                self += (temp - components[comp]->FDS_prob[i]) * states[st]->DGMUL[i];
                break;
            case DIV :
                self += (temp - components[comp]->FDS_prob[i]) * states[st]->DGDIV[i];
                break;
            case LOG :
                self += (temp - components[comp]->FDS_prob[i]) * states[st]->DGLOG[i];
                break;
            default : break;
        }
    }
    for (auto it : components[comp]->ins) {
        pred += pred_force(it, time);
    }

    for (auto it : components[comp]->outs) {
        succ += succ_force(it, time + components[comp]->num_cyc);
    }
    return self + pred + succ;
}

double hlsm::pred_force(int comp, int finish) {
    double temp[MAX_LAT] = {0.0};
    double pred = 0.0;
    int st = components[comp]->state;
    int ALAP = finish - components[comp]->num_cyc;                      // Hypothetical new ALAP of predecessor
    double prob = 1.0/double(ALAP - components[comp]->ASAP_lat + 1);    // Hypothetical new probability of scheduling predecessor

    for (int i = components[comp]->ASAP_lat; i <= ALAP; i++) {
        for (int j = 0; j < components[comp]->num_cyc; j++) {
            temp[i+j] += prob;                                          // Hypothetical new FDS_prob
        }
    }

    for (int i = 0; i < states[st]->num_cyc; i++) {
        switch (components[comp]->op) {
            case ADD :
                pred += (temp[i] - components[comp]->FDS_prob[i]) * states[st]->DGADD[i];
                break;
            case MUL :
                pred += (temp[i] - components[comp]->FDS_prob[i]) * states[st]->DGMUL[i];
                break;
            case DIV :
                pred += (temp[i] - components[comp]->FDS_prob[i]) * states[st]->DGDIV[i];
                break;
            case LOG :
                pred += (temp[i] - components[comp]->FDS_prob[i]) * states[st]->DGLOG[i];
                break;
            default : break;
        }
    }
    return pred;
}

double hlsm::succ_force(int comp, int start) {
    double temp[MAX_LAT] = {0.0};
    double succ = 0.0;
    int st = components[comp]->state;
    int ASAP = start;                               // Hypothetical new ASAP of predecessor
    double prob = 1.0/double(components[comp]->ALAP_lat - ASAP + 1);    // Hypothetical new probability of scheduling predecessor

    for (int i = ASAP; i <= components[comp]->ALAP_lat; i++) {
        for (int j = 0; j < components[comp]->num_cyc; j++) {
            temp[i+j] += prob;                                          // Hypothetical new FDS_prob
        }
    }

    for (int i = 0; i < states[st]->num_cyc; i++) {
        switch (components[comp]->op) {
            case ADD :
                succ += (temp[i] - components[comp]->FDS_prob[i]) * states[st]->DGADD[i];
                break;
            case MUL :
                succ += (temp[i] - components[comp]->FDS_prob[i]) * states[st]->DGMUL[i];
                break;
            case DIV :
                succ += (temp[i] - components[comp]->FDS_prob[i]) * states[st]->DGDIV[i];
                break;
            case LOG :
                succ += (temp[i] - components[comp]->FDS_prob[i]) * states[st]->DGLOG[i];
                break;
            default : break;
        }
    }
    return succ;
}

int hlsm::pred_ALAP(int comp) {
    int ALAP = components[comp]->ALAP_lat;
    for (auto it : components[comp]->ins) {
        if ((!components[it]->FDS_scheduled) && (components[it]->ALAP_lat > ALAP - components[it]->num_cyc)) {
            components[it]->ALAP_lat = ALAP - components[it]->num_cyc;
            pred_ALAP(it);
        }
    }
    return 0;
}

int hlsm::succ_ASAP(int comp) {
    int ASAP = components[comp]->ASAP_lat;
    for (auto it : components[comp]->outs) {
        if ((!components[it]->FDS_scheduled) && (components[it]->ASAP_lat < ASAP + components[comp]->num_cyc)) {
            components[it]->ASAP_lat = ASAP + components[comp]->num_cyc;
            succ_ASAP(it);
        }
    }
    return 0;
}