`timescale 1ns / 1ps
/************************************************************
* Module: VGA                                               *
* Description:  This module is used generate a VGA and      *
*               screen coordinates for a given resolution.  *
*               Resolution is configurable through input    * 
*               parameters.                                 *
* Input parameters: resolution settings, clk, rst           *
* Returns: HS, VS, Vde, x, y                                *
* Usages: useful for generating VGA signal                  *
************************************************************/

// parameterize the module...
module VGA #(parameter  WIDTH=1920, HEIGHT=1080, 
                        H_SYNC_TIME = 44, V_SYNC_TIME = 5,
                        H_F_PORCH = 88, V_F_PORCH = 4,
                        H_B_PORCH = 148, V_B_PORCH = 36,
                        H_LR_BORDER = 0, V_LR_BORDER = 0
                        ) (
    output HS, VS, Vde, [11:0] x, y,
    input clk, rst
    );

    // first edge of the horizontal drawable region
    localparam H_b_1 = H_SYNC_TIME + H_B_PORCH + H_LR_BORDER;
    // second edge of the horizontal drawable region 
    localparam H_b_2 = WIDTH + H_b_1; 
    // first edge of the vertical drawable region
    localparam V_b_1 = V_SYNC_TIME + V_B_PORCH + V_LR_BORDER;
    // second edge of the vertical drawable region
    localparam V_b_2 = HEIGHT + V_b_1;
    
    // rst value simply sum of all values...
    localparam H_rst = H_b_2 + H_F_PORCH + H_LR_BORDER; 
    localparam V_rst = V_b_2 + V_F_PORCH + V_LR_BORDER;    
    
    // reset signals for the horizontal and vertical counters
    wire [31:0] h_count, v_count;
    
    wire h_en, v_en;    // timer enables

    // always count horizontal
    assign h_en = 1;
    // only count when horizontal elapses
    assign v_en = (h_count >= (H_rst) ? 1 : 0); 

    // instantiate counters
    counter #(H_rst, 32) H_COUNT (h_count, h_en, clk, rst);
    counter #(V_rst, 32) V_COUNT (v_count, v_en, clk, rst);
    
    // assign sync sigs, true if counters are under sync time
    assign HS = (h_count < H_SYNC_TIME); 
    assign VS = (v_count < V_SYNC_TIME); 
  
    // lastly, Vde high if both counters are in the drawable region
    assign Vde = (h_count >= H_b_1) && (h_count < H_b_2) && (v_count >= V_b_1) && (v_count < V_b_2);

    // output is directly the counters 
    assign x = h_count;
    assign y = v_count;

endmodule

