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
    input [15:0] x, y, input vde,
    input [31:0] sprite_addr,
    output reg [7:0] R, G, B,
    output [8:0] current_tile
    );

    localparam TILES_PER_REG = 4;               // number of tiles per reg
    localparam SPRITE_SIZE = 40;                // size of the sprite in pixels
    localparam N_PER_ROW = WIDTH/(SPRITE_SIZE*TILES_PER_REG);   // number of tiles per row

    wire [15:0] offset_x, offset_y;
    // if in drawable region, offset to local coords
    assign offset_x = (vde ? (x - H_SYNC_TIME + H_B_PORCH + H_LR_BORDER): 0); 
    assign offset_y = (vde ? (y - V_SYNC_TIME + V_B_PORCH + V_LR_BORDER): 0); 

    // next, flatten 2d coords to current_tile
    // how does the division round?...
    assign current_tile = (offset_x/(SPRITE_SIZE*TILES_PER_REG) + N_PER_ROW*(offset_y/(SPRITE_SIZE)));

    // output block
    always @(*) begin 
        if (~vde) begin
            {R, G, B} <= 24'h000000;                // nothing
        end 
        else begin 
            if (vde) begin         // btn0
                {R, G, B} <= 'hFFFF00; 
            end
            else begin                   
                {R, G, B} <= 0;             // background  
            end 
        end
    end
endmodule