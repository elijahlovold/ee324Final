/************************************************************
* Module: flipper                                           *
* Description: simple module to flip a single bit           *
* Input parameters: A                                       *
* Returns: ~A                                               *
* Usages: simple bit flip                                   *
************************************************************/

module flipper(
    input in_true,
    output out_inv
    );
    
    assign out_inv = ~in_true;
endmodule