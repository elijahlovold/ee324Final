#pragma once

#include <stdint.h>
#include <unistd.h>
#include <math.h>
#include <vector>
#include <time.h>

#define GRID_CONTROLLER_BASE_ADDR 0x43C20000
#define INPUT_CONTROLLER_BASE_ADDR 0x43C10000
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
    BK = 1, 
    // WALL = 2, 
    // ARCH = 3, 

    // PORTALS = 10, // sending portal 
    // PORTALR = 11, // recieving portal

    // HEAD = 20,
    // TAIL = 21, 
    // BODY = 22,
    WALL = 1, 
    ARCH = 1, 

    PORTALS = 1, // sending portal 
    PORTALR = 1, // recieving portal

    HEAD = 1,
    TAIL = 1, 
    BODY = 1,
};

enum dir {
    RIGHT = 1,
    LEFT = -1,
    UP = 1, 
    DOWN = -1,
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