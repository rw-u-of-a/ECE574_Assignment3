//
// Created by Joe Loubert on 10/31/2017.
//

#include <iostream>
#include <fstream>
#include <map>
#include <stack>
#include <boost/regex.hpp>
#include "hlsm.h"

using namespace std;
using namespace boost;

int main(int argc, char ** argv) {
    string temp, temp2, temp3;
    if (argc != 4) {//uncomment for running in command line
        cout << "Usage: "<<argv[0]<<" cFile latency verilogFile" << endl;
        return 0;
    }
    string cFile = string(argv[1]);
    int latency = stoi(argv[2]);
    string verilogFile = string(argv[3]);
    string line;
    if (latency > MAX_LAT) {
        cout << "Latency > "<<MAX_LAT<<" not allowed. Edit MAX_LAT in hlsm.h to increase limit."<<endl;
        return -1;
    }
    hlsm G;

    string name, oline;
    string in1, in2, in3, out, oper;
    vector<string> olines;
    vector<string> lines;
    bool is_signed;
    bool multiple;
    int dw;
    regex emptyrgx("^[[:space:]]*(?:$|//)");
    regex declarergx("^[[:space:]]*(input|output|variable)[[:space:]]+(U)?Int([[:digit:]]+)[[:space:]]+"
                             "([[:print:]]+)(?:$|//)");
    regex vblesrgx("^([[:alnum:]]+)[,]?[[:space:]]*([[:print:]]*)");
    regex comrgx("^[[:space:]]*([[:alnum:]]+)[[:space:]]*=[[:space:]]*([[:alnum:]]+)[[:space:]]*"
                         "(\\+|-|\\*|<|>|==|>>|<<|/|%)"
                         "[[:space:]]*([[:alnum:]]+)[[:space:]]*(?:$|//)");
    regex muxrgx("^[[:space:]]*([[:alnum:]]+)[[:space:]]*=[[:space:]]*([[:alnum:]]+)[[:space:]]*"
                         "\\?[[:space:]]*([[:alnum:]]+)[[:space:]]*:[[:space:]]*([[:alnum:]]+)[[:space:]]*(?:$|//)");
    regex ifrgx("^[[:space:]]*if[[:space:]]*\\([[:space:]]*([[:alnum:]]+)[[:space:]]*\\)[[:space:]]*\\{[[:space:]]*(?:$|//)");
    regex bracrgx("^[[:space:]]*\\}[[:space:]]*(?:$|//)");
    regex elsergx("^[[:space:]]*else[[:space:]]*\\{[[:space:]]*(?:$|//)");
    smatch result;
    int id = 1;         // id 0 is inop, id -1 is onop
    int cur_state = 1;  // state 0 is wait
    readstate cur_readstate = READ;
    stack<pair<int,readstate>> state_stack;
    int next_avail_state = 2;

    ifstream infile(cFile);
    if(infile.is_open()){
        while(getline(infile,line)){
            lines.push_back(line);
        }
        infile.close();
    }
    else{
        cout << "File "<<cFile<<" could not be opened"<<endl;
        return 0;
    }
    temp3 = verilogFile;
    size_t pos = temp3.find(".v");
    string netlistname = temp3.substr(0,pos);

    olines.push_back("module " + netlistname + "(Clk, Rst, Start, Done");
    olines.push_back("  input Clk, Rst, Start;\n");
    olines.push_back("  output reg Done;\n");

    G.add_component(0, 0, "");  // inop
    G.inop(0);                  // inop

    G.add_state(cur_state);     // add the first state

    for(vector<string>::iterator itr = lines.begin(); itr != lines.end(); ++itr) {
        line = *itr;

        if (regex_search(line, result, emptyrgx)) {}        // Is the line empty?

        else if (regex_search(line, result, declarergx)) {  // Does the line declare variables?
            string dectype = result[1];
            is_signed = (result[2] != "U");                 // Was there no U in front of Int?
            dw = stoi(result[3]);                           // Datawidth
            string vbles = result[4];                       // List of variables

            if (dectype == "input") {
                if (dw == 1)                                // Single bit
                    oline = "  input";
                else
                    oline = "  input [" + to_string(dw - 1) + ":0]";
                multiple = false;                               // Are multiple variables declared in this line?
                while (regex_search(vbles, result, vblesrgx)) { // While there are more inputs to declare
                    if (multiple)
                        oline = oline + ",";
                    multiple = true;
                    name = result[1];
                    oline = oline + " " + name;
                    G.add_wire(name, is_signed, dw);    // Add the wire to the HLSM
                    G.wires[name]->from = 0;            // These wires come from inop
                    G.wires[name]->Rst = false;
                    vbles = result[2];
                    olines[0] += ", ";
                    olines[0] += name;
                }
            }

            else if (dectype == "output") {
                if (dw == 1)
                    oline = "  output reg";             // Because "output reg [0:0]" looks ugly
                else
                    oline = "  output reg [" + to_string(dw - 1) + ":0]";
                multiple = false;
                while (regex_search(vbles, result, vblesrgx)) { // While there are more outputs to declare
                    if (multiple)
                        oline = oline + ",";
                    multiple = true;
                    name = result[1];
                    oline = oline + " " + name;
                    G.add_wire(name, is_signed, dw);    // Add the wire to the HLSM
                    G.wires[name]->to = {-1};           // These wires go to onop
                    G.wires[name]->Rst = true;
                    vbles = result[2];
                    olines[0] += ", ";
                    olines[0] += name;
                }
            }

            else if (dectype == "variable") {
                if (dw == 1)
                    oline = "  reg";
                else
                    oline = "  reg [" + to_string(dw - 1) + ":0]";
                multiple = false;
                while (regex_search(vbles, result, vblesrgx)) { // While there are more regs to declare
                    if (multiple)
                        oline = oline + ",";
                    multiple = true;
                    name = result[1];                           // name = next reg
                    oline = oline + " " + name;
                    G.add_wire(name, is_signed, dw);
                    G.wires[name]->Rst = true;
                    vbles = result[2];
                }
            }
            oline = oline + ";\n";
            olines.push_back(oline);
        }

        else if (regex_search(line, result, comrgx)) {
            if (cur_readstate != READ) {
                int prev = cur_state;
                cur_state = next_avail_state;
                next_avail_state++;
                G.add_state(cur_state);
                G.set_child_state(prev, cur_state);

                cur_readstate = READ;
            }

            out = result[1];
            in1 = result[2];
            oper = result[3];
            in2 = result[4];

            if(!G.wires[out] || !G.wires[in1] || !G.wires[in2]){
                cout << "Undeclared variable in line:"<<line<<endl;
                return -1;
            }
            G.add_component(id, cur_state, out + " <= " + in1 + " " + oper + " " + in2 + ";");
            G.wire_to_component(in1, id);
            G.wire_to_component(in2, id);
            G.wire_from_component(out, id);

            if (oper == "*") {
                G.components[id]->num_cyc = 2;
                G.components[id]->op = MUL;
            }
            else if ((oper == "/") || (oper == "%")) {
                G.components[id]->num_cyc = 3;
                G.components[id]->op = DIV;
            }
            else if ((oper == "+") || (oper == "-")) {
                G.components[id]->num_cyc = 1;
                G.components[id]->op = ADD;
            }
            else {
                G.components[id]->num_cyc = 1;
                G.components[id]->op = LOG;
            }

            id++;
        }

        else if (regex_search(line, result, muxrgx)) {
            if (cur_readstate != READ) {
                int prev = cur_state;

                cur_state = next_avail_state;
                next_avail_state++;
                G.add_state(cur_state);

                G.set_child_state(prev, cur_state);

                cur_readstate = READ;
            }

            out = result[1];
            in1 = result[2];
            in2 = result[3];
            in3 = result[4];

            if(!G.wires[out] || !G.wires[in1] || !G.wires[in2] || !G.wires[in3]){
                cout << "Undeclared variable in line: "<<line<<endl;
                return -1;
            }
            G.add_component(id, cur_state, out + " <= " + in1 + " ? " + in2 + " : " + in3 + ";");
            G.wire_to_component(in1, id);
            G.wire_to_component(in2, id);
            G.wire_to_component(in3, id);
            G.wire_from_component(out, id);
            G.components[id]->num_cyc = 1;
            G.components[id]->op = LOG;

            id++;
        }

        else if (regex_search(line, result, ifrgx)) {
            if (cur_readstate != READ) {
                int prev = cur_state;

                cur_state = next_avail_state;
                next_avail_state++;
                G.add_state(cur_state);

                G.set_child_state(prev, cur_state);

                cur_readstate = READ;
            }

            G.add_component(id, cur_state, "");             // We have to create an operation for the if statement
            G.wire_to_component(result[1], id);             // To ensure that its condition has been calculated
            G.components[id]->num_cyc = 1;                  // By the time the 'if' statement shows up
            G.components[id]->op = IF1;                     // For scheduling purposes, let's give it its own type.
            id++;

            G.states[cur_state]->sbranch.cond = result[1];
            int if_s = next_avail_state;
            next_avail_state++;
            G.add_state(if_s);

            G.states[cur_state]->sbranch.if_state = if_s;   // Branch to newly created state
            G.states[cur_state]->sbranch.else_state = -1;   // By default else branches to final state

            cur_readstate = IF;
            state_stack.push(make_pair(cur_state, cur_readstate));

            cur_state = if_s;                               // Move into the newly created state branch
            cur_readstate = READ;                           // Get ready to read the next line
        }

        else if (regex_search(line, result, bracrgx)) {
            if (state_stack.empty()) {
                cout << "Syntax error: extra '}'" << endl;
                return -1;
            }
            cur_state = state_stack.top().first;
            cur_readstate = state_stack.top().second;
            state_stack.pop();
        }

        else if (regex_search(line, result, elsergx)) {
            if (cur_readstate != IF) {
                cout << "Syntax error: incorrect 'else'" << endl;
                return -1;
            }
            int else_state = next_avail_state;
            next_avail_state++;
            G.add_state(else_state);
            G.states[cur_state]->sbranch.else_state = else_state;
            G.states[cur_state]->sbranch.is_else = true;

            cur_readstate = ELSE;
            state_stack.push(make_pair(cur_state, cur_readstate));

            cur_state = G.states[cur_state]->sbranch.else_state;
            cur_readstate = READ;
        }

        else {
            cout << "Error with line:"<<line<<endl;
            return -1;
        }
    }

    olines[0] += ");\n";

    cur_state = next_avail_state;                // Add the "done" state
    next_avail_state++;
    G.add_state(cur_state);
    G.add_component(id, cur_state, "Done <= 1;");
    G.components[id]->num_cyc = 1;
    id++;
    G.add_branch("", cur_state, 0, 0);          // Loop back to the Wait state



    // *Schedule stuff goes here*
    unsigned int s = 0;
    for (s = 0; s < G.states.size(); s++) {
        G.ASAP(s);
    }
    G.min_succ_lat(1);              // Calculate the minimum latency needed for each state
    G.unsched_all();                // Unschedule all the states
    G.ASAP(cur_state);              // We shouldn't unschedule the Done state though
    if (G.ALAP_succ(1, latency) < 0) {
        cout << "Latency too short" << endl;
        return -1;
    }
    G.unsched_all();                // Unschedule all the states
    G.ASAP(cur_state);              // We shouldn't unschedule the Done state though
    for (s = 1; s < G.states.size()-1; s++) {     // Skip states Wait and Done
        G.FDS(s);
    }

    // Calculate total number of states
    // And convert all '-1' states to correct Done state
    int j = 0;
    for (s = 0; s < G.states.size(); s++) {
        G.states[s]->start_cyc = j;
        G.states[s]->last_cyc = j + G.states[s]->num_cyc - 1;
        j += G.states[s]->num_cyc;
        if (G.states[s]->sbranch.if_state == -1)
            G.states[s]->sbranch.if_state = cur_state;
        if (G.states[s]->sbranch.else_state == -1)
            G.states[s]->sbranch.else_state = cur_state;
    }
    j -= 1;     // Overcounted by one
    int q = 0;  // Number of bits in State, minus one
    while (j > 0){
        j = j >> 1;
        q++;
    }


    // Output stuff goes here
    olines.push_back("  reg [" + to_string(q-1) + ":0] State;\n");
    olines.push_back("  always @(posedge Clk) begin\n");
    olines.push_back("    if (Rst) begin\n");
    for (auto it : G.wires)
        if (it.second->Rst)
            olines.push_back("      " + it.second->name + " <= 0;\n");
    olines.push_back("      State <= 0;\n");
    olines.push_back("      Done <= 0;\n");
    olines.push_back("    end else begin\n");
    olines.push_back("      case (State)\n");

    for (unsigned int i = 0; i < G.states.size(); i++) {
        for (int j = 0; j < G.states[i]->num_cyc; j++) {
            int abs_cyc = G.states[i]->start_cyc + j;
            olines.push_back("        " + to_string(abs_cyc) + " : begin\n");
            auto it = G.states[i]->cycmap.find(j);
            if (it == G.states[i]->cycmap.end()) {}     // cycmap has no entry for cycle j
            else {
                for (auto it2 : G.states[i]->cycmap[j]) {
                    if (!G.components[it2]->out_str.empty()) {
                        olines.push_back("            " + G.components[it2]->out_str + "\n");
                    }
                }
            }
            if (abs_cyc < G.states[i]->last_cyc) {
                olines.push_back("            State <= " + to_string(abs_cyc+1) + ";\n");
            }
            else {          // On the last cycle, branch to next state
                int next_state = G.states[i]->sbranch.if_state;
                int next_cyc = G.states[next_state]->start_cyc;
                if (G.states[i]->sbranch.cond.empty()) {
                    olines.push_back("            State <= " + to_string(next_cyc) + ";\n");
                }
                else {
                    olines.push_back("            if (" + G.states[i]->sbranch.cond +") begin\n");
                    olines.push_back("              State <= " + to_string(next_cyc) + ";\n");
                    olines.push_back("            end else begin\n");
                    next_state = G.states[i]->sbranch.else_state;
                    next_cyc = G.states[next_state]->start_cyc;
                    olines.push_back("              State <= " + to_string(next_cyc) + ";\n");
                    olines.push_back("            end\n");
                }
            }
            olines.push_back("          end\n");
        }
    }

    olines.push_back("        default : State <= 0;\n");
    olines.push_back("      endcase\n");
    olines.push_back("    end\n");
    olines.push_back("  end\n");
    olines.push_back("endmodule\n");

    ofstream outfile(verilogFile);

    if(outfile.is_open()) {
        for (vector<string>::iterator itr = olines.begin(); itr != olines.end(); ++itr) {
            outfile << *itr;
        }
    }
    else {
        cout << "Unable to write to file "<<verilogFile<<endl;
        return -1;
    }

    return 0;
}
