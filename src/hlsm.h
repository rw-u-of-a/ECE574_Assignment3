//
// Created by Joe Loubert on 11/22/2017.
//

#ifndef HLSYN_HLSM_H
#define HLSYN_HLSM_H
#define MAX_LAT 100

#include <iostream>
#include <vector>
#include <map>

using namespace std;

enum operation {ADD, MUL, LOG, DIV, IF1};
enum readstate {READ, IF, ELSE};

struct component {
    int id;             // id of the component
    int state;
    operation op;
    string out_str;
    vector<int> ins;
    vector<int> outs;
    int start_cyc;
    int end_cyc;
    int num_cyc;
    double FDS_prob[MAX_LAT];
    bool is_scheduled;
    bool FDS_scheduled;
    int ASAP_lat;
    int ALAP_lat;
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
    bool Rst;           // Is this affected by Rst?
    wire(string t, bool s, int d) :
            name(t),
            is_signed(s),
            datawidth(d),
            from(-2)    // -2 is -2
    {}
};

struct branch {
    string cond;
    int if_state;
    int else_state;
    bool is_else;   // is there an else or nah
    branch() :
        cond(""),
        if_state(-1),
        else_state(-1),
        is_else(false)
    {};

    branch(string c, int t, int f) :
            cond(c),
            if_state(t),
            else_state(f)
    {}
};

struct statedef {
    int state;
    int start_cyc;
    int last_cyc;
    int num_cyc;
    int min_lat;
    int min_succ_lat;        // Minimum latency to schedule state and successors
    double DGADD[MAX_LAT];
    double DGMUL[MAX_LAT];
    double DGLOG[MAX_LAT];
    double DGDIV[MAX_LAT];  // Too lazy to figure out the right way to do this
    bool msl_sched;
    bool is_sched;
    map<int,vector<int>> cycmap;
    branch sbranch;
    statedef(int s) :
            state(s),
            start_cyc(0),
            last_cyc(0),
            num_cyc(0),
            min_lat(0),
            min_succ_lat(0),
            msl_sched(false),
            is_sched(false)
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
    int num_components;
    int inop(int);
    int add_state(int);
    int set_child_state(int, int);
    int add_component(int, int, const string&);
    int add_wire(const string&, bool, int);
    int add_branch(const string&, int, int, int);
    int wire_to_component(const string&, int);
    int wire_from_component(const string&, int);
    int unsched(int);
    int ASAP_me(int);
    int ASAP(int);
    int min_succ_lat(int);
    int ALAP_me(int, int);
    int ALAP(int, int);
    int ALAP_succ(int, int);
    int unsched_all();
    int FDS_setup(int);
    int FDS(int);
    double total_force(int, int);
    double pred_force(int, int);
    double succ_force(int, int);
    int pred_ALAP(int);
    int succ_ASAP(int);
};

#endif //HLSYN_HLSM_H
