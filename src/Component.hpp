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
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <typeinfo>

using namespace std;
class Component{
public:
    Component(string line);
    string type;
	string name;
    vector<string> dataInputs;
    vector<string> controlInputs;
    vector<string> dataOutputs;
    vector<string> controlOutputs;
    string description;
    int dataWidth;
    
	vector<string> string_split(string line, char delim);
};

#endif /* Component_hpp */
