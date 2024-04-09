/************************************************************
* Module: synth_controller                                  *
* Description:  ROM controller for generating audio signals *
* Input parameters: clk, rst, value                         *
* Returns: address of ROM table                             *
* Usages: useful for generating audio signals               *
************************************************************/

module synth_controller ( 
    input clk, rst, input [11:0] value, 
    output reg [9:0] addr 
    );

// f_note = f_clk / (value * number of samples)
// f_note = 100MHz / (value * 1024)
// value = 100MHz / (f_note * 1024)

reg [11:0] count;

always @ (posedge clk, posedge rst) begin
    if (rst) begin
        count <= 0;
        addr <= 0;
    end
    else begin
        count <= count + 1;
        // incrementing the counter UNTIL we hit our value,
        // then move to our next addr of our sine wave.
        if (count >= value) begin
            addr <= addr + 1;
            count <= 0;
        end
    end
end

endmodule
