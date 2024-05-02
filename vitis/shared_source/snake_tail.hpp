#pragma once

#include "presets.hpp"
#include "snake_node.hpp"

// inherit from snake_node
class snake_tail : public snake_node {
    public: 
        snake_tail(int x = 0, int y = 0) : snake_node(sp::TAIL_UP, x, y) {}

        void move_tail(int x, int y, int d_x, int d_y) {
            // first update tail sprite
            if (d_x > x) {
                this->set_sprite(sp::TAIL_RIGHT);
            } else if (d_x < x) {
                this->set_sprite(sp::TAIL_LEFT);
            } else if (d_y > y) {
                this->set_sprite(sp::TAIL_DOWN);
            } else {
                this->set_sprite(sp::TAIL_UP);
            }

            this->move_node(x, y);
        }
};



