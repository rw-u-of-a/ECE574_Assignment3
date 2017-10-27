//
//  Component.hpp
//  
//
//  Created by Jake Given on 10/26/17.
//
//

#ifndef Component_hpp
#define Component_hpp

#include <stdio.h>
#include <vector>
#include <string>
using namespace std;
class Component{
public:
    Component(string line);
    string type;
    vector<string> dataInputs;
    vector<string> controlInputs;
    vector<string> dataOutputs;
    vector<string> controlOutputs;
    string description;
    int dataWidth;
    
};

#endif /* Component_hpp */
