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
		type = string("ADD");
		cout << line << " --> " << line.find("+") << endl;
		size_t com_pos = line.find("+");
		vector<string> all_elements = string_split(line, ' ');
		
		bool all_alphanum = true;
		bool found_equals = false;
		
		for (int i=0; i < all_elements.size(); i++) {
			all_alphanum = true;
			
			for (int j=0; j<all_elements.at(i).length(); j++) {
				if (!isalnum(all_elements.at(i)[j])) {
					all_alphanum = false;
					if (all_elements.at(i)[j] == '=') {
						found_equals = true;
					}
				}
			}
			
			if (all_alphanum && !found_equals) {
				dataOutputs.push_back(all_elements.at(i));
				cout << "Found Output:  " << all_elements.at(i) << endl;
			}
			else if (all_alphanum) {
				dataInputs.push_back(all_elements.at(i));
				cout << "Found Input:  " << all_elements.at(i) << endl;
			}
		}
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

vector<string> Component::string_split(string line, char delim){
	vector<string> elems;
	stringstream line_stream(line);
    string item;
	
    while (getline(line_stream, item, delim)) {
		if (item.length() > 0) {
        elems.push_back(item);
		}
    }
	
	return elems;
}