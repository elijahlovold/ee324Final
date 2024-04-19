#pragma once

#include "presets.hpp"
#include "input_controller.hpp"
#include "grid_controller.hpp"

class snake_node {
    public: 
        snake_node(sp sprite, unsigned int x, unsigned int y);

        bool set_coords(unsigned int x, unsigned int y, bool check_collision = false);
        void set_sprite(sp sprite);

        unsigned int get_coords(bool y) const;
        unsigned int get_sprite() const;

    private: 
        sp node_sprite;  // only need 8 bytes
        unsigned int coord_x;       // grid tile the node is on
        unsigned int coord_y;       // grid tile the node is on 
};

bool snake_node::set_coords(unsigned int x, unsigned int y, bool check_collision) {
    // everytime we move the snake, we must first clear the current sprite
    grid_controller::set_sprite(this->coord_x, this->coord_y);

    if (x > MAX_X_COORDS || y > MAX_Y_COORDS) {
        return false;
    } else {
        this->coord_x = x;
        this->coord_y = y;
    }
    // update the node on the grid
    return grid_controller::set_sprite(this->coord_x, this->coord_y, this->node_sprite, check_collision);
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

unsigned int snake_node::get_sprite() const {
    return this->node_sprite;
}

snake_node::snake_node(sp sprite, unsigned int x, unsigned int y) {
    // check inputs... 
    this->set_coords(x, y);
    this->set_sprite(sprite);
}




class Snake {
    public:
        unsigned int head_i = 0;
        unsigned int tail_i = snake_length - 1;

        unsigned int snake_length;
        std::vector<snake_node> snake;

        dir head_direction;

        Snake(unsigned int length = SNAKE_LENGTH);
        
        void reset_snake(int head_x = 0, int head_y = 0);
        void set_direction(dir h_dir);

        bool step_snake();
        void add_segment();

        void read_inputs();
};

Snake::Snake(unsigned int length) : snake_length(snake_length) {
    // first, set direction to right
    this->set_direction(dir::RIGHT);

    // initialize snake with all nodes in center and all sprites snake body...
    this->snake = std::vector<snake_node> (snake_length, snake_node(sp::BODY, 0, 0));

    this->snake[this->head_i].set_sprite(sp::HEAD);   // set the tail
    this->snake[this->tail_i].set_sprite(sp::TAIL);   // set the head

    // set the snake in the middle of the map
    this->reset_snake();

}

// reset the snake to the initial conditions
void Snake::reset_snake(int head_x, int head_y) {
    this->snake[this->head_i].set_coords(head_x, head_y);
    if (this->head_direction == dir::RIGHT || this->head_direction == dir::LEFT) {
        for (int i = 0; i < snake_length - 1; i++) {
            this->snake[i].set_coords(head_x + this->head_direction*i, head_y);
        }
    } else {
        for (int i = 0; i < snake_length - 1; i++) {
            this->snake[i].set_coords(head_x, head_y + this->head_direction*i);
        }
    }
}

void Snake::set_direction(dir h_dir) {
    this->head_direction = h_dir;
}

bool Snake::step_snake() {
    // loop through snake updating positions

    unsigned int prev_x = this->snake[this->head_i].get_coords(0);
    unsigned int prev_y = this->snake[this->head_i].get_coords(1);
    unsigned int prev_x_temp, prev_y_temp;

    // first determine where head should go
    // if the head collides, return false
    if (this->head_direction == dir::RIGHT || this->head_direction == dir::LEFT) {
        if (this->snake[head_i].set_coords(prev_x + this->head_direction, prev_y, true) == false) {
            return false;
        }
    } else {
        if (this->snake[head_i].set_coords(prev_x, prev_y + this->head_direction, true) == false) {
            return false;
        }
    }

    // next, loop through the snake updating coords
    for (int i = 1; i < snake_length; i++) {
        // save current coords
        prev_x_temp = this->snake[i].get_coords(0);
        prev_y_temp = this->snake[i].get_coords(1);

        this->snake[i].set_coords(prev_x, prev_y);

        prev_x = prev_x_temp;
        prev_y = prev_y_temp;
    }
}

void Snake::add_segment() {
    // first figure out where to add the tail
    unsigned int tail_x = this->snake[this->tail_i].get_coords(0);
    unsigned int tail_y = this->snake[this->tail_i].get_coords(1);

    unsigned int tail_1_x = this->snake[this->tail_i - 1].get_coords(0);
    unsigned int tail_1_y = this->snake[this->tail_i - 1].get_coords(1);

    int x_diff = tail_x - tail_1_x;
    int y_diff = tail_y - tail_1_y;
    // if diff in x, add along x direction
    if (x_diff != 0) {
        this->snake.push_back(snake_node(sp::TAIL, tail_x + x_diff, tail_y));
    } 
    // else, add along y direction  
    else {
        this->snake.push_back(snake_node(sp::TAIL, tail_x, tail_y + y_diff));
    }

    // now, update old tail sprite
    this->snake[this->tail_i].set_sprite(sp::BODY);

    // update length and tail index
    this->snake_length = this->snake.size();
    this->tail_i = snake_length - 1;
}

void Snake::read_inputs() {
    unsigned int raw_cmds = input_controller::read_raw_inputs();

    if (input_controller::check_input(raw_cmds, inputs::UP_I)) {
        this->set_direction(dir::UP);
    } else if (input_controller::check_input(raw_cmds, inputs::DOWN_I)) {
        this->set_direction(dir::DOWN);
    } else if (input_controller::check_input(raw_cmds, inputs::RIGHT_I)) {
        this->set_direction(dir::RIGHT);
    } else if (input_controller::check_input(raw_cmds, inputs::LEFT_I)) {
        this->set_direction(dir::LEFT);
    }

    if (input_controller::check_input(raw_cmds, inputs::PORTAL1)) {
        // TODO: implement portal control
    } else if (input_controller::check_input(raw_cmds, inputs::PORTAL2)) {
        // TODO: implement portal control
    }

    if (input_controller::check_input(raw_cmds, inputs::BOOST)) {
        // TODO: implement boost control
    }
}