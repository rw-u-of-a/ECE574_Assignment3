//
// Created by Joe Loubert on 11/22/2017.
//

#ifndef DPGEN_HLSM_H
#define DPGEN_HLSM_H

#include <iostream>
#include <vector>
#include <map>

using namespace std;

struct component {
    int id;             // id of the component
    int state;
    string out_str;
    vector<int> ins;
    vector<int> outs;
    int start_cyc;
    int end_cyc;
    int num_cyc;
    bool is_scheduled;
    component(int i, int s, string t) :
            id(i),
            state(s),
            out_str(t),
            start_cyc(0),
            is_scheduled(false)
    {}
};

struct wire {
    string name;        // name of the wire
    bool is_signed;     // signedness of the wire
    int datawidth;      // datawidth of the wire
    int from;           // id of component with wire as output
    vector<int> to;     // ids of components with wire as input
    wire(string t, bool s, int d) :
            name(t),
            is_signed(s),
            datawidth(d),
            from(-2)    // -2 is -2
    {}
};

struct branch {
    string cond;
    int from_state;
    int to_state;
    branch(string c, int f, int t) :
            cond(c),
            from_state(f),
            to_state(t)
    {}
};

struct statedef {
    int state;
    int start_cyc;
    int last_cyc;
    int num_cyc;
    map<int,vector<int>> cycmap;
    bool is_parallel;
    vector<branch> branches;
    statedef(int s) :
            state(s),
            start_cyc(0),
            last_cyc(0),
            num_cyc(0)
    {}
};

class hlsm
{
public:
    typedef map<int, component *> cmap;
    typedef map<string, wire *> wmap;
    typedef map<int, statedef *> smap;
    cmap components;
    wmap wires;
    smap states;
    int num_states;
    int num_scheduled;
    int num_components;
    int inop(int);
    int add_state(int);
    int add_component(int, int, const string&);
    int add_wire(const string&, bool, int);
    int add_branch(const string&, int, int);
    int wire_to_component(const string&, int);
    int wire_from_component(const string&, int);
    int ASAP_me(int);
    int ASAP(int);
    int clear();
};

#endif //DPGEN_HLSM_H