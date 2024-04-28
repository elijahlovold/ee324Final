/************************************************************
* Module: counter_2_dig                                     *
* Description:  This module is used to count from 0 to 3.   *
* Input parameters: clock                                   *
* Returns: 2 bit counter                                    *
* Usages: useful for counting 0-3                           *
************************************************************/

module bit_counter_2 (
    output reg [1:0] counter,
    input clk, rst
);

    always @(posedge clk, negedge rst) begin      // on the positive clk edge...
        if (!rst) begin     // active low...
            counter <= 0;
        end else begin 
            counter <= counter + 1;     // increment counter, will roll over automatically
        end
    end
endmodule