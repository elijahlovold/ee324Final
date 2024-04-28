#pragma once

#include <stdint.h>
#include <unistd.h>
#include <math.h>
#include <vector>
#include <time.h>
#include <stdio.h>	  		// Library for srand() and rand()

#define PERIOD 1000     // sets the period of the PWM

#define GRID_CONTROLLER_BASE_ADDR 0x43C30000

// depricated, don't use
// #define INPUT_CONTROLLER_BASE_ADDR 0x43C40000

#define BUTTONS_BASEADDR 0x41200000		// Base address of the Buttons 
#define LED_BASEADDR     0x41210000		// Base address of LED IP
#define SWITCH_BASEADDR  0x41220000		// Base address of the Switches 						
#define SVN_SEG_CNTL     0x43C10000		// Base address of the Seven-Segment Display Control 	
#define SVN_SEG_DATA     0x43C10004		// Base address of the Seven-Segment Display Data

// RGB addresses for LED #10 and #11
#define B_0_EN 		0x43C00000
#define B_0_PERIOD  0x43C00004
#define B_0_WIDTH 	0x43C00008

#define G_0_EN 		0x43C00010
#define G_0_PERIOD  0x43C00014
#define G_0_WIDTH 	0x43C00018

#define R_0_EN 		0x43C00020
#define R_0_PERIOD  0x43C00024
#define R_0_WIDTH 	0x43C00028

#define B_1_EN 		0x43C00030
#define B_1_PERIOD  0x43C00034
#define B_1_WIDTH 	0x43C00038

#define G_1_EN 		0x43C00040
#define G_1_PERIOD  0x43C00044
#define G_1_WIDTH 	0x43C00048

#define R_1_EN 		0x43C00050
#define R_1_PERIOD  0x43C00054
#define R_1_WIDTH 	0x43C00058



#define GTC_BASE_ADDR 0xF8F00000

// #define TIMER_BASE_ADDR 0x41250000
#define UART_BASE_ADDR 0xE0001000

#define MAX_SPRITE_ADDR 45

#define X_RES 1920
#define Y_RES 1080
#define SPRITE_RES 32

#define MAX_X_COORDS 60
#define MAX_Y_COORDS 33

#define SNAKE_LENGTH 5

enum sp {
    TRANSPARENT = 0,

    HEAD_UP = 1, 
    HEAD_DOWN = 3, 
    HEAD_LEFT = 4, 
    HEAD_RIGHT = 2, 
    
    BODY = 5, 

    TAIL_UP = 6, 
    TAIL_DOWN = 8, 
    TAIL_RIGHT = 7, 
    TAIL_LEFT = 9, 

    FOOD = 10, 
    FOOD_TEL = 11, 

    PORTALS = 12, 
    PORTALR = 13, 

    WALL = 14, 

    ERROR = 999,
};

enum input_device {
    CONTROLLER = 0, 
    BOARD = 1
};

enum dir_big {
    UP = 0, 
    DOWN = 1, 
    RIGHT = 2, 
    LEFT = 3
}; 

enum dir {
    HORI = 0,
    VERT = 1
};

enum inc {
    POS = 1,
    NEG = -1, 
};

enum inputs {
    UP_I = 0, 
    DOWN_I = 1, 
    RIGHT_I = 2, 
    LEFT_I = 3, 

    PORTAL1 = 10,
    PORTAL2 = 11, 
    BOOST = 12, 

};

// enum portal_type {
//     SENDER1 = 0, 
//     RECIEVER1 = 1, 

//     SENDER2 = 2, 
//     RECIEVER2 = 3, 

//     SENDER3 = 4, 
//     RECIEVER3 = 5, 
// };

enum mp_i {
    LVL1 = 0,
    LVL2 = 0,
    LVL3 = 0,
};


struct map_el {
    sp sprite_addr;
    unsigned int x;
    unsigned int y;

    // Constructor to initialize the struct with values
    map_el(sp addr, unsigned int x_val, unsigned int y_val)
        : sprite_addr(addr), x(x_val), y(y_val) {}
};

// namespace mp {
//     std::vector<map_el> LVL1 = {map_el(sp::WALL, 4, 4), };

//     std::vector<map_el> LVL2 = {map_el(sp::WALL, 4, 4), };

//     std::vector<map_el> LVL3 = {map_el(sp::WALL, 4, 4), };
// };