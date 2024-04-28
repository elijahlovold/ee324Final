/************************************************************
* Module: mux_2x4                                     		*
* Description:  This functions as a generalized 2 to 4 mux. *
*               the width of the input and output can be    *
*               specified as a parameter.                   *
* Input parameters: width, select line, input			    *
* Returns: output of mux                                    *
* Usages: useful for selecting between various inputs		*
************************************************************/

module mux_2x4 #(parameter w = 1) (
    output reg [w - 1 : 0]y,
    input [1:0]s, input [4*w - 1 : 0]x
);

	always @(*)
		begin
			case (s)
				0: y <= x[1*w - 1 : 0];		// slice 1st output range
				1: y <= x[2*w - 1 : 1*w];	// slice 2nd output range
				2: y <= x[3*w - 1 : 2*w];	// slice 3rd output range
				3: y <= x[4*w - 1 : 3*w];	// slice 4th output range
				
				default: y <= 0; 	// default 0
			endcase
		end
endmodule