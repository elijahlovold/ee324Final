/************************************************************
* Module: seven_decoder                                     *
* Description:  This decodes a number from 0-9 for a digit	*
*               of the 7-seg display.				    	*
* Input parameters: select line, number to display			*
* Returns: cathodes, anodes									*
* Usages: useful for writing a digit on the 7-seg			*
************************************************************/

module seven_decoder ( 
    output reg [7:0]seg_cat, [3:0]seg_an,
    input [1:0]sel, [7:0]number, [1:0]en
);

	assign seg_an = ~(4'b0001 << sel);  // select anode with sel line

	always @(*)
		begin
		  if (en == 1) begin
			case (number)      // .GFEDCBA
				0 : seg_cat <= 8'b11000000;     // G
				1 : seg_cat <= 8'b11111001;     // ~BC
				2 : seg_cat <= 8'b10100100;     // CF
				3 : seg_cat <= 8'b10110000;     // EF
				4 : seg_cat <= 8'b10011001;     // EDA
				5 : seg_cat <= 8'b10010010;     // BE
				6 : seg_cat <= 8'b10000010;     // B
				7 : seg_cat <= 8'b11111000;     // ~ABC
				8 : seg_cat <= 8'b10000000;     //
				9 : seg_cat <= 8'b10010000;     // E
				
				default : seg_cat <= 8'b11111111; // turn off if invalid
			endcase
		  end else if (en == 3) begin
		      seg_cat <= number;
		  end else begin
		      seg_cat <= 8'b11111111; // turn off if invalid
		  end
		  
		end
endmodule