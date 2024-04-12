#pragma once

#include "presets.hpp"


// might make this into a singleton class...
class grid_controller {

    public: 
        unsigned char flattened_grid[MAX_X_COORDS*MAX_Y_COORDS];

        static void set_sprite(unsigned char sprite_addr, unsigned int x, unsigned int y); 

};

void grid_controller::set_sprite(unsigned char sprite_addr, unsigned int x, unsigned int y) {

    // ... implement ...
}
