
#ifndef Operator_hpp
#define Operator_hpp

#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <typeinfo>

#include "Component.hpp"

using namespace std;
class Operator{
public:
    Operator(string line);
    string type;
    vector<string> op_dataInputs;
    vector<string> op_controlInputs;
    vector<string> op_dataOutputs;
    vector<string> op_controlOutputs;
    int dataWidth;
	
	vector<string> input_word;
	vector<string> output_word;

	bool valid_operator
    
	vector<string> string_split(string line, char delim);
	
	void put_operator(Operator& op, ofstream& outfile);
};

#endif 