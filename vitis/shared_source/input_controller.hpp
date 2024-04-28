#pragma once

#include "snake.hpp"

namespace controller {

unsigned char read_input(Snake *p1) {
    // read in p1 movement
    p1->read_controller();
    // read in dump data
    uart::read_char_UART1();

    return 0;
}

unsigned char read_input(Snake* p1, Snake* p2) {
    // read in p1 movement
    p1->read_controller();
    // read in p2 movement
    p2->read_controller();

    return 0;
}


    
};