#pragma once

#include "presets.hpp"

class snake_node {
    public: 
        snake_node(unsigned char sprite, unsigned int x, unsigned int y);

        void set_coords(unsigned int x, unsigned int y);
        void set_sprite(unsigned char sprite);

        unsigned int get_coords(bool y) const;
        unsigned int get_sprite() const;

    private: 
        unsigned char node_sprite;  // only need 8 bytes
        unsigned int coord_x;       // grid tile the node is on
        unsigned int coord_y;       // grid tile the node is on 
};

void snake_node::set_coords(unsigned int x, unsigned int y) {
    if (x > MAX_X_COORDS || y > MAX_Y_COORDS) {
        return;
    } else {
        this->coord_x = x;
        this->coord_y = y;
    }
}

void snake_node::set_sprite(unsigned char sprite) {
    if (sprite > MAX_SPRITE_ADDR) {
        return;
    } else {
        this->node_sprite = sprite;
    }
}

unsigned int snake_node::get_coords(bool y) const {
    if (y) {
        return this->coord_y;
    } else {
        return this->coord_x;
    }
}

unsigned int snake_node::get_sprite() const {
    return this->node_sprite;
}


snake_node::snake_node(unsigned char sprite, unsigned int x, unsigned int y) {
    // check inputs... 
    this->set_coords(x, y);
    this->set_sprite(sprite);
}

class Snake {
    public:
        unsigned int snake_length;
        std::vector<snake_node> snake;

        Snake(unsigned int length);

};


    Snake::Snake(unsigned int length) : snake_length(snake_length) {
        this->snake = std::vector<snake_node> (snake_length, snake_node(0, 0, 0));

        this->snake[0].set_sprite(1);   // set the tail
        this->snake[snake_length - 1].set_sprite(2);   // set the head

        // set the snake in the middle of the map
        // ...

    }