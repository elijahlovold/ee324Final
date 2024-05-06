#pragma once

#include "presets.hpp"
#include "snake_node.hpp"

// tail inherits from snake_node
class snake_tail : public snake_node {
    public: 
        // default constructor sets node to TAIL_UP
        snake_tail(int x = 0, int y = 0) : snake_node(sp::TAIL_UP, x, y) {}

        // function for moving the tail
        void move_tail(int x, int y, int d_x, int d_y) {
            // determine which direction the tail is pointing
            // ...to set the correct sprite direction
            if (d_x > x) {
                this->set_sprite(sp::TAIL_RIGHT);
            } else if (d_x < x) {
                this->set_sprite(sp::TAIL_LEFT);
            } else if (d_y > y) {
                this->set_sprite(sp::TAIL_DOWN);
            } else {
                this->set_sprite(sp::TAIL_UP);
            }

            // move the node
            this->move_node(x, y);
        }
};



