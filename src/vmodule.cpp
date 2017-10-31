//
//  vmodule.cpp
//  
//
//  Created by Jake Given on 10/27/17.
//
//
#include "vmodule.h"
#include <stdio.h>

map<string, map<int, double>> make_latencies(){
    return {{"REG",
                    {{1, 2.616},
                            {2, 2.644},
                            {8, 2.879},
                            {16, 3.061},
                            {32, 3.602},
                            {64, 3.966}}},
            {"ADD",
                    {{1, 2.704},
                            {2, 3.713},
                            {8, 4.924},
                            {16, 5.638},
                            {32, 7.270},
                            {64, 9.566}}},
            {"SUB",
                    {{1, 3.024},
                            {2, 3.412},
                            {8, 4.890},
                            {16, 5.569},
                            {32, 7.253},
                            {64, 9.566}}},
            {"MUL",
                    {{1, 2.438},
                            {2, 3.651},
                            {8, 7.453},
                            {16, 7.811},
                            {32, 12.395},
                            {64, 15.354}}},
            {"COMP",
                    {{1, 3.031},
                            {2, 3.934},
                            {8, 5.949},
                            {16, 6.256},
                            {32, 7.264},
                            {64, 8.416}}},
            {"MUX2x1",
                    {{1, 4.083},
                            {2, 4.115},
                            {8, 4.815},
                            {16, 5.623},
                            {32, 8.079},
                            {64, 8.766}}},
            {"SHR",
                    {{1, 3.644},
                            {2, 4.007},
                            {8, 5.178},
                            {16, 6.460},
                            {32, 8.819},
                            {64, 11.095}}},
            {"SHL",
                    {{1, 3.614},
                            {2, 3.980},
                            {8, 5.152},
                            {16, 6.549},
                            {32, 8.565},
                            {64, 11.220}}},
            {"DIV",
                    {{1, 0.619},
                            {2, 2.144},
                            {8, 15.439},
                            {16, 33.093},
                            {32, 86.312},
                            {64, 243.233}}},
            {"MOD",
                    {{1, 0.758},
                            {2, 2.149},
                            {8, 16.078},
                            {16, 35.563},
                            {32, 88.142},
                            {64, 250.583}}},
            {"INC",
                    {{1, 1.792},
                            {2, 2.218},
                            {8, 3.111},
                            {16, 3.471},
                            {32, 4.347},
                            {64, 6.200}}},
            {"DEC",
                    {{1, 1.792},
                            {2, 2.218},
                            {8, 3.108},
                            {16, 3.701},
                            {32, 4.685},
                            {64, 6.503}}}};
};


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

void vmodule::add_wire(const string &name, bool i, int d) {
    wmap::iterator itr = wires.find(name);
    if (itr == wires.end()) {
        wire *v;
        v = new wire(name, i, d);
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
