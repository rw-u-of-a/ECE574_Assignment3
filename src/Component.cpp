//
//  Component.cpp
//  
//
//  Created by Jake Given on 10/26/17.
//
//

#include "Component.hpp"

Component::Component(string line){
    else if(line.find("wire")!=string::npos){
        type = string("REG");
        dataInputs.push_back(string("d"));
        controlInputs.push_back(string("Clk"));
        controlInputs.push_back(string("Rst"));
        dataOutputs.push_back(string("q"));
        description = string("Register");
        
    }    
	else if(line.find("input")!=string::npos){
        type = string("input");
        
    }    
	else if(line.find("output")!=string::npos){
        type = string("output");
        
    }
    else{
        //not a valid component
    }
}