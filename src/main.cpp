// Assignment2.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include "stdlib.h"
#include "Driver.hpp"


using namespace std;

void put_operator(int , string , int, ofstream&);

enum operator_enum {SUB, ADD};
static string operator_string[] = { "-", "+" };
static string operator_title[] = { "SUB", "ADD" };
static string operator_output[] = { "diff", "sum" };

int main(int argc, char ** argv)
{
    string temp,temp2;
    if(argc != 3){
        cout <<"Incorrect number of arguments"<<endl;
        return 0;
    }
    temp = string("../src/circuits/")+string(argv[2]);
	int PATH_MAX = 100;
    char path[PATH_MAX];
    char path2[PATH_MAX];
    realpath(temp.c_str(),path);//don't need absolute path on linux but osx is weird about it
    temp = string(path);
    realpath((string("../src/circuits/")+string(argv[1])).c_str(),path2);
    temp2 = string(path2);
    Driver Baby;
    Baby.Run(temp2,temp);
//    ifstream infile(path);
//    if(!infile.is_open()){
//        cout <<"Can't open circuit file "<< circuitFilename <<endl;
//        return 0;
//    }
//
//	string line;
//	string space_delim = " ";
//	string line_end = "\n";
//	string word;
//	size_t pos = 0;
//	int add_counter = 0;
//	int sub_counter = 0;
//
//	ofstream outfile;

//    
//	outfile.open(path2);
//    if(outfile.is_open()){
//        cout << "Could not open netlist file" <<netlistFilename<< endl;
//        return 0;
//    }
//
//	if (infile.is_open())
//	{
//		while (getline(infile, line))
//		{
//            
//			cout << line;
//            string this_line = line;
//            if(line.find("input") != string::npos){
//                cout << "Handle input"<<endl;
//                //handleInput(line);
//            }
//            else if(line.find("output")!= string::npos){
//                cout << "Handle output" <<endl;
//                //handleOutput(line);
//            }
//            else if(line.find("wire")!=string::npos&&line.find("wire",1)==string::npos){
//                //handleWire(line);
//                cout << "Handle wire" <<endl;
//            }
//            else {
//                //handleComponent(line);
//                cout << "Handle component" <<endl;
//            }
//
//			size_t equals_pos = this_line.find("=");
//			size_t add_pos = this_line.find("+");
//			size_t sub_pos = this_line.find("-");
//			size_t end_of_line = this_line.find("\n");
//
//			if ((equals_pos != string::npos) & (add_pos != string::npos))
//			{
//				add_counter = add_counter + 1;
//
//				put_operator(ADD, this_line, add_counter, outfile);
//
//			}
//
//			if ((equals_pos != string::npos) & (sub_pos != string::npos))
//			{
//				sub_counter = sub_counter + 1;
//
//				put_operator(SUB, this_line, sub_counter, outfile);
//
//			}
//
//		}
//		infile.close();
//	}
//
//	outfile.close();
//	infile.close();
    return 0;



}

void put_operator(int type, string line, int type_num, ofstream& outfile)
{

	size_t equals_pos = line.find("=");
	size_t type_pos = line.find(operator_string[operator_enum(type)]);

	string space_delim = " ";

	string output_word = line.substr(0, equals_pos - space_delim.length());
	string input1_word = line.substr(equals_pos + 2 * space_delim.length(),
		type_pos - output_word.length() - 3 * space_delim.length());
	string input2_word = line.substr(type_pos + 2 * space_delim.length(),
		line.length() - space_delim.length());

	cout << "equals_pos = " << equals_pos << '\n';
	cout << "sub_pos = " << type_pos << '\n';

	outfile << operator_title[operator_enum(type)] << " #(.DATAWIDTH(8)) " << 
		operator_title[operator_enum(type)] << "_" << to_string(type_num) << 
		"( .a(" << input1_word << "), .b(" << input2_word << "), ." <<
		operator_output[operator_enum(type)] << "(" <<
		output_word << "));\n";

}
