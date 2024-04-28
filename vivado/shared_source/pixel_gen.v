/************************************************************
* Module: pixel_gen                                         *
* Description:  This module generates 4 quarter-circle      *
*               btn icons for the Simon game.               *
* Input parameters: x, y, vde, btn en, bck color,           *
*                   btn colors (0-3)                        *
* Returns: rgb value of current pixel                       *
* Usages: useful for Simon game                             *
************************************************************/

// decide what should be displayed on the screen
module pixel_gen_temp #(parameter  WIDTH=1920, HEIGHT=1080, 
                        H_SYNC_TIME = 44, V_SYNC_TIME = 5,
                        H_F_PORCH = 88, V_F_PORCH = 4,
                        H_B_PORCH = 148, V_B_PORCH = 36,
                        H_LR_BORDER = 0, V_LR_BORDER = 0
                        ) (
    input [11:0] x, y, input vde,
    input [31:0] sprite_addr, // to choose sprite from this file
    input clk,
    input [23:0] color0,
    input [23:0] color1,
    input [23:0] color2,
    input [23:0] color3,
    input [23:0] color4,
    input [23:0] color5,
    input [23:0] color6,
    input [23:0] color7,

    output reg [7:0] R, G, B,
    output [7:0] current_tile   // NOTE, UPDATE TO 8:0 ONCE GRID CONTROLLER FIXED!!!!
    );


    // localparam TILES_PER_REG = 4;               // number of tiles per reg
    // localparam SPRITE_SIZE = 32;                // size of the sprite in pixels
    // localparam N_PER_COL = 33;  // actually 33.75, account for later...
    localparam N_PER_ROW = 60;

    wire [12:0] offset_x, offset_y;
    // if in drawable region, offset to local coords
    assign offset_x = x - 192; // (H_SYNC_TIME + H_B_PORCH + H_LR_BORDER) 
    assign offset_y = y - 41; // (V_SYNC_TIME + V_B_PORCH + V_LR_BORDER)

    // next, flatten 2d coords to current_tile
    wire [5:0] x_comp;
    wire [10:0] y_comp;
    assign x_comp = offset_x >> 5;
    assign y_comp = 60*(offset_y >> 5); // * N_PER_ROW
    assign current_tile = (x_comp + y_comp) >> 3;
                                                                       
    // Now we need to specify the sprite data.                            
    wire[95:0] spriteDATA; // The total number of columns of data        
    wire[2:0] spritePIX;                                                  
    wire[11:0] spriteROW; //                                              
    wire[11:0] spriteCOL;                                                 
    wire spritePIX_on;
    
    wire [4:0] sprite_val;
    assign sprite_val = offset_x[7:5] << 2;
    
    wire [3:0] sprite_fin;
    assign sprite_fin = sprite_addr[sprite_val +: 4];
     
    assign spriteCOL = offset_x[4:0];   // mod 32
    assign spriteROW = {sprite_fin, offset_y[4:0]};   // upper bits define offset
    
    // Pull Sprite from .COE File
    blk_mem_gen_2 sprite(clk, spriteROW, spriteDATA);
    assign spritePIX = spriteDATA[((31 - spriteCOL)*3+2) -: 3];
    
    // add in future, y_offset < 1080 - 12
    assign spritePIX_on = (spritePIX != 3'b000); // Transparency also handles "ON ROM state"

    // output block
    always @(*) begin 
        if (~vde) begin
            {R, G, B} = 24'h000000;
        end 
        else begin
            if(spritePIX_on) begin
                case (spritePIX)
                  0:  {R,G,B} = color0; // transparent
                  1:  {R,G,B} = color1;
                  2:  {R,G,B} = color2;
                  3:  {R,G,B} = color3;
                  4:  {R,G,B} = color4;
                  5:  {R,G,B} = color5;
                  6:  {R,G,B} = color6;
                  7:  {R,G,B} = color7;
                endcase
            end
            else begin                   
                {R, G, B} = 24'hFFFFFF;             // background  
            end 
        end
    end
endmodule