
#include "Component.hpp"
#include "Operator.hpp"

Operator::Operator(string line){
	
	valid_operator = false;
	
	vector<string> all_elements = string_split(line, ' ');
	size_t op_pos;
	
	cout << line << endl;
	
    if(line.find("DEC")!=string::npos){
        //DEC
		// Not sure if this has it's own operator
		// or if we need to find 'x - 1'
    }

    else if(line.find("+")!=string::npos){
        //ADD
		type = string("ADD");
		//cout << line << " --> " << line.find("+") << endl;
		op_pos = line.find("+");
		
		valid_operator = true;
		
		input_word.push_back("a");
		input_word.push_back("b");
		output_word.push_back("sum");
    }
    else if(line.find("-")!=string::npos){
        //SUB
		type = string("SUB");
		op_pos = line.find("-");
		
		valid_operator = true;
		
		input_word.push_back("a");
		input_word.push_back("b");
		output_word.push_back("diff");
    }
    else if(line.find("*")!=string::npos) {
        //MULT
		type = string("MULT");
		op_pos = line.find("*");
		
		valid_operator = true;
		
		input_word.push_back("a");
		input_word.push_back("b");
		output_word.push_back("prod");
    }
    else if(line.find("?")!=string::npos &&
			line.find(":")!=string::npos){
		//MUX2x1
		type = string("MUX2x1");
		
		valid_operator = true;
		
		input_word.push_back("a");
		input_word.push_back("b");
		output_word.push_back("d");
		control_input_word.push_back("sel");
    }
    else if(line.find_first_of(">")!=line.find_last_of(">")){
        //SHR
		type = string("SHR");
		//op_pos = line.find(">>");
		
		
		cout << "Found SHR" << endl;
		
		valid_operator = true;
		
		input_word.push_back("a");
		control_input_word.push_back("sh_amt");
		output_word.push_back("d");
    }
    else if(line.find_first_of("<")!=line.find_last_of("<")){
        //SHL
		type = string("SHL");
		//op_pos = line.find("<<");
		
		cout << "Found SHL" << endl;
		
		valid_operator = true;
		
		input_word.push_back("a");
		control_input_word.push_back("sh_amt");
		output_word.push_back("d");
    }
    else if(line.find("/")!=string::npos){
        //DIV
		type = string("DIV");
		op_pos = line.find("/");
		
		valid_operator = true;
		
		input_word.push_back("a");
		input_word.push_back("b");
		output_word.push_back("quot");
    }
    else if(line.find("%")!=string::npos){
        //MOD
		type = string("MOD");
		op_pos = line.find("%");
		
		valid_operator = true;
		
		input_word.push_back("a");
		input_word.push_back("b");
		output_word.push_back("rem");
    }    
	else if(line.find(">")!=string::npos ||
			line.find("<")!=string::npos ||
			line.find("==")!=string::npos){
        //COMP
    }
    else if(line.find("INC")!=string::npos){
        //INC
		// Not sure if this has it's own operator
		// or if we need to find 'x + 1'
    }
    else{
        //not a valid operator
    }
	
	cout << type << endl;
	
	if (valid_operator){
		bool all_alphanum = true;
		bool found_equals = false;
		bool found_questionmark = false;
		bool found_shift = false;
		
		for (int i=0; i < all_elements.size(); i++) {
			all_alphanum = true;
			
			for (int j=0; j<all_elements.at(i).length(); j++) {
				if (!isalnum(all_elements.at(i)[j])) {
					all_alphanum = false;
					if (all_elements.at(i)[j] == '=') {
						found_equals = true;
					}
					if (all_elements.at(i)[j] == '?') {
						found_questionmark = true;
					}
					if (all_elements.at(i)[j] == '<' ||
						all_elements.at(i)[j] == '>'){
						found_shift = true;
					}
				}
			}
			
			if (all_alphanum && !found_equals) {
				op_dataOutputs.push_back(all_elements.at(i));
				cout << "Found Data Output:  " << all_elements.at(i) << endl;
			}
			else if (all_alphanum && type == string("MUX2x1") && !found_questionmark){
				op_controlInputs.push_back(all_elements.at(i));
				cout << "Found Control Input:  " << all_elements.at(i) << endl;
			}
			else if (all_alphanum && (type == string("SHR") || type == string("SHL")) 
				&& found_shift){
				op_controlInputs.push_back(all_elements.at(i));
				cout << "Found Control Input:  " << all_elements.at(i) << endl;
			}
			else if (all_alphanum) {
				op_dataInputs.push_back(all_elements.at(i));
				cout << "Found Data Input:  " << all_elements.at(i) << endl;
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
		op.type << "_" << 1 << "(";
		
	for (int i=0; i < op.input_word.size(); i++){
		outfile << " ." << op.input_word[i] << "(" << op.op_dataInputs[i] << "), ";
	}
	for (int i=0; i < op.control_input_word.size(); i++){
		outfile << " ." << op.control_input_word[i] << "(" << op.op_controlInputs[i] << "),";
	}
	for (int i=0; i < op.output_word.size(); i++){
		outfile << " ." << op.output_word[i] << "(" << op.op_dataOutputs[i] << "),";
	}
	for (int i=0; i < op.control_output_word.size(); i++){
		outfile << " ." << op.control_output_word[i] << "(" << op.op_controlOutputs[i] << "),";
	}
		
	outfile << ");\n";
}