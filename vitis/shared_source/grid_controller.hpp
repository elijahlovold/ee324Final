#pragma once

#include "presets.hpp"
#include "maps.hpp"

#include <iostream>

namespace grid_controller {

    bool set_sprite(unsigned int x, unsigned int y, sp sprite_addr = sp::TRANSPARENT, bool check_collision = false); 
    unsigned int flatten_coords(unsigned int x, unsigned int y);

    void clear_grid(sp addr = sp::TRANSPARENT);

    void load_map(mp_i map_index);

    // if check_collision, will return true if collision detected
    bool set_sprite(unsigned int x, unsigned int y, sp sprite_addr, bool check_collision) { 
        unsigned int flat_coords = flatten_coords(x, y);    
        
        // first grab the offset
        unsigned int offset = flat_coords >> 2;

        // grab the 32 bit sprite storing 4...
        unsigned int sprite = *((unsigned int *)(GRID_CONTROLLER_BASE_ADDR + offset));
        // unsigned int sprite = 0xFFFFFFFF;

        // since, sprite contains 4 elements, only want to set the correct one 
        // figure out which byte it is by mod 4
        unsigned int byte_position = (flat_coords % 4)*8;

        // if check_collision and not clearing, first see if a non-transparent sprite is there... 
        if (check_collision && sprite_addr != sp::TRANSPARENT) {
            unsigned char existing_sprite = 0xFF & (sprite >> byte_position);
            if (existing_sprite != static_cast<unsigned char>(sp::TRANSPARENT)) {
                return 1;
            }
        }

        // clear the byte to zeros
        sprite &= ~(0xFF << byte_position);

        unsigned char sprite_addr_val = static_cast<unsigned char>(sprite_addr);
        sprite |= (sprite_addr_val << byte_position);

        // std::cout << sprite << " " << offset << " " << flat_coords;
        // store the sprite_addr back into the register... 
        *((unsigned int *)(GRID_CONTROLLER_BASE_ADDR + offset)) = sprite;

        return 0;
    }

    unsigned int grid_controller::flatten_coords(unsigned int x, unsigned int y) {
        return y*MAX_X_COORDS + x;
    }

    // completely wipe the grid
    void grid_controller::clear_grid(sp addr){
        for (int x = 0; x < MAX_X_COORDS; x++) {
            for (int y = 0; y < MAX_Y_COORDS; y++) {
                set_sprite(x, y, addr);
            }
        }
    }

    void grid_controller::load_map(mp_i map_index) {

    }

};