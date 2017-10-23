// Assignment2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void put_operator(int , string , int, ofstream&);

enum operator_enum {SUB, ADD};
static string operator_string[] = { "-", "+" };
static string operator_title[] = { "SUB", "ADD" };
static string operator_output[] = { "diff", "sum" };

int main()
{

	string line;
	string space_delim = " ";
	string line_end = "\n";
	string word;
	size_t pos = 0;
	ifstream infile("circuits/474a_circuit1.txt");
	int add_counter = 0;
	int sub_counter = 0;

	ofstream outfile;
	outfile.open("v_circuit1.v");

	if (infile.is_open())
	{
		while (getline(infile, line))
		{
			cout << line << '\n';
			string this_line = line;

			/*while (((pos = line.find(space_delim)) != std::string::npos))
			{
				word = line.substr(0, line.find(space_delim));
				cout << word << pos << '\n';
				line.erase(0, pos + space_delim.length());			
			}
			word = line.substr(0, line.find(line_end));
			cout << word << '\n';
			*/

			size_t equals_pos = this_line.find("=");
			size_t add_pos = this_line.find("+");
			size_t sub_pos = this_line.find("-");
			size_t end_of_line = this_line.find("\n");

			if ((equals_pos != string::npos) & (add_pos != string::npos))
			{
				add_counter = add_counter + 1;

				put_operator(ADD, this_line, add_counter, outfile);

			}

			if ((equals_pos != string::npos) & (sub_pos != string::npos))
			{
				sub_counter = sub_counter + 1;

				put_operator(SUB, this_line, sub_counter, outfile);

			}

		}
		infile.close();
	}

	outfile.close();
	infile.close();
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