#pragma once

#include "presets.hpp"
#include "basic_io.hpp"

// functions for interacting with the sprite grid
namespace grid_controller {
    sp set_sprite(unsigned int x, unsigned int y, sp sprite_addr, bool check_collision = false); 

    void clear_grid(sp addr = sp::TRANSPARENT);

    sp get_sprite(unsigned int x, unsigned int y);
    
    bool draw_wall_x(unsigned int y, unsigned int x1, unsigned int x2);
    bool draw_wall_y(unsigned int x, unsigned int y1, unsigned int y2);

    void load_map(int i);


    sp get_sprite(unsigned int x, unsigned int y) {
        // since only 8 registers per row (64 tiles) and 60 tiles + 6 out of bounds from h_sync, etc...
        // reuse first few for the end of the screen since offscreen anyways 
        // mod by x by 64
        x = x%64;

        // grab the offset 
        unsigned int offset = y*REG_PER_ROW + (x >> 3);

        // grab the 32 bit sprite storing 8 tiles...
        unsigned int sprite = *((unsigned int *)(GRID_CONTROLLER_BASE_ADDR + offset*4));

        // figure out which byte it is by mod 8 
        unsigned int byte_position = (x % 8)*4;

        // mask off existing sprite
        unsigned char existing_sprite = 0xF & (sprite >> byte_position);

        // cast and return 
        return static_cast<sp>(existing_sprite);
    }

    // if check_collision, will return true if collision detected
    sp set_sprite(unsigned int x, unsigned int y, sp sprite_addr, bool check_collision) { 
        // since only 8 registers per row (64 tiles) and 60 tiles + 6 out of bounds from h_sync, etc...
        // reuse first few for the end of the screen since offscreen anyways 
        // mod by x by 64 
        x = x%64;

        // grab the offset 
        unsigned int offset = y*REG_PER_ROW + (x >> 3);

        // grab the 32 bit sprite storing 8...
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
        // else, set the sprite

        // clear the target nibble to zeros
        sprite &= ~(0xF << byte_position);

        // convert desired sprite to char
        unsigned char sprite_addr_val = static_cast<unsigned char>(sprite_addr) & 0xF;
        // or with existing to set
        sprite |= (sprite_addr_val << byte_position);

        // store the sprite_addr back into the register... 
        *((unsigned int *)(GRID_CONTROLLER_BASE_ADDR + offset*4)) = sprite;
        return sp::TRANSPARENT;
    }


    // completely wipe the grid
    void clear_grid(sp addr){
        // loop through all registers setting them to desired addr
        unsigned char addr_v = static_cast<unsigned char>(addr) & 0xF;
        for (int i = 0; i < 288; i++) {
            unsigned int val = 0;
            for (int j = 0; j < 8; j++) {
                val |= (addr_v << 4*j);
            }
            *((unsigned int *)(GRID_CONTROLLER_BASE_ADDR + i*4)) = val;
        }
    }

    // check if x-y pair is in bounds
    bool check_coords(unsigned int x, unsigned int y) {
        return ((x >= MIN_X_COORD) && (x <= MAX_X_COORD) && (y >= MIN_Y_COORD) && (y <= MAX_Y_COORD));
    } 

    // draw a wall segment
    void draw_seg(wall_seg seg) {
        // determine if its a horizontal or vertical wall
        if (seg.d == dir::HORI) {
            draw_wall_x(seg.c, seg.b1, seg.b2);
        } else {
            draw_wall_y(seg.c, seg.b1, seg.b2);
        }
    }
    
    // draw a horizontal wall
    bool draw_wall_x(unsigned int y, unsigned int x1, unsigned int x2) {
        for (int i = x1; i <= x2; i++) { 
            grid_controller::set_sprite(i, y, sp::WALL);
        }
    }

    // draw a vertical wall
    bool draw_wall_y(unsigned int x, unsigned int y1, unsigned int y2) {
        for (int i = y1; i <= y2; i++) { 
            grid_controller::set_sprite(x, i, sp::WALL);
        }
    }

    // load a preset map from index
    void load_map(int i) {
        grid_controller::clear_grid();

        // blank
        if (i == 0) {
            return;
        }
        
        // mod by map size to avoid errors on invalid access
        i = i % maps.size(); 
        // select the map, is a vector of wall segments 
        std::vector<wall_seg> selected_map = maps[i];

        // draw each wall segment
        for (wall_seg seg : selected_map) {
            draw_seg(seg);
        }
    }
}