#pragma once

#include "presets.hpp"
#include "maps.hpp"

#include <iostream>

namespace grid_controller {
    sp set_sprite(unsigned int x, unsigned int y, sp sprite_addr, bool check_collision = false); 
    unsigned int flatten_coords(unsigned int x, unsigned int y);

    void clear_grid(sp addr = sp::TRANSPARENT);

    void load_map(mp_i map_index);

    sp get_sprite(unsigned int x, unsigned int y);

    sp get_sprite(unsigned int x, unsigned int y) {

    }

    // if check_collision, will return true if collision detected
    sp set_sprite(unsigned int x, unsigned int y, sp sprite_addr, bool check_collision) { 
        unsigned int flat_coords = flatten_coords(x, y);    
        
        // first grab the offset
        unsigned int offset = (flat_coords >> 3);

        // grab the 32 bit sprite storing 4...
        unsigned int sprite = *((unsigned int *)(GRID_CONTROLLER_BASE_ADDR + offset*4));

        // unsigned int sprite = 0xFFFFFFFF;

        // since, sprite contains 4 elements, only want to set the correct one 
        // figure out which byte it is by mod 4
        if (y & 1) {
            flat_coords -= 4;
        }

        unsigned int byte_position = (flat_coords % 8)*4;

        // if check_collision and not clearing, first see if a non-transparent sprite is there... 
        if (check_collision && (sprite_addr != sp::TRANSPARENT)) {
            unsigned char existing_sprite = 0xF & (sprite >> byte_position);
            if (existing_sprite != static_cast<unsigned char>(sp::TRANSPARENT)) {
                
                // if collided, return sprite we collided with 
                return static_cast<sp>(existing_sprite);
            }
        }

        // clear the byte to zeros
        sprite &= ~(0xF << byte_position);

        unsigned char sprite_addr_val = static_cast<unsigned char>(sprite_addr) & 0xF;
        sprite |= (sprite_addr_val << byte_position);

        // store the sprite_addr back into the register... 
        *((unsigned int *)(GRID_CONTROLLER_BASE_ADDR + offset*4)) = sprite;
        return sp::TRANSPARENT;
    }


    // // if check_collision, will return true if collision detected
    // bool set_sprite(unsigned int x, unsigned int y, sp &sprite_addr, bool check_collision) { 
    //     unsigned int offset = flatten_coords(x, y);    

    //     // grab the 32 bit sprite storing 4...
    //     unsigned char sprite = *((unsigned char *)(GRID_CONTROLLER_BASE_ADDR + offset));

    //     // if check_collision and not clearing, first see if a non-transparent sprite is there... 
    //     if (check_collision && (sprite_addr != sp::TRANSPARENT)) {
    //         unsigned char existing_sprite = 0xFF & sprite;  // isolate only lowest sprite
    //         if (existing_sprite != static_cast<unsigned char>(sp::TRANSPARENT)) {
                
    //             // if collided, set the sprite_addr to the tile we collided with 
    //             // this allows the user to see what object was there
    //             sprite_addr = static_cast<sp>(existing_sprite);

    //             return true;
    //         }
    //     }

    //     // clear the byte to zeros
    //     sprite &= ~0xFF;  

    //     unsigned char sprite_addr_byte = static_cast<unsigned char>(sprite_addr);
    //     sprite |= sprite_addr_byte;

    //     // store the sprite_addr back into the register... 
    //     *((unsigned int *)(GRID_CONTROLLER_BASE_ADDR + offset)) = sprite;

    //     return false;
    // }

    unsigned int flatten_coords(unsigned int x, unsigned int y) {
        return y*MAX_X_COORDS + x;
    }

    // completely wipe the grid
    void clear_grid(sp addr){
        unsigned char addr_v = static_cast<unsigned char>(addr) & 0xF;
        for (int i = 0; i < 256; i++) {
            unsigned int val = 0;
            for (int j = 0; j < 8; j++) {
                val |= (addr_v << 4*j);
            }
            *((unsigned int *)(GRID_CONTROLLER_BASE_ADDR + i*4)) = val;
        }
    }

    void load_map(mp_i map_index) {

    }

};