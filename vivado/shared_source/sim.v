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
    snake_gen test(x_vga, y_vga, clk, 'h0, 'hf, 'hf0, 'hff, 'hf00, 'hf0f, 'hff0, 'hfff, sprite_addr, clk, R, G, B, tile_x, tile_y);
    grid_controller_32_2D_v1_0 test();
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

