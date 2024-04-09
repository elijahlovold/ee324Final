/************************************************************
* Module: PWM                                               *
* Description:  This module creates a simple PWM signal.    *
* Input parameters: clk, rst, duty cycle                    *
* Returns: pwm signal                                       *
* Usages: useful for generating pwm signals                 *
************************************************************/

module PWM ( 
    input clk, rst, input [7:0] dutycycle, 
    output pwm_out 
    );

    reg [7:0] width; // store my duty cycle temporarily
    reg [7:0] count; // counter value that varies from 0 - 255
    
    // This block create a variable width duty cycle PWM signal out based on an input duty cycle clk
    // pwm_out will be a square wave with duty cycle = dutycycle/255
    always @ (posedge clk, posedge rst) begin
        if (rst) begin
            count <= 0;
            width <= 0;
        end
        else begin
            count <= count + 1;
            if (count == 0) begin
                width <= dutycycle;
            end
            if (width > 0) begin
                width <= width - 1;            
            end 
        end
    end    
    
    // assign pwm signal based on width
    assign pwm_out = width ? 1'b1 : 1'b0;
    
endmodule
