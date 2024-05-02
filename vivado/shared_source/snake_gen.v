
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
    output [5:0] tile_x, tile_y,

    output snake_on, 
    output reg [23:0] snake_color
    );

    // wire [11:0] offset_x, offset_y;
    // assign offset_x = x - 192; // (H_SYNC_TIME + H_B_PORCH + H_LR_BORDER) 
    // assign offset_y = y - 41;  // (V_SYNC_TIME + V_B_PORCH + V_LR_BORDER)

    assign tile_x = x >> 5;
    assign tile_y = y >> 5;   
                     
    // Now we need to specify the sprite data.                            
    wire [95:0] spriteDATA; // The total number of columns of data        
    reg [2:0] spritePIX;                                                  
    wire [8:0] spriteROW; //                                              
    wire [4:0] spriteCOL;                                                 
    wire spritePIX_on;
    
    // wire [4:0] sprite_val;
    // assign sprite_val = offset_x[7:5] << 2;
    
    // wire [3:0] sprite_fin;
    // assign sprite_fin = sprite_addr[sprite_val +: 4];

    // faster slicing
    reg [3:0] sprite_fin;
    always @(*) begin
        case(x[7:5])
            0: sprite_fin = sprite_addr[3:0];
            1: sprite_fin = sprite_addr[7:4];
            2: sprite_fin = sprite_addr[11:8];
            3: sprite_fin = sprite_addr[15:12];
            4: sprite_fin = sprite_addr[19:16];
            5: sprite_fin = sprite_addr[23:20];
            6: sprite_fin = sprite_addr[27:24];
            7: sprite_fin = sprite_addr[31:28];
        endcase
    end
     
    assign spriteCOL = x[4:0];   // mod 32
    assign spriteROW = {sprite_fin, y[4:0]};   // upper bits define offset
    
    // pull Sprite from .COE File
    blk_mem_gen_2 sprite(clk, spriteROW, spriteDATA);

    // wire [4:0] flipped = ~x[4:0];
    // assign spritePIX = spriteDATA[((flipped << 1) + flipped + 2) -: 3];

    // faster than mulitply by 3 and slicing...
   always @(*) begin
        case(x[4:0])
            0: spritePIX = spriteDATA[95:93];
            1: spritePIX = spriteDATA[92:90];
            2: spritePIX = spriteDATA[89:87];
            3: spritePIX = spriteDATA[86:84];
            4: spritePIX = spriteDATA[83:81];
            5: spritePIX = spriteDATA[80:78];
            6: spritePIX = spriteDATA[77:75];
            7: spritePIX = spriteDATA[74:72];
            8: spritePIX = spriteDATA[71:69];
            9: spritePIX = spriteDATA[68:66];
            10: spritePIX = spriteDATA[65:63];
            11: spritePIX = spriteDATA[62:60];
            12: spritePIX = spriteDATA[59:57];
            13: spritePIX = spriteDATA[56:54];
            14: spritePIX = spriteDATA[53:51];
            15: spritePIX = spriteDATA[50:48];
            16: spritePIX = spriteDATA[47:45];
            17: spritePIX = spriteDATA[44:42];
            18: spritePIX = spriteDATA[41:39];
            19: spritePIX = spriteDATA[38:36];
            20: spritePIX = spriteDATA[35:33];
            21: spritePIX = spriteDATA[32:30];
            22: spritePIX = spriteDATA[29:27];
            23: spritePIX = spriteDATA[26:24];
            24: spritePIX = spriteDATA[23:21];
            25: spritePIX = spriteDATA[20:18];
            26: spritePIX = spriteDATA[17:15];
            27: spritePIX = spriteDATA[14:12];
            28: spritePIX = spriteDATA[11:9];
            29: spritePIX = spriteDATA[8:6];
            30: spritePIX = spriteDATA[5:3];
            31: spritePIX = spriteDATA[2:0];
        endcase
    end
   
    // snake if on if one of the bits are high, i.e. number is not 0
    assign snake_on = spritePIX[0] | spritePIX[1] | spritePIX[2]; 

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
