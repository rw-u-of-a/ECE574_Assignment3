//
//  Component.cpp
//  
//
//  Created by Jake Given on 10/26/17.
//
//

#include "Component.hpp"
#include <vector>

Component::Component(String line){
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
        DataInputs.pushback(string("d"));
        ControlInputs.pushback(string("Clk"));
        ControlInputs.pushback(string("Rst"));
        DataOutputs.pushback(string("q"));
        Description = string("Register");
        
    }
    else{
        //not a valid component
    }
}
