//
//  vmodule.cpp
//  
//
//  Created by Jake Given on 10/27/17.
//
//
#include "vmodule.h"
#include <stdio.h>

void vmodule::add_component(const string &name, double d) {
    cmap::iterator itr = components.find(name);
    if (itr == components.end()) {
        component *v;
        v = new component(name, d);
        components[name] = v;
        num_incomplete++;
        return;
    }
    cout << "\nComponent already exists!";
}

void vmodule::wire_to_component(const string &w, const string &c) {
    wmap::iterator itw = wires.find(w);
    if (itw == wires.end()) {
        cout << "\nWire does not exist!";
        return;
    }
    itw->second->tos.push_back(c);
    
    cmap::iterator itc = components.find(c);
    if (itc == components.end()) {
        cout << "\nComponent " << c << " does not exist!";
        return;
    }
    itc->second->ins.push_back(w);
}

void vmodule::wire_from_component(const string &w, const string &c){
    wmap::iterator itw = wires.find(w);
    if (itw == wires.end()) {
        cout << "\nWire does not exist!";
        return;
    }
    itw->second->from = c;
    
    cmap::iterator itc = components.find(c);
    if (itc == components.end()) {
        cout << "\nComponent " << c << " does not exist!";
        return;
    }
    itc->second->outs.push_back(w);
}

void vmodule::add_wire(const string &name, bool i) {
    wmap::iterator itr = wires.find(name);
    if (itr == wires.end()) {
        wire *v;
        v = new wire(name, i);
        wires[name] = v;
        return;
    }
    cout << "\nWire already exists!";
}

void vmodule::make_register(const string &name) {
    cmap::iterator itr = components.find(name);
    if (itr == components.end()) {
        cout << "\nComponent " << name << " does not exist!";
        return;
    }
    itr->second->is_completed = true;
    num_incomplete--;
    wires[itr->second->outs.front()]->latency = itr->second->delay;
    if(itr->second->delay > max_latency)
        max_latency = itr->second->delay;
    wires[itr->second->outs.front()]->is_completed = true;
}

void vmodule::propagate(const string &name) {
    cmap::iterator itr = components.find(name);
    double m = 0;
    if (itr == components.end()){
        cout << "\nComponent " << name << " does not exist!";
        return;
    }
    if (itr->second->is_completed == true)
        return;
    for (auto ins_itr : itr->second->ins) {
        if (wires[ins_itr]->is_completed == false)
            return;
        if (wires[ins_itr]->latency > m)
            m = wires[ins_itr]->latency;
    }
    for (auto outs_itr : itr->second->outs) {
        wires[outs_itr]->latency = m + itr->second->delay;
        wires[outs_itr]->is_completed = true;
        if (wires[outs_itr]->latency > max_latency)
            max_latency = wires[outs_itr]->latency;
    }
    num_incomplete--;
}
