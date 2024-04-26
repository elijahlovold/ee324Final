#pragma once

#include "presets.hpp"
#include "input_controller.hpp"
#include "grid_controller.hpp"
#include "portal.hpp"
#include "basic_io.hpp"
#include "uart_controller.hpp"


class snake_node {
    public: 
        snake_node(sp sprite, unsigned int x, unsigned int y);

        bool set_coords(unsigned int x, unsigned int y);
        sp move_node(unsigned int x, unsigned int y, bool check_collision = false);
        void set_sprite(sp sprite);

        unsigned int get_coords(bool y) const;
        unsigned int get_sprite() const;

    protected: 
        dir_big node_dir;

        sp node_sprite;  // only need 8 bytes
        unsigned int coord_x;       // grid tile the node is on
        unsigned int coord_y;       // grid tile the node is on 
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

sp snake_node::move_node(unsigned int x, unsigned int y, bool check_collision) {
    // everytime we move the snake, we must first clear the current sprite
    grid_controller::set_sprite(this->coord_x, this->coord_y, sp::TRANSPARENT);

    if (x > MAX_X_COORDS || y > MAX_Y_COORDS) {
        io::output_to_SevenSeg(234);    // error indication
        return sp::ERROR;
    } else {
        this->coord_x = x;
        this->coord_y = y;
    }

    io::output_to_SevenSeg(this->coord_y);
    sp test = sp::BODY;
    // *((unsigned int *)(GRID_CONTROLLER_BASE_ADDR_LOWER + 5*4)) = 0x01010101;
    // *((unsigned int *)(GRID_CONTROLLER_BASE_ADDR_LOWER + 6*4)) = 0x01010101;
    // *((unsigned int *)(GRID_CONTROLLER_BASE_ADDR_LOWER + 7*4)) = 0x01010101;
    // grid_controller::set_sprite(4, 4, test);
    // grid_controller::set_sprite(4, 5, test);
    // grid_controller::set_sprite(4, 6, test);

    this->set_sprite(sp::BODY);
    // return grid_controller::set_sprite(this->coord_x, this->coord_y, this->node_sprite);
    // return grid_controller::set_sprite(this->coord_x, this->coord_y, this->node_sprite, check_collision);
    // return grid_controller::set_sprite(this->coord_x, this->coord_y, temp);
    return sp::TRANSPARENT;
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
    if (x > MAX_X_COORDS || y > MAX_Y_COORDS) {
        return;
    } else {
        this->coord_x = x;
        this->coord_y = y;
    }
    this->set_sprite(sprite);
}

// inherit from snake_node
class snake_head : public snake_node {
    public: 
        dir direction;
        inc increment;
        unsigned int food_eaten;

        snake_head(int x, int y, dir dir_in, inc inc_in) : snake_node(sp::HEAD, x, y), sender(sp::PORTALS), reciever(sp::PORTALR), 
                                                            food_eaten(0), direction(dir_in), increment(inc_in) {
            this->sender.set_corresponding(&reciever);
            this->reciever.set_corresponding(&sender);
        }

        portal sender;
        portal reciever; 
 
        void set_direction(dir h_dir, inc np) {
           this->direction = h_dir;
           this->increment = np; 
        }

        // returns true unless invalid collision
        bool step_head() {
            sp collided_sprite;
            // determine which direction to step in... 
            if (this->direction == dir::HORI) {
                return this->move_head(this->coord_x + this->increment, this->coord_y);
            } else {
                return this->move_head(this->coord_x, this->coord_y + this->increment);
            }
        }


        bool move_head(unsigned int x, unsigned int y) {
            sp collided_sprite = this->move_node(x, y, true);

            switch (collided_sprite) {
                // if nothing, no collision
                case sp::TRANSPARENT: 
                    return true;

                // next, interactions
                case sp::PORTALS:
                    // handle portal mechanics
                    // grab the destination coords
                    unsigned int x, y;
                    x = this->reciever.x;
                    y = this->reciever.y;

                    // next, set the head past the destination portal
                    // if (this->direction == dir::HORI) {
                    //     this->move_node
                    // } else {

                    // }
                    return true;    // will probably depend on if other side is clear or not
                case sp::FOOD: 
                    // increment the food eaten tracker
                    this->food_eaten++;
                    // replace food with head
                    grid_controller::set_sprite(this->coord_x, this->coord_y, this->node_sprite);
                    return true;
                
                // default case is assumed to be a collision
                default: 
                    return false; 
            }
        }

};

// inherit from snake_node
class snake_tail : public snake_node {
    public: 
        snake_tail(int x, int y) : snake_node(sp::TAIL, x, y) {}
};





class Snake {
    public:
        unsigned int length;

