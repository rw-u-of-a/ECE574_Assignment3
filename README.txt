1)   Names and NetIDs of group members and course enrolled in

Jake Given - jakegiven ECE574
Joseph Loubert - loubert ECE574
Giresse Tchegho - gtchegho ECE574
Richard Wilson - richardwilson ECE574

2) Brief description of program

This program takes a specially formatted high-level program along with a latency constraint, and turns it into a verilog file satisfying the latency constraint. It parses each line to first determine the inputs, outputs, and internal registers of the program. It then parses the code line-by-line, creating new branches corresponding in the case of an if statement, and otherwise adding a new component to the current state. In the event of a branch, the latency used to schedule the branch is the maximum of the two ASAP latencies of each individual branch, and this is deducted from the latency of the parent branch. After building the top level states, branches, and latencies, each branch is scheduling using the Force-Directed Scheduling algorithm. The schedule is then implemented in verilog, adding an initial Wait state 0, and a final Done state.

3) contributions

Jake Given - debugging, testing
Joseph Loubert - lead programmer
Giresse Tchegho - Moral support
Richard Wilson - FDS algorithm design, debugging
