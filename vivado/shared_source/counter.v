/************************************************************
* Module: up_counter                                        *
* Description:  This module will count up until rst or      *
*               rollover.                                   *
* Input parameters: width, clk, rst                         *
* Returns: W bit counter                                    *
* Usages: useful for up counters                            *
************************************************************/

// simple up counter that will count until reset
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

/************************************************************
* Module: counter                                           *
* Description:  This module will count up while enabled     *
*               until rst or configured max number.         *
* Input parameters: max number & width, en, clk, rst        *
* Returns: W bit counter                                    *
* Usages: useful for generic counters                       *
************************************************************/

// counter that will count to a specific value
module counter #(parameter NUM = 1000, W = 32) (
    output reg [W - 1:0] counter,
    input en, clk, rst
);

    always @(posedge clk, posedge rst) begin // on the positive clk edge...
        if (rst) begin 
            counter <= 0;
        end else begin
            if (counter >= NUM) begin   // if elapsed...
                counter <= 0;           // ...reset
            end else if (en) begin      // else if enabled...
                counter <= counter + 1; // ...increment counter
            end
        end 
    end 
endmodule