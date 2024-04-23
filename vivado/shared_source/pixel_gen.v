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
    input [7:0] sprite_addr, // to choose sprite from this file
    input clk,
    output reg [7:0] R, G, B,
    output [8:0] current_tile
    );

    localparam TILES_PER_REG = 4;               // number of tiles per reg
    localparam SPRITE_SIZE = 40;                // size of the sprite in pixels
    localparam N_PER_ROW = WIDTH/(SPRITE_SIZE*TILES_PER_REG);   // number of tiles per row

    wire [11:0] offset_x, offset_y;
    // if in drawable region, offset to local coords
    assign offset_x = (vde ? (x - (H_SYNC_TIME + H_B_PORCH + H_LR_BORDER)): 0); 
    assign offset_y = (vde ? (y - (V_SYNC_TIME + V_B_PORCH + V_LR_BORDER)): 0); 

    // next, flatten 2d coords to current_tile
    assign current_tile = (offset_x/(SPRITE_SIZE*TILES_PER_REG) + N_PER_ROW*(offset_y/(SPRITE_SIZE)));
   
                                                                       
    // Now we need to specify the sprite data.                            
    wire[119:0] spriteDATA; // The total number of columns of data        
    wire[2:0] spritePIX;                                                  
    wire[11:0] spriteROW; //                                              
    wire[11:0] spriteCOL;                                                 
    wire spritePIX_on;                                                    

                
    reg [5:0] mod_by_40_lut[0:2047];
   
    integer i;
    initial begin
        for (i = 0; i <= 2047; i = i + 1) begin
            mod_by_40_lut[i] = i % 40;
        end
    end
    
    
    wire [11:0] offset = (sprite_addr << 5) + (sprite_addr << 3);     // *40
//    wire [11:0] offset = 40*sprite_addr[3:0];
    assign spriteCOL = mod_by_40_lut[offset_x];
    assign spriteROW = mod_by_40_lut[offset_y] + offset;
    
    // Pull Sprite from .COE File
    blk_mem_gen_0 sprite(clk, spriteROW, spriteDATA);
    assign spritePIX = spriteDATA[((39 - spriteCOL)*3+2) -:3];
    assign spritePIX_on = (spritePIX != 3'b000); // Transparency also handles "ON ROM state"

    // output block
    always @(*) begin 
        if (~vde) begin
            {R, G, B} <= 24'h000000;
        end 
        else begin
            if(spritePIX_on) begin
                case (spritePIX)
                  0:  {R,G,B} = 24'h000000; // transparent
                  1:  {R,G,B} = 24'h0000FF; // body outline
                  2:  {R,G,B} = 24'h00FF00; // body color
                  3:  {R,G,B} = 24'h000000; // eye color
                  4:  {R,G,B} = 24'hFF0000; // spine color
                  5:  {R,G,B} = 24'h000000;
                  6:  {R,G,B} = 24'h000000;
                  7:  {R,G,B} = 24'h000000;
                  default: {R,G,B} = 24'h000000;
                endcase
            end
            else begin                   
                {R, G, B} <= 24'hFFFFFF;             // background  
            end 
        end
    end
endmodule
