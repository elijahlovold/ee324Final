#pragma once

#include "presets.hpp"
#include "maps.hpp"
#include "basic_io.hpp"

namespace grid_controller {
    sp set_sprite(unsigned int x, unsigned int y, sp sprite_addr, bool check_collision = false); 

    void clear_grid(sp addr = sp::TRANSPARENT);

    sp get_sprite(unsigned int x, unsigned int y);
    
    bool draw_wall_x(unsigned int y, unsigned int x1, unsigned int x2);
    bool draw_wall_y(unsigned int x, unsigned int y1, unsigned int y2);


    sp get_sprite(unsigned int x, unsigned int y) {
        unsigned int offset = y*REG_PER_ROW + (x >> 3);

        // grab the 32 bit sprite storing 4...
        unsigned int sprite = *((unsigned int *)(GRID_CONTROLLER_BASE_ADDR + offset*4));

        // figure out which byte it is by mod 4
        unsigned int byte_position = (x % 8)*4;

        unsigned char existing_sprite = 0xF & (sprite >> byte_position);

        return static_cast<sp>(existing_sprite);
    }

    // if check_collision, will return true if collision detected
    sp set_sprite(unsigned int x, unsigned int y, sp sprite_addr, bool check_collision) { 
        unsigned int offset = y*REG_PER_ROW + (x >> 3);

        // grab the 32 bit sprite storing 4...
        unsigned int sprite = *((unsigned int *)(GRID_CONTROLLER_BASE_ADDR + offset*4));

        // since, sprite contains 4 elements, only want to set the correct one 
        // figure out which byte it is by mod 4
        unsigned int byte_position = (x % 8)*4;

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


    // completely wipe the grid
    void clear_grid(sp addr){
        unsigned char addr_v = static_cast<unsigned char>(addr) & 0xF;
        for (int i = 0; i < 264; i++) {
            unsigned int val = 0;
            for (int j = 0; j < 8; j++) {
                val |= (addr_v << 4*j);
            }
            *((unsigned int *)(GRID_CONTROLLER_BASE_ADDR + i*4)) = val;
        }
    }

    bool check_coords(unsigned int x, unsigned int y) {
        return (x < MAX_X_COORD && y < MAX_Y_COORD);
    } 

    bool draw_wall_x(unsigned int y, unsigned int x1, unsigned int x2) {
        for (int i = x1; i <= x2; i++) { 
            grid_controller::set_sprite(i, y, sp::WALL);
        }
    }
    
    bool draw_wall_y(unsigned int x, unsigned int y1, unsigned int y2) {
        for (int i = y1; i <= y2; i++) { 
            grid_controller::set_sprite(x, i, sp::WALL);
        }
    }

    void set_borders() {
        draw_wall_y(0, 0, MAX_Y_COORD);
        draw_wall_y(2, 0, MAX_Y_COORD);
        draw_wall_y(4, 0, MAX_Y_COORD);
        draw_wall_y(5, 0, MAX_Y_COORD);
    }

}
