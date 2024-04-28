/************************************************************
* Module: pwm                                               *
* Description:  This module creates a pwm generator using   *
*               an up counter.                              *
* Input parameters: clk, enable, period, width              *
* Returns: pwm signal                                       *
* Usages: useful for building simple pwm generator          *
************************************************************/

module pwm #(parameter W = 32) (
    output pwm,
    input clk, en,
    input [W - 1:0]period, [W - 1:0]width
    );

    wire [W - 1:0]up_count;     // track the up counter
    wire up_rst;                // rst signal
    
    up_counter #(W) UP_COUNTER (up_count, clk, up_rst);  // instantiate up counter...

    assign pwm = ((up_count < width ? 1 : 0) & en); // if width elapsed or en is low, turn pwm off
    assign up_rst = (up_count > period ? 1 : 0);    // if period elapsed, rst
endmodule