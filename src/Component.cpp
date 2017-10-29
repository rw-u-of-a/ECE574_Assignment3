//
//  Component.cpp
//  
//
//  Created by Jake Given on 10/26/17.
//
//

#include "Component.hpp"

Component::Component(string line){
    if(line.find("DEC")!=string::npos){
        //DEC
		// Not sure if this has it's own operator
		// or if we need to find 'x - 1'
    }

    else if(line.find("+")!=string::npos){
        //ADD
    }
    else if(line.find("-")!=string::npos){
        //SUB
    }
    else if(line.find("*")!=string::npos) {
        //MULT
    }
    else if(line.find(">")!=string::npos ||
			line.find("<")!=string::npos ||
			line.find("==")!=string::npos){
        //COMP
    }
    else if(line.find("?")!=string::npos &&
			line.find(":")!=string::npos){
		//MUX2x1
    }
    else if(line.find(">>")!=string::npos){
        //SHR
    }
    else if(line.find("<<")!=string::npos){
        //SHL
    }
    else if(line.find("/")!=string::npos){
        //DIV
    }
    else if(line.find("%")!=string::npos){
        //MOD
    }
    else if(line.find("INC")!=string::npos){
        //INC
		// Not sure if this has it's own operator
		// or if we need to find 'x + 1'
    }
    else if(line.find("wire")!=string::npos){
        type = string("REG");
        dataInputs.push_back(string("d"));
        controlInputs.push_back(string("Clk"));
        controlInputs.push_back(string("Rst"));
        dataOutputs.push_back(string("q"));
        description = string("Register");
        
    }
    else{
        //not a valid component
    }
}
