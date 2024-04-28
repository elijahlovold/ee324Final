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
module pixel_gen_temp (
    input vde,
    input snake_on,
    input [23:0] snake_color,

    output reg [7:0] R, G, B
    );

    // output block
    always @(*) begin 
        if (~vde) begin
            {R, G, B} = 24'h000000;
        end 
        else begin
            if(snake_on) begin
                {R, G, B} = snake_color;
            end
            else begin                   
                {R, G, B} = 24'hFFFFFF;             // background  
            end 
        end
    end
endmodule