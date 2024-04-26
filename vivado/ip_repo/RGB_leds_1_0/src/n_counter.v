/************************************************************
* Module: counter_2_dig                                     *
* Description:  This module is used to up until rst or      *
*               rollover.                                   *
* Input parameters: clock, rst                              *
* Returns: W bit counter                                    *
* Usages: useful for up counters                            *
************************************************************/

module up_counter #(parameter W = 32) (
    output reg [W - 1:0] counter,  // use max size counter
    input clk, rst
);

    always @(posedge clk, posedge rst) begin // on the positive clk edge...
        if (rst) begin 
            counter <= 0;
        end else begin
            counter <= counter + 1;     // increment counter
        end 
    end 
endmodule