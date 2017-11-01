1)   Names and NetIDs of group members and course enrolled in

Jake Given - jakegiven ECE 574
Joseph Loubert - loubert ECE574
Giresse Tchegho - gtchegho ECE574
Richard Wilson - richardwilson ECE574

2) Brief description of program
	This program will take specifically formatted netlist.txt files and turn them into verilog files ready for synthesis.  It parses each line and determines if the line needs to be a variable declaration or module instantiation.  It keeps track of which inputs,outputs,wires are available so it can track if a wire is attempting to be used without actually being declared at the top.  As it builds the verilog file, it creates a graph that is used for the critical path analysis.  The critical path is calculated as the longest input to register or register to register time.

3) contributions

Jake Given - debugging, preliminary design
Joseph Loubert - lead programmer
Giresse Tchegho - Moral support
Richard Wilson - Git manager, Attempted execution of original plan,debugging