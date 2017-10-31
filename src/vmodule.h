#ifndef VMODULE_H
#define VMODULE_H

#include <iostream>
#include <vector>
#include <map>

using namespace std;

map<string, map<int, double>> make_latencies();

struct component {
    string name;
    double delay;
    vector<string> ins;
    vector<string> outs;
    bool is_completed;
    component(string s, double d) :
            name(s),
            delay(d),
            is_completed(false)
    {}
};

struct wire {
    string name;
    double latency;
    string from;
    vector<string> tos;
    bool is_completed;
    bool is_signed;
    int datawidth;
    wire(string s, bool i, int d) :
            name(s),
            latency(0),
            is_completed(false),
            is_signed(i),
            datawidth(d)
    {}
};

class vmodule
{
public:
    typedef map<string, component *> cmap;
    typedef map<string, wire *> wmap;
    cmap components;
    wmap wires;
    int num_incomplete = 0;
    double max_latency = 0;
    void add_component(const string&, double);
    void wire_to_component(const string&, const string&);
    void wire_from_component(const string&, const string&);
    void add_wire(const string&, bool, int);
    void make_register(const string&);
    void propagate(const string&);
};

#endif //VMODULE_H
