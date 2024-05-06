#pragma once

#include "presets.hpp"
#include "grid_controller.hpp"
#include "portal.hpp"
#include "basic_io.hpp"
#include "uart_controller.hpp"
#include "food.hpp"

// base class to create snake nodes
class snake_node {
    public: 
        // constructor
        snake_node(sp sprite, unsigned int x = 0, unsigned int y = 0);

        // helpful setters
        bool set_coords(unsigned int x, unsigned int y);
        sp move_node(unsigned int x, unsigned int y, bool check_collision = false, bool replace = true);
        void set_sprite(sp sprite);
        void clear_node();

        // helpful getters
        unsigned int get_coords(bool y) const;
        sp get_sprite() const;

        unsigned int coord_x;       // grid tile the node is on
        unsigned int coord_y;       // grid tile the node is on 

        sp node_sprite;  // only need 8 bytes
};

// check and set coords 
bool snake_node::set_coords(unsigned int x, unsigned int y) {
    if (x > MAX_X_COORD || y > MAX_Y_COORD) {
        io::output_to_SevenSeg(234);    // error indication
        return false;
    } else {
        this->coord_x = x;
        this->coord_y = y;
    }
}    

// move the node to new location, options to check for collision and replace first
sp snake_node::move_node(unsigned int x, unsigned int y, bool check_collision, bool replace) {
    // everytime we move the snake, we must first clear the current sprite
    // ...if enabled
    if (replace) {
        grid_controller::set_sprite(this->coord_x, this->coord_y, sp::TRANSPARENT);
    }

    // check coords - later replace with check coords function...
    if (x > MAX_X_COORD || x < MIN_X_COORD || y > MAX_Y_COORD || y < MIN_Y_COORD) {
        io::output_to_SevenSeg(234);    // error indication
        return sp::ERROR;
    } else {
        this->coord_x = x;
        this->coord_y = y;
    }

    // set the sprite
    return grid_controller::set_sprite(this->coord_x, this->coord_y, this->node_sprite, check_collision);
}

// set the sprite
void snake_node::set_sprite(sp sprite) {
    this->node_sprite = sprite;

    // update the node on the grid
    grid_controller::set_sprite(this->coord_x, this->coord_y, this->node_sprite);
}

// clear the node
void snake_node::clear_node() {
    grid_controller::set_sprite(this->coord_x, this->coord_y, sp::TRANSPARENT);
}

// get current coord, either x or y
unsigned int snake_node::get_coords(bool y) const {
    if (y) {
        return this->coord_y;
    } else {
        return this->coord_x;
    }
}

// get the sprite
sp snake_node::get_sprite() const {
    return this->node_sprite;
}

// constructor
snake_node::snake_node(sp sprite, unsigned int x, unsigned int y) {
    // note, fine to spawn them outside bounds...
    this->coord_x = x;
    this->coord_y = y;
    this->node_sprite = sprite;
}