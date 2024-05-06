#pragma once

#include "presets.hpp"
#include "snake_node.hpp"
#include "snake_head.hpp"
#include "snake_tail.hpp"
#include "colors.hpp"

// snake player class
class Snake {
    public:
        // sending portal range
        unsigned int range;

        // player instance
        int instance;
        // life status
        bool alive;

        // input device - currently unimplemented
        input_device dev;

        // snake length = snake_body.size()
        unsigned int length;

        // initial x and y
        unsigned int s_x, s_y;

        // body color index
        unsigned int body_color_i;

        // vector of snake_node makes the body
        std::vector<snake_node> snake_body;
        snake_head head;
        snake_tail tail;

        // constructor
        Snake(unsigned int x = 30, unsigned int y = 15, unsigned int length = SNAKE_LENGTH, int instance = 0);

        // helpful functions
        void reset_snake();
        void kill_snake();
        void clear_snake();

        void set_direction(dir h_dir, inc np, snake_node &prev);

        bool step_snake();

        void ps4_write();
        void decode_inputs(unsigned char data);

        void set_color();
        void inc_color();
        void dec_color();


};

Snake::Snake(unsigned int x, unsigned int y, unsigned int length, int instance) :
    range(5), alive(false), length(length), body_color_i(7), head(snake_head(dir::VERT, inc::NEG)), tail(snake_tail()) {

    // grab current 0,0 since it's default location of node initialization
    // ...will have to reset after building snake
    sp temp = grid_controller::get_sprite(0, 0);

    // set the instance
    this->instance = instance;

    // set the color
    this->set_color();

    // check if valid coords
    if (grid_controller::check_coords(x,y)) {
        this->s_x = x;
        this->s_y = y;
    } else {
        this->s_x = 30;
        this->s_y = 15;
    }

    // initialize snake with all nodes in center and all sprites snake body...
    this->snake_body = std::vector<snake_node> (length, snake_node(sp::BODY));

    // replace what was initially there...
    grid_controller::set_sprite(0, 0, temp);
}

// kill the snake
void Snake::kill_snake() {
    this->alive = false;
    this->head.food_eaten = 0;
}

// clear the body
void Snake::clear_snake() {
    // loop through and clear each body node
    for (int i = 0; i < this->length; i++) {
        this->snake_body[i].clear_node();
    }

    // clear tail and head
    this->tail.clear_node();
    this->head.clear_node();
}

// reset the snake to the initial conditions
void Snake::reset_snake() {
    // first clear the snake
    this->clear_snake();
    
    // check which instance determines spawning location
    // later, move this to a preset vector which is selected by index...
    if (this->instance == 1) {
        this->head.direction = dir::HORI;
        this->head.increment = inc::POS;
        this->head.node_sprite = sp::HEAD_RIGHT;
        this->tail.node_sprite = sp::TAIL_RIGHT;
    } else if (this->instance == 2) {
        this->head.direction = dir::HORI;
        this->head.increment = inc::NEG;
        this->head.node_sprite = sp::HEAD_LEFT;
        this->tail.node_sprite = sp::HEAD_LEFT;
    }

    // set length
    this->length = SNAKE_LENGTH;
    sp temp = grid_controller::get_sprite(0, 0);

    // // initialize snake with all nodes in center and all sprites snake body...
    this->snake_body = std::vector<snake_node> (length, snake_node(sp::BODY));

    // replace what was initially there...
    grid_controller::set_sprite(0, 0, temp);

    // set alive
    this->alive = true;

    // set the head
    this->head.move_node(this->s_x, this->s_y, false, false);

    // determine the direction and move the nodes to correct location
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

// set the snake direction
// need to pass in body joint connecting to head to determine which direction of 
// ...head sprite to set
void Snake::set_direction(dir h_dir, inc np, snake_node &prev) {
    this->head.set_direction(h_dir, np, prev);
}

// step the snake
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

    // step the head
    bool step = this->head.step_head();
    
    // if successful and a food was eaten, grow snake
    if (this->head.food_eaten > prev_food) {
        this->length++;
        this->snake_body.push_back(snake_node(sp::BODY, tail_x, tail_y));
        this->snake_body[length-1].set_sprite(sp::BODY);
    } else {    // else, move the tail
        this->tail.move_tail(tail_x, tail_y, dir_x, dir_y);
    }

    // if unsuccessful, kill snake
    if (step == false) {
        this->kill_snake();
        return false;
    }
    return true;
}

// decode ps4 controller data
void Snake::decode_inputs(unsigned char data) {
    unsigned int x, y;
    if (this->alive) {
        // big switch for all the commands
        switch (data) {
            // change the direction
            case CMDS::UP: 
                this->set_direction(dir::VERT, inc::NEG, snake_body[0]);
                break;
            case CMDS::DOWN:
                this->set_direction(dir::VERT, inc::POS, snake_body[0]);
                break;
            case CMDS::LEFT:
                this->set_direction(dir::HORI, inc::NEG, snake_body[0]);
                break;
            case CMDS::RIGHT:
                this->set_direction(dir::HORI, inc::POS, snake_body[0]);
                break;

            // shoot a portal
            case CMDS::PORTAL1: 
                // first, compute destination
                if (this->head.direction == HORI) {
                    x = this->head.get_coords(0) + this->range*this->head.increment;   // shoot 5 tiles out
                    y = this->head.get_coords(1);
                } else {
                    x = this->head.get_coords(0);
                    y = this->head.get_coords(1) + this->range*this->head.increment;   // shoot 5 tiles out
                } 
                this->head.sender.shoot_portal(x, y);
                break;
            case CMDS::PORTAL2: 
                // first, compute destination
                if (this->head.direction == HORI) {
                    x = this->head.get_coords(0) + 7*this->head.increment;   // shoot 7 tiles out
                    y = this->head.get_coords(1);
                } else {
                    x = this->head.get_coords(0);
                    y = this->head.get_coords(1) + 7*this->head.increment;   // shoot 7 tiles out
                } 
                this->head.reciever.shoot_portal(x, y);
                break;

            // changed controller setting should ping player
            case 3:
                audio::play_audio(clip::PING);
                break;

            // change color
            case CMDS::INC_COLOR:
                this->inc_color(); 
                break;
            case CMDS::DEC_COLOR:
                this->dec_color(); 
                break;

            // change portal range
            case CMDS::RANGE: 
                if (this->range == 5) {
                    this->range = 2;
                } else {
                    this->range = 5;
                }
                audio::play_audio(clip::PING);
                break;

            default: 
                break;
        }
    } 
    // only valid if snake is dead 
    else if (data == CMDS::START) {
        this->reset_snake();
    } else if (data == CMDS::MINUS) {
        this->clear_snake();
    }
}

// write ps4 command to uart
void Snake::ps4_write() {
    uart::ps4_write(color_presets[this->body_color_i], true);
}

// set the snake color
void Snake::set_color() {
    colors::set_color(color::BODY_CORE, color_presets[this->body_color_i]); 
    io::RGB_led(color_presets[this->body_color_i], this->instance);
}

// increment color
void Snake::inc_color() {
    if (this->body_color_i == NUM_COLORS - 1) {
        this->body_color_i = 0;
    } else {
        this->body_color_i++;
    }
    this->set_color();
}

// decrement color
void Snake::dec_color() {
    if (this->body_color_i == 0) {
        this->body_color_i = NUM_COLORS - 1;
    } else {
        this->body_color_i--;
    }
    this->set_color();
}