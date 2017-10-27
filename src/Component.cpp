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
        
    }

    else if(line.find("ADD")!=string::npos){
        
    }
    else if(line.find("SUB")!=string::npos){
        
    }
    else if(line.find("MUL")!=string::npos) {
        
    }
    else if(line.find("COMP")!=string::npos){
        
    }
    else if(line.find("MUX2x1")!=string::npos){
    
    }
    else if(line.find("SHR")!=string::npos){
        
    }
    else if(line.find("SHL")!=string::npos){
        
    }
    else if(line.find("DIV")!=string::npos){
        
    }
    else if(line.find("MOD")!=string::npos){
        
    }
    else if(line.find("INC")!=string::npos){
        
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
