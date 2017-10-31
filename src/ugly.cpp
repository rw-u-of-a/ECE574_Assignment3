//
// Created by Joe Loubert on 10/31/2017.
//

#include <iostream>
#include <fstream>
#include <map>
#include <regex>
#include "vmodule.h"

using namespace std;

int main(int argc, char ** argv)
{
    string temp,temp2;
//    if(argc != 3){
//        cout <<"Incorrect number of arguments"<<endl;
//        return 0;
//    }
    string netlistFile = "../src/circuits/"+string(argv[1]);//474a_circuit2.txt";
    string verilogFile = "../src/circuits/"+string(argv[2]);//circuit2.v";
    vmodule G;
    string line, name, oline, in1, in2, in3, out1;
    vector<string> olines;
    vector<string> lines;
    bool is_signed;
    bool multiple;
    int dw;
    regex inputrgx ("[[:space:]]*input[[:space:]]+(U)?Int([[:digit:]]+)[[:space:]]+([[:print:]]+)");
    regex outputrgx ("[[:space:]]*output[[:space:]]+(U)?Int([[:digit:]]+)[[:space:]]+([[:print:]]+)");
    regex wirergx ("[[:space:]]*wire[[:space:]]+(U)?Int([[:digit:]]+)[[:space:]]+([[:print:]]+)");
    regex vblesrgx ("([[:alnum:]]+)[,]*[[:space:]]*([[:print:]]*)");
    regex regrgx ("[[:space:]]*([[:alnum:]]+)[[:space:]]*=[[:space:]]*([[:alnum:]]+)[[:space:]]*$");
    regex addrgx ("[[:space:]]*([[:alnum:]]+)[[:space:]]*=[[:space:]]*([[:alnum:]]+)[[:space:]]*\\+[[:space:]]*([[:alnum:]]+)");
    regex subrgx ("[[:space:]]*([[:alnum:]]+)[[:space:]]*=[[:space:]]*([[:alnum:]]+)[[:space:]]*-[[:space:]]*([[:alnum:]]+)");
    regex mulrgx ("[[:space:]]*([[:alnum:]]+)[[:space:]]*=[[:space:]]*([[:alnum:]]+)[[:space:]]*\\*[[:space:]]*([[:alnum:]]+)");
    regex ltrgx ("[[:space:]]*([[:alnum:]]+)[[:space:]]*=[[:space:]]*([[:alnum:]]+)[[:space:]]*<[[:space:]]*([[:alnum:]]+)");
    regex gtrgx ("[[:space:]]*([[:alnum:]]+)[[:space:]]*=[[:space:]]*([[:alnum:]]+)[[:space:]]*>[[:space:]]*([[:alnum:]]+)");
    regex eqrgx ("[[:space:]]*([[:alnum:]]+)[[:space:]]*=[[:space:]]*([[:alnum:]]+)[[:space:]]*==[[:space:]]*([[:alnum:]]+)");
    regex muxrgx ("[[:space:]]*([[:alnum:]]+)[[:space:]]*=[[:space:]]*([[:alnum:]]+)[[:space:]]*\\?[[:space:]]*([[:alnum:]]+)[[:space:]]*:[[:space:]]*([[:alnum:]]+)");
    regex shrrgx ("[[:space:]]*([[:alnum:]]+)[[:space:]]*=[[:space:]]*([[:alnum:]]+)[[:space:]]*>>[[:space:]]*([[:alnum:]]+)");
    regex shlrgx ("[[:space:]]*([[:alnum:]]+)[[:space:]]*=[[:space:]]*([[:alnum:]]+)[[:space:]]*<<[[:space:]]*([[:alnum:]]+)");
    regex divrgx ("[[:space:]]*([[:alnum:]]+)[[:space:]]*=[[:space:]]*([[:alnum:]]+)[[:space:]]*/[[:space:]]*([[:alnum:]]+)");
    regex modrgx ("[[:space:]]*([[:alnum:]]+)[[:space:]]*=[[:space:]]*([[:alnum:]]+)[[:space:]]*%[[:space:]]*([[:alnum:]]+)");
    regex incrgx ("[[:space:]]*([[:alnum:]]+)[[:space:]]*=[[:space:]]*([[:alnum:]]+)[[:space:]]*\\+[[:space:]]*1[[:space:]]*$");
    regex decrgx ("[[:space:]]*([[:alnum:]]+)[[:space:]]*=[[:space:]]*([[:alnum:]]+)[[:space:]]*-[[:space:]]*1[[:space:]]*$");
    int reg_idx = 1, add_idx = 1, sub_idx = 1, mul_idx = 1, comp_idx = 1, mux_idx = 1, shr_idx = 1, shl_idx = 1,
            div_idx = 1, mod_idx = 1, inc_idx = 1, dec_idx = 1;
    smatch result;
    map<string, map<int, double>> latencies;
    latencies = make_latencies();

    ifstream infile(netlistFile);
    if(infile.is_open()){
        while(getline(infile,line)){
            lines.push_back(line);
        }
        infile.close();
    }
    size_t pos = netlistFile.find(".");
    string netlistname = netlistFile.substr(0,pos);

    olines.push_back("module " + netlistname + "(Clk, Rst");

    for(vector<string>::iterator itr = lines.begin(); itr != lines.end(); ++itr) {
        line = *itr;

        if (regex_search(line, result, inputrgx)) { // Does the line declare inputs?
            is_signed = (result[1] != "U");         // Was there no U in front of Int?
            dw = stoi(result[2]);                   // Datawidth
            string vbles = result[3];               // List of variables
            if (dw == 1)                            // Single bit
                oline = "input";
            else
                oline = "input [" + to_string(dw - 1) + ":0]";
            multiple = false;
            while (regex_search(vbles, result, vblesrgx)) {
                if (multiple)
                    oline = oline + ",";
                multiple = true;
                name = result[1];
                oline = oline + " " + name;
                G.add_wire(name, is_signed, dw);    // Add the wire to the vmodule
                G.wires[name]->is_completed = true; // These are inputs, so mark as completed
                vbles = result[2];
                olines[0] += ", ";
                olines[0] += name;
            }
            oline = oline + ";\n";
            olines.push_back(oline);
        }

        else if (regex_search(line, result, outputrgx)) {// Does the line declare outputs?
            is_signed = (result[1] != "U");         // Was there no U in front of Int?
            dw = stoi(result[2]);                   // Datawidth
            string vbles = result[3];               // List of variables
            if (dw == 1)
                oline = "output";
            else
                oline = "output [" + to_string(dw - 1) + ":0]";
            multiple = false;
            while (regex_search(vbles, result, vblesrgx)) { // While there are more wires to declare
                if (multiple)
                    oline = oline + ",";
                multiple = true;
                name = result[1];                           // name = next wire
                oline = oline + " " + name;
                G.add_wire(name, is_signed, dw);
                vbles = result[2];
                olines[0] += ", ";
                olines[0] += name;
            }
            oline = oline + ";\n";
            olines.push_back(oline);
        }

        else if (regex_search(line, result, wirergx)) {     // Does the line declare wires?
            is_signed = (result[1] != "U");         // Was there no U in front of Int?
            dw = stoi(result[2]);                   // Datawidth
            string vbles = result[3];               // List of variables
            if (dw == 1)
                oline = "wire";
            else
                oline = "wire [" + to_string(dw - 1) + ":0]";
            multiple = false;
            while (regex_search(vbles, result, vblesrgx)) { // While there are more wires to declare
                if (multiple)
                    oline = oline + ",";
                multiple = true;
                name = result[1];                           // name = next wire
                oline = oline + " " + name;
                G.add_wire(name, is_signed, dw);
                vbles = result[2];
            }
            oline = oline + ";\n";
            olines.push_back(oline);
        }

        else if (regex_search(line, result, regrgx)) {      // Is the component a REG?
            name = "REG_" + to_string(reg_idx);
            reg_idx++;
            out1 = result[1];
            in1 = result[2];
            dw = G.wires[in1]->datawidth;
            G.add_component(name, latencies["REG"][dw]);
            G.wire_to_component(in1, name);
            G.wire_from_component(out1, name);
            G.make_register(name);
            oline = "REG #(.DATAWIDTH(" + to_string(dw) + ")) " + name +
                    "(.d(" + in1 + "), .q(" + out1 + "), .Clk(Clk), .Rst(Rst));\n";
            olines.push_back(oline);
        }

        else if (regex_search(line, result, incrgx)) {      // Is the component an INC?
            name = "INC_" + to_string(inc_idx);
            inc_idx++;
            out1 = result[1];
            in1 = result[2];
            dw = G.wires[in1]->datawidth;
            G.add_component(name, latencies["INC"][dw]);
            G.wire_to_component(in1, name);
            G.wire_from_component(out1, name);
            oline = "INC #(.DATAWIDTH(" + to_string(dw) + ")) " + name +
                    "(.a(" + in1 + "), .d(" + out1 + "));\n";
            olines.push_back(oline);
        }

        else if (regex_search(line, result, decrgx)) {      // Is the component a DEC?
            name = "DEC_" + to_string(dec_idx);
            dec_idx++;
            out1 = result[1];
            in1 = result[2];
            dw = G.wires[in1]->datawidth;
            G.add_component(name, latencies["DEC"][dw]);
            G.wire_to_component(in1, name);
            G.wire_from_component(out1, name);
            oline = "DEC #(.DATAWIDTH(" + to_string(dw) + ")) " + name +
                    "(.a(" + in1 + "), .d(" + out1 + "));\n";
            olines.push_back(oline);
        }

        else if (regex_search(line, result, addrgx)) {      // Is the component an ADD?
            name = "ADD_" + to_string(add_idx);
            add_idx++;
            out1 = result[1];
            in1 = result[2];
            in2 = result[3];
            dw = max(G.wires[in1]->datawidth, G.wires[in2]->datawidth);
            is_signed = G.wires[in1]->is_signed && G.wires[in2]->is_signed;  // Are both inputs signed?
            G.add_component(name, latencies["ADD"][dw]);
            G.wire_to_component(in1, name);
            G.wire_to_component(in2, name);
            G.wire_from_component(out1, name);
//            if (is_signed)
//                oline = "S";
//            else
                oline = "";
            oline += "ADD #(.DATAWIDTH(" + to_string(dw) + ")) " + name +
                     "(.a(" + in1 + "), .b(" + in2 + "), .sum(" + out1 + "));\n";
            olines.push_back(oline);
        }

        else if (regex_search(line, result, subrgx)) {      // Is the component a SUB?
            name = "SUB_" + to_string(sub_idx);
            sub_idx++;
            out1 = result[1];
            in1 = result[2];
            in2 = result[3];
            dw = max(G.wires[in1]->datawidth, G.wires[in2]->datawidth);
            is_signed = G.wires[in1]->is_signed && G.wires[in2]->is_signed;  // Are both inputs signed?
            G.add_component(name, latencies["SUB"][dw]);
            G.wire_to_component(in1, name);
            G.wire_to_component(in2, name);
            G.wire_from_component(out1, name);
//            if (is_signed)
//                oline = "S";
//            else
                oline = "";
            oline = oline + "SUB #(.DATAWIDTH(" + to_string(dw) + ")) " + name +
                    "(.a(" + in1 + "), .b(" + in2 + "), .diff(" + out1 + "));\n";
            olines.push_back(oline);
        }

        else if (regex_search(line, result, mulrgx)) {      // Is the component a MUL?
            name = "MUL_" + to_string(mul_idx);
            mul_idx++;
            out1 = result[1];
            in1 = result[2];
            in2 = result[3];
            dw = max(G.wires[in1]->datawidth, G.wires[in2]->datawidth);
            is_signed = G.wires[in1]->is_signed && G.wires[in2]->is_signed;  // Are both inputs signed?
            G.add_component(name, latencies["MUL"][dw]);
            G.wire_to_component(in1, name);
            G.wire_to_component(in2, name);
            G.wire_from_component(out1, name);
//            if (is_signed)
//                oline = "S";
//            else
                oline = "";
            oline = oline + "MUL #(.DATAWIDTH(" + to_string(dw) + ")) " + name +
                    "(.a(" + in1 + "), .b(" + in2 + "), .prod(" + out1 + "));\n";
            olines.push_back(oline);
        }

        else if (regex_search(line, result, gtrgx)) {      // Is the component a '>'?
            name = "COMP_" + to_string(comp_idx);
            comp_idx++;
            out1 = result[1];
            in1 = result[2];
            in2 = result[3];
            dw = max(G.wires[in1]->datawidth, G.wires[in2]->datawidth);
            is_signed = G.wires[in1]->is_signed && G.wires[in2]->is_signed;  // Are both inputs signed?
            G.add_component(name, latencies["COMP"][dw]);
            G.wire_to_component(in1, name);
            G.wire_to_component(in2, name);
            G.wire_from_component(out1, name);
//            if (is_signed)
//                oline = "S";
//            else
                oline = "";
            oline += "COMP #(.DATAWIDTH(" + to_string(dw) + ")) " + name +
                     "(.a(" + in1 + "), .b(" + in2 + "), .gt(" + out1 + "));\n";
            olines.push_back(oline);
        }

        else if (regex_search(line, result, ltrgx)) {      // Is the component a '<'?
            name = "COMP_" + to_string(comp_idx);
            comp_idx++;
            out1 = result[1];
            in1 = result[2];
            in2 = result[3];
            dw = max(G.wires[in1]->datawidth, G.wires[in2]->datawidth);
            is_signed = G.wires[in1]->is_signed && G.wires[in2]->is_signed;  // Are both inputs signed?
            G.add_component(name, latencies["COMP"][dw]);
            G.wire_to_component(in1, name);
            G.wire_to_component(in2, name);
            G.wire_from_component(out1, name);
//            if (is_signed)
//                oline = "S";
//            else
                oline = "";
            oline = oline + "COMP #(.DATAWIDTH(" + to_string(dw) + ")) " + name +
                    "(.a(" + in1 + "), .b(" + in2 + "), .lt(" + out1 + "));\n";
            olines.push_back(oline);
        }

        else if (regex_search(line, result, eqrgx)) {      // Is the component a '=='?
            name = "COMP_" + to_string(comp_idx);
            comp_idx++;
            out1 = result[1];
            in1 = result[2];
            in2 = result[3];
            dw = max(G.wires[in1]->datawidth, G.wires[in2]->datawidth);
            is_signed = G.wires[in1]->is_signed && G.wires[in2]->is_signed;  // Are both inputs signed?
            G.add_component(name, latencies["COMP"][dw]);
            G.wire_to_component(in1, name);
            G.wire_to_component(in2, name);
            G.wire_from_component(out1, name);
//            if (is_signed)
//                oline = "S";
//            else
                oline = "";
            oline = oline + "COMP #(.DATAWIDTH(" + to_string(dw) + ")) " + name +
                    "(.a(" + in1 + "), .b(" + in2 + "), .eq(" + out1 + "));\n";
            olines.push_back(oline);
        }

        else if (regex_search(line, result, muxrgx)) {      // Is the component a MUX?
            name = "MUX2x1_" + to_string(mux_idx);
            mux_idx++;
            out1 = result[1];
            in1 = result[2];
            in2 = result[3];
            in3 = result[4];
            dw = max(G.wires[in2]->datawidth, G.wires[in3]->datawidth);
            G.add_component(name, latencies["MUX2x1"][dw]);
            G.wire_to_component(in1, name);
            G.wire_to_component(in2, name);
            G.wire_to_component(in3, name);
            G.wire_from_component(out1, name);
            oline = "MUX2x1 #(.DATAWIDTH(" + to_string(dw) + ")) " + name +
                    "(.a(" + in3 + "), .b(" + in2 + "), .sel(" + in1 + "), .d(" + out1 + "));\n";
            olines.push_back(oline);
        }

        else if (regex_search(line, result, shrrgx)) {      // Is the component a SHR?
            name = "SHR_" + to_string(shr_idx);
            shr_idx++;
            out1 = result[1];
            in1 = result[2];
            in2 = result[3];
            dw = G.wires[in1]->datawidth;
            G.add_component(name, latencies["SHR"][dw]);
            G.wire_to_component(in1, name);
            G.wire_to_component(in2, name);
            G.wire_from_component(out1, name);
            oline = "SHR #(.DATAWIDTH(" + to_string(dw) + ")) " + name +
                    "(.a(" + in1 + "), .sh_amt(" + in2 + "), .d(" + out1 + "));\n";
            olines.push_back(oline);
        }

        else if (regex_search(line, result, shlrgx)) {      // Is the component a SHR?
            name = "SHL_" + to_string(shl_idx);
            shl_idx++;
            out1 = result[1];
            in1 = result[2];
            in2 = result[3];
            dw = G.wires[in1]->datawidth;
            G.add_component(name, latencies["SHL"][dw]);
            G.wire_to_component(in1, name);
            G.wire_to_component(in2, name);
            G.wire_from_component(out1, name);
            oline = "SHL #(.DATAWIDTH(" + to_string(dw) + ")) " + name +
                    "(.a(" + in1 + "), .sh_amt(" + in2 + "), .d(" + out1 + "));\n";
            olines.push_back(oline);
        }

        else if (regex_search(line, result, divrgx)) {      // Is the component a DIV?
            name = "DIV_" + to_string(div_idx);
            div_idx++;
            out1 = result[1];
            in1 = result[2];
            in2 = result[3];
            dw = max(G.wires[in1]->datawidth, G.wires[in2]->datawidth);
            is_signed = G.wires[in1]->is_signed && G.wires[in2]->is_signed;  // Are both inputs signed?
            G.add_component(name, latencies["DIV"][dw]);
            G.wire_to_component(in1, name);
            G.wire_to_component(in2, name);
            G.wire_from_component(out1, name);
//            if (is_signed)
//                oline = "S";
//            else
                oline = "";
            oline = oline + "DIV #(.DATAWIDTH(" + to_string(dw) + ")) " + name +
                    "(.a(" + in1 + "), .b(" + in2 + "), .quot(" + out1 + "));\n";
            olines.push_back(oline);
        }

        else if (regex_search(line, result, modrgx)) {      // Is the component a MOD?
            name = "MOD_" + to_string(mod_idx);
            mod_idx++;
            out1 = result[1];
            in1 = result[2];
            in2 = result[3];
            dw = max(G.wires[in1]->datawidth, G.wires[in2]->datawidth);
            is_signed = G.wires[in1]->is_signed && G.wires[in2]->is_signed;  // Are both inputs signed?
            G.add_component(name, latencies["MOD"][dw]);
            G.wire_to_component(in1, name);
            G.wire_to_component(in2, name);
            G.wire_from_component(out1, name);
//            if (is_signed)
//                oline = "S";
//            else
                oline = "";
            oline = oline + "MOD #(.DATAWIDTH(" + to_string(dw) + ")) " + name +
                    "(.a(" + in1 + "), .b(" + in2 + "), .rem(" + out1 + "));\n";
            olines.push_back(oline);
        }

    }



//    for(map<string, wire *>::iterator itr = G.wires.begin(); itr != G.wires.end(); ++itr){//moved to the input, output checking sections
//        olines[0] += ", ";
//        olines[0] += itr->first;
//    }
    olines[0] += ");\n";

    olines.push_back("endmodule\n");

    ofstream outfile(verilogFile);

    if(outfile.is_open()) {
        for (vector<string>::iterator itr = olines.begin(); itr != olines.end(); ++itr) {
            outfile << *itr;
        }
    }

    while (G.num_incomplete > 0)
        for (auto comp_itr : G.components)
            G.propagate(comp_itr.first);
    cout << "Critical Path : " << G.max_latency << " ns\n";
    return 0;
}
