
// decide what should be displayed on the screen
module snake_gen(
    input [11:0] x, y,
    input clk,
    input [23:0] color0,
    input [23:0] color1,
    input [23:0] color2,
    input [23:0] color3,
    input [23:0] color4,
    input [23:0] color5,
    input [23:0] color6,
    input [23:0] color7,

    input [31:0] sprite_addr, // to choose sprite from this file
    output [7:0] current_tile,

    output snake_on, 
    output reg [23:0] snake_color
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
    assign snake_on = (spritePIX != 3'b000); // Transparency also handles "ON ROM state"

    always @(*) begin 
        case (spritePIX)
            0:  snake_color = color0; // transparent
            1:  snake_color = color1;
            2:  snake_color = color2;
            3:  snake_color = color3;
            4:  snake_color = color4;
            5:  snake_color = color5;
            6:  snake_color = color6;
            7:  snake_color = color7;
        endcase
    end




endmodule