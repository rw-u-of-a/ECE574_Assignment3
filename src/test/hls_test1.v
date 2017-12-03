module HLSM (Clk, Rst, Start, Done);
	input Clk, Rst, Start;
	output reg Done;
	
	input [15:0] a, b, c;
	
	output [7:0] z;
	output [15:0] x;

	wire [7:0] UInt8 d, e, f, g 	
	
	always @(posedge Clk) begin
		if (Start)
			state = state_1;
		else
			case(state)
			state_1:ADD #(.DATAWIDTH(16)) ADD_1(.a(a), .b(b), .sum(d));
					ADD #(.DATAWIDTH(16)) ADD_2(.a(a), .b(c), .sum(e));
					COMP #(.DATAWIDTH(16)) COMP_1(.a(d), .b(e), .gt(g));
					MUX2x1 #(.DATAWIDTH(8)) MUX2x1_1(.a(g), .b(d), .sel(e), .d(z));
					MUL #(.DATAWIDTH(16)) MUL_1(.a(a), .b(c), .prod(f));
					SUB #(.DATAWIDTH(16)) SUB_1(.a(f), .b(d), .diff(xwire));
					state = wait_state;
			default:
		end
	end
	
endmodule