        std::vector<snake_node> snake_body;
        snake_head head;
        snake_tail tail;

        Snake(unsigned int length = SNAKE_LENGTH);
        
        void reset_snake(int head_x = 4, int head_y = 15);
        void set_direction(dir h_dir, inc np);

        bool step_snake();
        void add_segment();

        void read_controller();


};

Snake::Snake(unsigned int length) : length(length), head(snake_head(0, 0, dir::VERT, inc::POS)), tail(snake_tail(0, 0)) {

    // initialize snake with all nodes in center and all sprites snake body...
    this->snake_body = std::vector<snake_node> (length, snake_node(sp::BODY, 0, 0));

    // set the snake in the middle of the map
    this->reset_snake();
    
    // link the portals
    }

// reset the snake to the initial conditions
void Snake::reset_snake(int head_x, int head_y) {
    // set the head
    this->head.move_node(head_x, head_y);

    // determine the direction 
    if (this->head.direction == dir::VERT) {
        // set the body
        for (unsigned int i = 0; i < this->length; i++) {
            this->snake_body[i].move_node(head_x + this->head.increment*(i + 1), head_y);
        }
        // set the tail
        this->tail.move_node(head_x + this->head.increment*(this->length + 1), head_y);
    } else {
        // set the body
        for (unsigned int i = 0; i < this->length; i++) {
            this->snake_body[i].move_node(head_x, head_y + this->head.increment*(i + 1));
        }
        // set the tail
        this->tail.move_node(head_x, head_y + this->head.increment*(this->length + 1));
    }

}

void Snake::set_direction(dir h_dir, inc np) {
    this->head.set_direction(h_dir, np);
}

bool Snake::step_snake() {
    unsigned int tar_x, tar_y; 

    // 1. clear tail
    this->tail.set_sprite(sp::TRANSPARENT);

    // 2. move tail to end of body 
    tar_x = this->snake_body[this->length - 1].get_coords(0); // end of body x
    tar_y = this->snake_body[this->length - 1].get_coords(1); // end of body x
    this->tail.move_node(tar_x, tar_y);

    // 3. update body locations
    for (int i = this->length - 1; i > 0; i--) {            // start at end of body 
        tar_x = this->snake_body[i - 1].get_coords(0);      // next node x
        tar_y = this->snake_body[i - 1].get_coords(1);      // next node y
        this->snake_body[i].set_coords(tar_x, tar_y);  // set coords   
    }
    tar_x = this->head.get_coords(0);   // head x
    tar_y = this->head.get_coords(1);   // head y
    this->snake_body[0].set_coords(tar_x, tar_y);  // set joint location to head

    // 4. update joint sprite before head
    // won't need if cube sprites...

    // 5. move head 
    return this->head.step_head();
}

void Snake::add_segment() {
    // // first figure out where to add the tail
    // unsigned int tail_x = this->snake[this->tail_i].get_coords(0);
    // unsigned int tail_y = this->snake[this->tail_i].get_coords(1);

    // unsigned int tail_1_x = this->snake[this->tail_i - 1].get_coords(0);
    // unsigned int tail_1_y = this->snake[this->tail_i - 1].get_coords(1);

    // int x_diff = tail_x - tail_1_x;
    // int y_diff = tail_y - tail_1_y;
    // // if diff in x, add along x direction
    // if (x_diff != 0) {
    //     this->snake.push_back(snake_node(sp::TAIL, tail_x + x_diff, tail_y));
    // } 
    // // else, add along y direction  
    // else {
    //     this->snake.push_back(snake_node(sp::TAIL, tail_x, tail_y + y_diff));
    // }

    // // now, update old tail sprite
    // this->snake[this->tail_i].set_sprite(sp::BODY);

    // // update length and tail index
    // this->length = this->snake.size();
    // this->tail_i = length - 1;
}

void Snake::read_controller() {
    // first, send over some info
    for (int i = 0; i < 8; i++) {
        uart::send_char_UART1(56);
    }
    // next, read back controller info byte
    unsigned char temp = uart::read_char_UART1(); 
    io::output_to_SevenSeg(temp);

    if (temp == 22) {
        this->set_direction(dir::VERT, inc::POS);
        this->step_snake();
    } else if (temp == 23) {
        this->set_direction(dir::VERT, inc::NEG);
        this->step_snake();
    } else if (temp == 21) {
        this->set_direction(dir::HORI, inc::POS);
        this->step_snake();
    } else if (temp == 20) {
        this->set_direction(dir::HORI, inc::NEG);
        this->step_snake();
    }

    else if (temp == 30) {  // assume portal1 shoot
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
    } else if (temp == 31) {  // assume portal2 shoot
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
}