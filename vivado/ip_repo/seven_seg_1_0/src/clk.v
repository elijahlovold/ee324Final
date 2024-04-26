/************************************************************
* Module: clk_div                                           *
* Description:  This module is used to divide the raw       *
*               100MHz clk to 1kHz. This is accomplished by *
*               incrementing a counter to the correct ratio *
*               and flipping the output clk every time it's *
*               reached.                                    *
* Input parameters: target frequency, clock, reset signal   *
* Returns: divided clock signal                             *
* Usages: helpful for generating slower clocks              *
************************************************************/

module clk_div #(parameter freq = 1000) (
    output reg div_clk,
    input clk, rst
);

    reg [15:0] counter = 0;     // local register for counter
    
    always @(posedge clk, negedge rst) begin
        if (!rst) begin     // active low...
            counter = 0;
        end else begin
            // Clock Divider: 100MHz -> freq (100ms)
            // period = (100MHz / freq) / 2 = 50MHz/freq
            counter <= counter + 1;     // increment counter 
            if (counter >= 50000000/freq - 1) begin    // wait for period to elapse...
                div_clk <= ~div_clk;            // flip divided clk
                counter <= 0;                   // reset counter
            end 
        end
    end
endmodule