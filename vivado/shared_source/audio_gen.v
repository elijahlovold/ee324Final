/************************************************************
* Module: sine_controller                                   *
* Description: This module creates a sine wave from a ROM   *
*              block and PWM generator. Can configure       *  
*              freq and set en.                             *
* Input parameters: clk, rst, en, frequency                 *
* Returns: audio pwm wave                                   *
* Usages: useful for generating tones                       *
************************************************************/

module sine_controller (
    input clk, rst, en, [1:0]freq, 
    output audio
    );

    reg [11:0] value;       // controls the frequency of sine wave
    wire [7:0] duty_cycle;  // duty cycle of pwm generator 
    wire [9:0] sine_addr;   // sine wave addr for ROM 

    // instantiate sine controller and ROM
    synth_controller SINE_CONTROLLER(clk, rst, value, sine_addr);
    blk_mem_gen_0 SINEWAVE_ROM(clk, sine_addr, duty_cycle);

    // pwm block to pipe out to audio pin
    wire raw_audio;
    PWM pwm1(clk, rst, duty_cycle, raw_audio);
    assign audio = raw_audio & en;  // toggle with enable

    // fnote = fclk/[(number of samples)*(value)]
    // -> value = fclk/[(number of samples)*fnote]
    // -> value = 100Mhz/[(1024)*fnote]
    always @(*) begin
        case (freq)
            0: value <= 163;    // freq = 600Hz -> value = 163 	
            1: value <= 122;	// freq = 800Hz -> value = 122
            2: value <= 81;	    // freq = 1200Hz -> value = 81
            3: value <= 61;	    // freq = 1600Hz -> value = 61
        endcase
    end
endmodule