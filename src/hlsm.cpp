//
// Created by Joe Loubert on 11/22/2017.
//

#include "hlsm.h"

int hlsm::inop(int id) {
    auto it = components.find(id);
    if (it == components.end()) {
        cout << "inop cannot be defined using id "<<id<<endl;
        return -1;
    }
    add_state(0);
    states[0]->start_cyc = 0;
    states[0]->last_cyc = 0;
    states[0]->num_cyc = 1;
    states[0]->is_parallel = false;
    add_branch("", 0, 1, 1);
    components[id]->state = 0;
    components[id]->start_cyc = 0;
    components[id]->num_cyc = 1;
    components[id]->end_cyc = 1;
    components[id]->out_str = "";
    components[id]->is_scheduled = true;
    return 0;
}

int hlsm::add_state(int st) {
    statedef *s;
    s = new statedef(st);
    states[st] = s;
    return 0;
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

int hlsm::clear() {
    for (auto it : components) {
        it.second->is_scheduled = false;
    }
    num_scheduled = 0;
    return 0;
}

int hlsm::ASAP_me(int i) {
    if(components[i]->is_scheduled)
        return components[i]->end_cyc;

    int max_cyc = 0;
    int temp;
    for (auto it : components[i]->ins) {
        temp = ASAP_me(it);
        if (temp > max_cyc)
            max_cyc = temp;
    }

    components[i]->start_cyc = max_cyc;
    components[i]->end_cyc = max_cyc + components[i]->num_cyc;
    components[i]->is_scheduled = true;
    num_scheduled++;
    if (components[i]->end_cyc > states[components[i]->state]->num_cyc)
        states[components[i]->state]->num_cyc = components[i]->end_cyc;
    return components[i]->end_cyc;
}

int hlsm::ASAP(int st){
    int max_cyc = 0;
    for (auto it : components) {
        if (it.second->state == st) {
            int temp = ASAP_me(it.first);
            if (temp > max_cyc)
                max_cyc = temp;
            int cyc = it.second->start_cyc;
            auto it2 = states[st]->cycmap.find(cyc);
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
