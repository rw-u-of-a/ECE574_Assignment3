
#include "Component.hpp"
#include "Operator.hpp"

Operator::Operator(string line){
	
	valid_operator = false;
	
	vector<string> all_elements;
	size_t op_pos;
	
    if(line.find("DEC")!=string::npos){
        //DEC
		// Not sure if this has it's own operator
		// or if we need to find 'x - 1'
    }

    else if(line.find("+")!=string::npos){
        //ADD
		type = string("ADD");
		cout << line << " --> " << line.find("+") << endl;
		op_pos = line.find("+");
		all_elements = string_split(line, ' ');
		
		valid_operator = true;
		
		input_word.push_back("a");
		input_word.push_back("b");
		output_word.push_back("c");
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
    else{
        //not a valid operator
    }
	
	if (valid_operator){
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
				op_dataOutputs.push_back(all_elements.at(i));
				cout << "Found Output:  " << all_elements.at(i) << endl;
			}
			else if (all_alphanum) {
				op_dataInputs.push_back(all_elements.at(i));
				cout << "Found Input:  " << all_elements.at(i) << endl;
			}
		}
	}
	
}

vector<string> Operator::string_split(string line, char delim){
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

void Operator::put_operator(Operator& op, ofstream& outfile){
	int temp_datawidth = 8;
	cout << "outfile" << endl;
	outfile << op.type << " #(.DATAWIDTH(" << temp_datawidth << ")) " << 
		op.type << "_" << 1 << 
		"( ." << op.input_word[0] << "(" << op.op_dataInputs[0] << 
		"), ." << op.input_word[1] << "(" << op.op_dataInputs[1] << 
		"), ." << op.output_word[0] << "(" << op.op_dataOutputs[0] <<
		"));\n";
}