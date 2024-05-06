#pragma once

#include <stdint.h>
#include <unistd.h>
#include <math.h>
#include <vector>
#include <time.h>
#include <stdio.h>	  		// Library for srand() and rand()

#define PERIOD 1000     // sets the period of the PWM

// all the AXI addresses...

#define GRID_CONTROLLER_BASE_ADDR 0x43C30000
#define COLOR_CONTROLLER_BASE_ADDR 0x43C40000
#define AUDIO_CONTROLLER_BASE_ADDR 0x43C50000

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
#define UART_BASE_ADDR 0xE0001000

// max sprite allowed
#define MAX_SPRITE_ADDR 45

// resolution data of screen and sprite
#define X_RES 1920
#define Y_RES 1080
#define SPRITE_RES 32

// valid coords
#define MAX_X_COORD 65
#define MIN_X_COORD 6
#define MAX_Y_COORD 34
#define MIN_Y_COORD 1

// registers per row of the screen
#define REG_PER_ROW 8

// initial snake length
#define SNAKE_LENGTH 5

// enum for all the valid sprites
enum sp {
    TRANSPARENT = 0,

    // snake sprites
    HEAD_UP = 1, 
    HEAD_DOWN = 3, 
    HEAD_LEFT = 4, 
    HEAD_RIGHT = 2, 
    
    BODY = 5, 

    TAIL_UP = 6, 
    TAIL_DOWN = 8, 
    TAIL_RIGHT = 7, 
    TAIL_LEFT = 9, 

    // map items
    FOOD = 10, 
    FOOD_TEL = 11, 

    PORTALS = 12, 
    PORTALR = 13, 

    WALL = 14, 

    ERROR = 999,
};

// enum for all the valid clips
enum clip {
    NONE = 0,
    CHOMP = 1, 
    PORTAL_PLACE = 2, 
    PORTAL_TRAVEL = 3, 
    PING = 4, 
    GAME_OVER = 5,
};

// struct for an RGB color
struct RGB {
    unsigned char R;
    unsigned char G;
    unsigned char B;

    RGB(unsigned char r, unsigned char g, unsigned char b) : R(r), G(g), B(b) {}
};

// all the color presets
#define NUM_COLORS 17
RGB color_presets[] = {
    RGB(255, 0, 0),     // Red
    RGB(220, 20, 60),     // Crimson
    RGB(178, 34, 34),     // Fire Brick
    RGB(255, 127, 0),   // Orange
    RGB(255, 165, 0),   // Lighter Orange
    RGB(255, 255, 0),   // Yellow
    RGB(173, 255, 47),  // Spring Green
    RGB(0, 255, 0),     // Green
    RGB(173, 216, 230),   // Light Blue
    RGB(0, 255, 255),   // Cyan
    RGB(0, 191, 255),     // Deep Sky Blue
    RGB(0, 0, 255),     // Blue
    RGB(65, 105, 225),    // Royal Blue
    RGB(138, 43, 226),  // Darker Violet
    RGB(148, 0, 211),   // Violet
    RGB(75, 0, 130),    // Indigo
};

// enum for all the valid ps4 controller commands
enum CMDS {
    UP = 22,
    DOWN = 23, 
    RIGHT = 20, 
    LEFT = 21, 

    PORTAL1 = 8, 
    PORTAL2 = 7, 

    START = 14,
    MINUS = 15,

    RANGE = 16,

    INC_COLOR = 18,
    DEC_COLOR = 19
};

// enum for all the color controls on the screen
enum color {
    BODY_CORE = 1, 
    PUPIL = 2,
    IRIS_APPLE = 3, 
    BODY_ACCENT_SEND = 4, 

    RECIEVE = 7,
};

// enum for all valid input devices
enum input_device {
    CONTROLLER = 0, 
    BOARD = 1
};

// enum for all valid directions
enum dir {
    HORI = 0,
    VERT = 1
};

// enum for increment directions
enum inc {
    POS = 1,
    NEG = -1, 
};

// struct for a vertical or horizontal wall segment
struct wall_seg {
    dir d;
    unsigned int c;
    unsigned int b1;
    unsigned int b2;

