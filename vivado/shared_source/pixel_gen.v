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
module pixel_gen_temp(
    output reg [7:0] R, G, B, 
    input [15:0] x, y, input vde,
    input [3:0] btn_on     // GRYB  
    );

    // set in center of screen
    localparam center_x = 1100;
    localparam center_y = 562;

    // control lines to enable the 4 buttons
    wire btn0_en, btn1_en, btn2_en, btn3_en;

    // get local coords relative to center of screen
    wire [15:0] local_x, local_y;
    assign local_x = x - center_x;
    assign local_y = y - center_y;

 
    // output block
    always @(*) begin 
        if (~vde) begin
            {R, G, B} <= 24'h000000;                // nothing
        end 
        else begin 
            if (btn0_en && btn_on[0]) begin         // btn0
                {R, G, B} <= 0; 
            end
            else begin                   
                {R, G, B} <= 0;             // background  
            end 
        end
    end
endmodule