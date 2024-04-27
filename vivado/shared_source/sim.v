`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/26/2024 12:26:21 PM
// Design Name: 
// Module Name: sim
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

module sim();

    wire HS, VS, Vde;
    wire [11:0] x_vga, y_vga;
    reg clk, rst;
    reg [31:0] sprite_addr;
    
    wire [7:0] R, G, B;
    wire [7:0] current_tile;
    // create the display using default parameters
    VGA display (HS, VS, Vde, x_vga, y_vga, clk, rst);
    pixel_gen_temp test(x_vga, y_vga, Vde, sprite_addr, clk, R, G, B, current_tile);

    // create the clock
    always begin
        #10
        clk = ~clk;
    end 

    initial begin
        sprite_addr = 32'hFFEEDDCC;
        clk = 0;    // initialize clk low
        rst = 0; 
        #1
        rst = 1;    // initialize rst high
        #1
        rst = 0;    // drive rst low
        
        // run the simulation for a long time...
        #100000000
        
        $finish;
    end
    

endmodule

