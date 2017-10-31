//
//  Driver.cpp
//  
//
//  Created by Jake Given on 10/27/17.
//
//

#include "Driver.hpp"
#include "Component.hpp"
#include "Operator.hpp"

Driver::Driver(){
    latencies["REG"][1] = 2.616;
    latencies["REG"][2] = 2.644;
    latencies["REG"][8] = 2.879;
    latencies["REG"][16] = 3.061;
    latencies["REG"][32] = 3.602;
    latencies["REG"][64] = 3.966;
    
    latencies["ADD"][1] = 2.704;
    latencies["ADD"][2] = 3.713;
    latencies["ADD"][8] = 4.924;
    latencies["ADD"][16] = 5.638;
    latencies["ADD"][32] = 7.270;
    latencies["ADD"][64] = 9.566;
    
    latencies["SUB"][1] = 3.024;
    latencies["SUB"][2] = 3.412;
    latencies["SUB"][8] = 4.890;
    latencies["SUB"][16] = 5.569;
    latencies["SUB"][32] = 7.253;
    latencies["SUB"][64] = 9.566;
    
    latencies["MUL"][1] = 2.438;
    latencies["MUL"][2] = 3.651;
    latencies["MUL"][8] = 7.453;
    latencies["MUL"][16] = 7.811;
    latencies["MUL"][32] = 12.395;
    latencies["MUL"][64] = 15.354;
    
    latencies["COMP"][1] = 3.031;
    latencies["COMP"][2] = 3.934;
    latencies["COMP"][8] = 5.949;
    latencies["COMP"][16] = 6.256;
    latencies["COMP"][32] = 7.264;
    latencies["COMP"][64] = 8.416;
    
    latencies["MUX2x1"][1] = 4.083;
    latencies["MUX2x1"][2] = 4.115;
    latencies["MUX2x1"][8] = 4.815;
    latencies["MUX2x1"][16] = 5.623;
    latencies["MUX2x1"][32] = 8.079;
    latencies["MUX2x1"][64] = 8.766;
    
    latencies["SHR"][1] = 3.644;
    latencies["SHR"][2] = 4.007;
    latencies["SHR"][8] = 5.178;
    latencies["SHR"][16] = 6.460;
    latencies["SHR"][32] = 8.819;
    latencies["SHR"][64] = 11.095;
    
    latencies["SHL"][1] = 3.614;
    latencies["SHL"][2] = 3.980;
    latencies["SHL"][8] = 5.152;
    latencies["SHL"][16] = 6.549;
    latencies["SHL"][32] = 8.565;
    latencies["SHL"][64] = 11.220;
    
    latencies["DIV"][1] = 0.619;
    latencies["DIV"][2] = 2.144;
    latencies["DIV"][8] = 15.439;
    latencies["DIV"][16] = 33.093;
    latencies["DIV"][32] = 86.312;
    latencies["DIV"][64] = 243.233;
    
    latencies["MOD"][1] = 0.758;
    latencies["MOD"][2] = 2.149;
    latencies["MOD"][8] = 16.078;
    latencies["MOD"][16] = 35.563;
    latencies["MOD"][32] = 88.142;
    latencies["MOD"][64] = 250.583;
    
    latencies["INC"][1] = 1.792;
    latencies["INC"][2] = 2.218;
    latencies["INC"][8] = 3.111;
    latencies["INC"][16] = 3.471;
    latencies["INC"][32] = 4.347;
    latencies["INC"][64] = 6.200;
    
    latencies["DEC"][1] = 1.792;
    latencies["DEC"][2] = 2.218;
    latencies["DEC"][8] = 3.108;
    latencies["DEC"][16] = 3.701;
    latencies["DEC"][32] = 4.685;
    latencies["DEC"][64] = 6.503;
}

void Driver::Run(string netlistFilename, string circuitFilename){
    ifstream infile(circuitFilename);
    ofstream outfile(netlistFilename);
    string line;
    if(!infile.is_open()){
        cout <<"Can't open circuit file "<< circuitFilename <<endl;
        return;
    }
    if(!outfile.is_open()){
        cout << "Could not open netlist file" <<netlistFilename<< endl;
        return;
    }
    while(getline(infile,line)){
        if(line.find("input") !=string::npos){
            cout << "Handle input Component"<<endl;
            //do input stuff
            
        }
        else if(line.find("output")!=string::npos){
            cout << "Handle output Component" <<endl;
            //do output stuff
        }
        else if(line.find("wire")!=string::npos&&line.find("wire",1)==string::npos){
            //do wire stuff
            cout << "Handle wire Component" <<endl;
        }
        else {
            //handleComponent(line);
            cout << "Handle Operator" <<endl;
//			Operator temp_operator(line);
//			if (temp_operator.valid_operator) {
//				temp_operator.put_operator(temp_operator, outfile);
//			}
        }

    }
	outfile.close();
	infile.close();
    cout << "Critical path latency" <<CalculateLatency()<<endl;

}

double Driver::CalculateLatency(){
    while (G.num_incomplete > 0)
        for (auto comp_itr : G.components)
            G.propagate(comp_itr.first);
    return G.max_latency;
}

string Driver::getType(string type){
    if(type == "Int8")return string("[7:0]");
    else if (type == "Int2")return string("[1:0]");
    else if(type == "Int16")return string("[15:0]");
    else if(type == "Int16")return string("[15:0]");//FIXME
    else if(type == "Int16")return string("[15:0]");//etc
    else if(type == "Int16")return string("[15:0]");
    else if(type == "Int16")return string("[15:0]");
    else if(type == "Int16")return string("[15:0]");
    else if(type == "Int16")return string("[15:0]");
}
