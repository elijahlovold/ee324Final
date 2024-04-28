#pragma once

#include "presets.hpp"
#include "snake_node.hpp"
#include "snake_head.hpp"
#include "snake_tail.hpp"

class Snake {
    public:
        input_device dev;

        unsigned int length;

        unsigned int s_x, s_y;

        std::vector<snake_node> snake_body;
        snake_head head;
        snake_tail tail;

        Snake(unsigned int x = 30, unsigned int y = 15, unsigned int length = SNAKE_LENGTH);
        
        void reset_snake();
        void set_direction(dir h_dir, inc np, snake_node &prev);

        bool step_snake();

        unsigned char read_controller(bool game_state = true);


};

Snake::Snake(unsigned int x, unsigned int y, unsigned int length) :
    length(length), head(snake_head(0, 0, dir::VERT, inc::NEG)), tail(snake_tail(0, 0)) {

    if (grid_controller::check_coords(x,y)) {
        this->s_x = x;
        this->s_y = y;
    } else {
        this->s_x = 30;
        this->s_y = 15;
    }
    // initialize snake with all nodes in center and all sprites snake body...
    this->snake_body = std::vector<snake_node> (length, snake_node(sp::BODY, 0, 0));

    // set the snake in the middle of the map
    this->reset_snake();
    
    // link the portals
    }

// reset the snake to the initial conditions
void Snake::reset_snake() {
    // set the head
    this->head.move_node(this->s_x, this->s_y);

    // determine the direction 
    if (this->head.direction == dir::HORI) {
        // set the body
        for (unsigned int i = 0; i < this->length; i++) {
            this->snake_body[i].move_node(this->s_x - this->head.increment*(i + 1), this->s_y);
        }
        // set the tail
        this->tail.move_node(this->s_x - this->head.increment*(this->length + 1), this->s_y);
    } else {
        // set the body
        for (unsigned int i = 0; i < this->length; i++) {
            this->snake_body[i].move_node(this->s_x, this->s_y - this->head.increment*(i + 1));
        }
        // set the tail
        this->tail.move_node(this->s_x, this->s_y - this->head.increment*(this->length + 1));
    }

}

void Snake::set_direction(dir h_dir, inc np, snake_node &prev) {
    this->head.set_direction(h_dir, np, prev);
}

bool Snake::step_snake() {
    unsigned int tail_x, tail_y; 
    // 1. get target tail coords
    tail_x = this->snake_body[this->length - 1].get_coords(0); // end of body x
    tail_y = this->snake_body[this->length - 1].get_coords(1); // end of body x
    unsigned int dir_x = this->snake_body[this->length - 2].get_coords(0); // end of body x
    unsigned int dir_y = this->snake_body[this->length - 2].get_coords(1); // end of body x

    unsigned int tar_x, tar_y; 
    // 2. update body locations
    for (int i = this->length - 1; i > 0; i--) {            // start at end of body 
        tar_x = this->snake_body[i - 1].get_coords(0);      // next node x
        tar_y = this->snake_body[i - 1].get_coords(1);      // next node y
        this->snake_body[i].set_coords(tar_x, tar_y);  // set coords   
    }
    tar_x = this->head.get_coords(0);   // head x
    tar_y = this->head.get_coords(1);   // head y
    this->snake_body[0].move_node(tar_x, tar_y, false, false);  // set joint location to head

    // 3. move head 
    // need to see if food was eaten to tell if we should add a segment
    unsigned int prev_food = this->head.food_eaten;

    bool ret = this->head.step_head();

    // if successful and a food was eaten, grow snake
    if (ret && (this->head.food_eaten > prev_food)) {
        this->length++;
        this->snake_body.push_back(snake_node(sp::BODY, tail_x, tail_y));
    } else {    // else, move the tail
        this->tail.move_tail(tail_x, tail_y, dir_x, dir_y);
    }

    return ret;
}

unsigned char Snake::read_controller(bool game_state) {
    // first, send over some info
    RGB test(0, 255, 0);
    unsigned char temp = uart::ps4_transfer(test, game_state);

    if (temp == 22) {
        this->set_direction(dir::VERT, inc::NEG, snake_body[0]);
    } else if (temp == 23) {
        this->set_direction(dir::VERT, inc::POS, snake_body[0]);
    } else if (temp == 21) {
        this->set_direction(dir::HORI, inc::NEG, snake_body[0]);
    } else if (temp == 20) {
        this->set_direction(dir::HORI, inc::POS, snake_body[0]);
    }

    else if (temp == 8) {  // assume portal1 shoot
        // first, compute destination
        unsigned int x, y;
        if (this->head.direction == HORI) {
            x = this->head.get_coords(0) + 5*this->head.increment;   // shoot 5 tiles out
            y = this->head.get_coords(1);
        } else {
            x = this->head.get_coords(0);
            y = this->head.get_coords(1) + 5*this->head.increment;   // shoot 5 tiles out
        } 
        this->head.sender.shoot_portal(x, y);
    } else if (temp == 7) {  // assume portal2 shoot
        // first, compute destination
        unsigned int x, y;
        if (this->head.direction == HORI) {
            x = this->head.get_coords(0) + 7*this->head.increment;   // shoot 7 tiles out
            y = this->head.get_coords(1);
        } else {
            x = this->head.get_coords(0);
            y = this->head.get_coords(1) + 7*this->head.increment;   // shoot 7 tiles out
        } 
        this->head.reciever.shoot_portal(x, y);
    }
    return temp;
}
