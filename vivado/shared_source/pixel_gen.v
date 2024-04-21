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
    input [31:0] sprite_addr, // to choose sprite from this file
    input clk,
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

    // Creating our new X and Y variables
    assign x_grid = x / 40;
    assign y_grid = y / 40;
    assign x_pos = x * 40;
    assign y_pos = y * 40;
    
    wire[15:0] spriteLE = x_grid*40;
    wire[15:0] spriteTE = y_grid*40;
    wire[15:0] spriteRE = spriteLE + SPRITE_SIZE - 1;
    wire[15:0] spriteBE = spriteTE + SPRITE_SIZE - 1;
    wire spriteON;
    assign spriteON = ((x_pos >= spriteLE && x_pos <= spriteRE) && (y_pos >= spriteTE && y_pos <= spriteBE));
    wire[15:0] x_adj;
    wire[15:0] y_adj;
    assign x_adj = x - spriteLE;
    assign y_adj = y + sprite_addr*40 - spriteTE;
    
    // Now we need to specify the sprite data.
    wire[119:0] spriteDATA; // The total number of columns of data
    wire[1:0] spritePIX;
    wire[15:0] spriteROW; //
    wire[5:0] spriteCOL;
    wire spritePIX_on;
    assign spriteCOL = x_adj[5:0];
    assign spriteROW = y_adj[15:0];
    // Pull Sprite from .COE File
    blk_mem_gen_0 sprite(clk, spriteROW, spriteDATA);
    assign spritePIX = spriteDATA[((39-spriteCOL)*3+2)-:3];
    assign spritePIX_on = (spritePIX != 3'b000) & spriteON; // Transparency also handles "ON ROM state"


    // output block
    always @(*) begin 
        if (~vde) begin
            {R, G, B} <= 24'h000000;
        end 
        else begin
            if(spritePIX_on) begin
                case (spritePIX)
                  3'b000:  {R,G,B} = 24'h000000; // transparent
                  3'b001:  {R,G,B} = 24'h0000FF; // body outline
                  3'b010:  {R,G,B} = 24'h00FF00; // body color
                  3'b011:  {R,G,B} = 24'h000000; // eye color
                  3'b100:  {R,G,B} = 24'hFF0000; // spine color
                  3'b101:  {R,G,B} = 24'h000000;
                  3'b110:  {R,G,B} = 24'h000000;
                  3'b111:  {R,G,B} = 24'h000000;
                  default: {R,G,B} = 24'h000000;
                endcase
            end
            else begin                   
                {R, G, B} <= 24'hFFFFFF;             // background  
            end 
        end
    end
endmodule