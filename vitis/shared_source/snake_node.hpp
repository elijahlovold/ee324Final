#pragma once

#include "presets.hpp"
#include "input_controller.hpp"
#include "grid_controller.hpp"
#include "portal.hpp"
#include "basic_io.hpp"
#include "uart_controller.hpp"
#include "food.hpp"

class snake_node {
    public: 
        snake_node(sp sprite, unsigned int x, unsigned int y);

        bool set_coords(unsigned int x, unsigned int y);
        sp move_node(unsigned int x, unsigned int y, bool check_collision = false, bool replace = true);
        void set_sprite(sp sprite);

        unsigned int get_coords(bool y) const;
        sp get_sprite() const;

        unsigned int coord_x;       // grid tile the node is on
        unsigned int coord_y;       // grid tile the node is on 

    protected: 
        dir_big node_dir;

        sp node_sprite;  // only need 8 bytes
 };

bool snake_node::set_coords(unsigned int x, unsigned int y) {
    if (x > MAX_X_COORDS || y > MAX_Y_COORDS) {
        io::output_to_SevenSeg(234);    // error indication
        return false;
    } else {
        this->coord_x = x;
        this->coord_y = y;
    }
}    

sp snake_node::move_node(unsigned int x, unsigned int y, bool check_collision, bool replace) {
    // everytime we move the snake, we must first clear the current sprite
    if (replace) {
        grid_controller::set_sprite(this->coord_x, this->coord_y, sp::TRANSPARENT);
    }

    if (x > MAX_X_COORDS || y > MAX_Y_COORDS) {
        io::output_to_SevenSeg(234);    // error indication
        return sp::ERROR;
    } else {
        this->coord_x = x;
        this->coord_y = y;
    }

    return grid_controller::set_sprite(this->coord_x, this->coord_y, this->node_sprite, check_collision);
    // return grid_controller::set_sprite(this->coord_x, this->coord_y, this->node_sprite, check_collision);
}

void snake_node::set_sprite(sp sprite) {
    this->node_sprite = sprite;

    // update the node on the grid
    grid_controller::set_sprite(this->coord_x, this->coord_y, this->node_sprite);
}

unsigned int snake_node::get_coords(bool y) const {
    if (y) {
        return this->coord_y;
    } else {
        return this->coord_x;
    }
}

sp snake_node::get_sprite() const {
    return this->node_sprite;
}

snake_node::snake_node(sp sprite, unsigned int x, unsigned int y) {
    if (grid_controller::check_coords(x, y) == false) {
        return;
    } else {
        this->coord_x = x;
        this->coord_y = y;
    }
    this->set_sprite(sprite);
}