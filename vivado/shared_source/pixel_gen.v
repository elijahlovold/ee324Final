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
    input clk,
    output reg [7:0] R, G, B,
    output [8:0] current_tile
    );

    localparam TILES_PER_REG = 4;               // number of tiles per reg
    localparam SPRITE_SIZE = 40;                // size of the sprite in pixels (Changes directly effect sprites right now)
    localparam N_PER_ROW = WIDTH/(SPRITE_SIZE*TILES_PER_REG);   // number of tiles per row

    wire [15:0] offset_x, offset_y;
    // if in drawable region, offset to local coords
    assign offset_x = (vde ? (x - H_SYNC_TIME + H_B_PORCH + H_LR_BORDER): 0); 
    assign offset_y = (vde ? (y - V_SYNC_TIME + V_B_PORCH + V_LR_BORDER): 0); 

    // next, flatten 2d coords to current_tile
    // how does the division round?...
    assign current_tile = (offset_x/(SPRITE_SIZE*TILES_PER_REG) + N_PER_ROW*(offset_y/(SPRITE_SIZE)));




    // Oh yeah its sprite makin time :0
    localparam spriteLE = 1000; // These will be changed to hardware inputs? Or the math of the Current Tile?
    localparam spriteTE = 600;
    localparam spriteRE = spriteLE + SPRITE_SIZE - 1;
    localparam spriteBE = spriteTE + SPRITE_SIZE - 1;
    wire spriteON;
    assign spriteON = ((x >= spriteLE && x <= spriteRE) && (y >= spriteTE && y <= spriteBE)); // change to offset_x offset_y when sprite is working
    assign x_adj = x - spriteLE;
    assign y_adj = y - spriteTE;
    
    // Now we need to specify the sprite data.
    wire[119:0] spriteDATA; // The total number of columns of data
    wire[1:0] spritePIX;
    wire[5:0] spriteROW; //
    wire[5:0] spriteCOL;
    wire spritePIX_on;
    assign spriteCOL = x - spriteLE;
    assign spriteROW = y - spriteTE;
    // Pull Sprite from .COE File
    blk_mem_gen_0 sprite(clk, spriteROW, spriteDATA);
    assign spritePIX = spriteDATA[((~spriteCOL)*3+2)-:3];
    assign spritePIX_on = (spritePIX != 3'b000) & spriteON;


    // output block
    always @(*) begin 
        if (~vde) begin
            {R, G, B} <= 24'h000000;                // nothing
        end 
        else begin 
            if(spritePIX_on) begin
                case (spritePIX)
                  'b000:  {R,G,B} = 'h000000;
                  'b001:  {R,G,B} = 'h0000FF;
                  'b010:  {R,G,B} = 'h22B14C;
                  'b011:  {R,G,B} = 'h0A0A0A;
                  'b100:  {R,G,B} = 'hFF0000;
                  'b101:  {R,G,B} = 'h000000;
                  'b110:  {R,G,B} = 'h000000;
                  'b111:  {R,G,B} = 'h000000;
                  default: {R,G,B} = 'hFFFFFF;
                endcase
            end        
            else begin           
                {R, G, B} <= 24'hFFFF00;             // background  
            end            
        end
    end
endmodule