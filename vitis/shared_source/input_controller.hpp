#pragma once

#include "snake.hpp"

namespace controller {

unsigned char read_input(Snake *p1) {
    // read in p1 movement
    return p1->read_controller();
}

unsigned char read_input(Snake* p1, Snake* p2) {
    // read in p1 movement
    p1->read_controller();
    // read in p2 movement
    p2->read_controller();

    return 0;
}


    
};