    wall_seg(dir d, unsigned int c, unsigned int b1, unsigned int b2) : d(d), c(c), b1(b1), b2(b2) {}
};

// vector of possible maps to play
// maps get harder the further the index
// each map is just a vector of wall segments
// this implementation makes it very easy to add new maps
std::vector<std::vector<wall_seg>> maps = {
    // empty map
    {},

    {
    wall_seg(dir::VERT, 30, 4, 18), 
    wall_seg(dir::VERT, 40, 4, 18), 
    wall_seg(dir::VERT, 20, 22, 28), 
    wall_seg(dir::VERT, 50, 22, 28), 
    wall_seg(dir::HORI, 28, 20, 50), 
    }, 

    {
    wall_seg(dir::HORI, 17, MIN_X_COORD, MAX_X_COORD), 
    wall_seg(dir::VERT, 35, MIN_Y_COORD, MAX_Y_COORD), 
    },

    {
    wall_seg(dir::HORI, 10, 15, 56), 
    wall_seg(dir::HORI, 25, 15, 56), 
    wall_seg(dir::VERT, 15, 10, 25), 
    wall_seg(dir::VERT, 56, 10, 25), 
    },

    {
    wall_seg(dir::HORI, 5, MIN_X_COORD, 34), 
    wall_seg(dir::HORI, 10, 35, MAX_X_COORD), 
    wall_seg(dir::HORI, 15, MIN_X_COORD, 34), 
    wall_seg(dir::HORI, 20, 35, MAX_X_COORD), 
    wall_seg(dir::HORI, 25, MIN_X_COORD, 34), 
    wall_seg(dir::HORI, 30, 35, MAX_X_COORD), 
    }, 

    {
    wall_seg(dir::HORI, 10, MIN_X_COORD, 15), 
    wall_seg(dir::HORI, 10, 56, MAX_X_COORD), 
    wall_seg(dir::HORI, 25, MIN_X_COORD, 15), 
    wall_seg(dir::HORI, 25, 56, MAX_X_COORD), 
    wall_seg(dir::VERT, 15, MIN_Y_COORD, 10), 
    wall_seg(dir::VERT, 15, 25, MAX_Y_COORD), 
    wall_seg(dir::VERT, 56, MIN_Y_COORD, 10), 
    wall_seg(dir::VERT, 56, 25, MAX_Y_COORD), 
    wall_seg(dir::HORI, 17, 34, 35), 
    wall_seg(dir::HORI, 18, 34, 35), 
    }, 

    {
    wall_seg(dir::HORI, 13, MIN_X_COORD, MAX_X_COORD), 
    wall_seg(dir::HORI, 22, MIN_X_COORD, MAX_X_COORD), 
    wall_seg(dir::VERT, 30, MIN_Y_COORD, MAX_Y_COORD), 
    wall_seg(dir::VERT, 40, MIN_Y_COORD, MAX_Y_COORD), 
    }, 

    {
    wall_seg(dir::HORI, 13, MIN_X_COORD, MAX_X_COORD), 
    wall_seg(dir::HORI, 17, MIN_X_COORD, MAX_X_COORD), 
    wall_seg(dir::HORI, 22, MIN_X_COORD, MAX_X_COORD), 
    wall_seg(dir::VERT, 10, MIN_Y_COORD, 22), 
    wall_seg(dir::VERT, 15, MIN_Y_COORD, MAX_Y_COORD), 
    wall_seg(dir::VERT, 25, MIN_Y_COORD, MAX_Y_COORD), 
    wall_seg(dir::VERT, 30, MIN_Y_COORD, MAX_Y_COORD), 
    wall_seg(dir::VERT, 40, MIN_Y_COORD, MAX_Y_COORD), 
    wall_seg(dir::VERT, 45, MIN_Y_COORD, MAX_Y_COORD), 
    wall_seg(dir::VERT, 55, MIN_Y_COORD, MAX_Y_COORD), 
    wall_seg(dir::VERT, 60, 13, MAX_Y_COORD), 
    },
    
};