#pragma once

#include "presets.hpp"
#include "snake_node.hpp"
#include "audio_controller.hpp"

// inherit from snake_node
class snake_head : public snake_node {
    public: 
        dir direction;
        inc increment;
        unsigned int food_eaten;

        snake_head(dir dir_in, inc inc_in, int x = 0, int y = 0) : snake_node(sp::HEAD_UP, x, y), sender(sp::PORTALS), reciever(sp::PORTALR), 
                                                            food_eaten(0), direction(dir_in), increment(inc_in) {
            this->sender.set_corresponding(&reciever);
            this->reciever.set_corresponding(&sender);
        }

        portal sender;
        portal reciever; 
 
        void set_direction(dir h_dir, inc np, snake_node prev) {
            // no need if same axis
            if (this->direction == h_dir) {
                return; 
            }
            
            if (h_dir == dir::HORI) {
                if (np == inc::POS) {
                    if (prev.coord_x > this->coord_x) {
                        return; 
                    }
                    this->set_sprite(sp::HEAD_RIGHT);
                } else {
                    if (prev.coord_x < this->coord_x) {
                        return; 
                    }
                    this->set_sprite(sp::HEAD_LEFT);
                }
            } else {
                if (np == inc::POS) {
                    if (prev.coord_y > this->coord_y) {
                        return; 
                    }
                    this->set_sprite(sp::HEAD_DOWN);
                } else {
                    if (prev.coord_y < this->coord_y) {
                        return; 
                    }
                    this->set_sprite(sp::HEAD_UP);
                }
            }

            // if successful, update
            this->direction = h_dir;
            this->increment = np; 
        }

        // returns true unless invalid collision
        bool step_head() {
            // determine which direction to step in... 
            if (this->direction == dir::HORI) {
                return this->move_head(this->coord_x + this->increment, this->coord_y);
            } else {
                return this->move_head(this->coord_x, this->coord_y + this->increment);
            }
        }


        bool move_head(unsigned int x, unsigned int y) {
            sp collided_sprite = this->move_node(x, y, true, false);
            // sp collided_sprite = grid_controller::set_sprite(x, y, this->node_sprite, true);

            switch (collided_sprite) {
                // if nothing, no collision
                case sp::TRANSPARENT: {
                    return true;
                } 

                // next, interactions
                case sp::PORTALS: {
                    // first, see if reciever is set
                    // if not, clear 
                    if (this->reciever.set == false) {
                        grid_controller::set_sprite(this->coord_x, this->coord_y, this->node_sprite);
                        return true;
                    }

                    audio::play_audio(clip::PORTAL_TRAVEL);
                    // else, go through portal 
                    
                    // grab the destination coords of reciever
                    unsigned int x, y;
                    x = this->reciever.x;
                    y = this->reciever.y;

                    // compute the offset from the portal based off current head direction
                    if (this->direction == dir::HORI) {
                        x += this->increment;
                    } else {
                        y += this->increment;
                    }

                    // next, check if the output is a valid destination
                    sp dst_spt = grid_controller::get_sprite(x, y);

                    // three valid cases:
                    if (dst_spt == sp::TRANSPARENT || dst_spt == sp::FOOD || dst_spt == sp::FOOD_TEL) {
                        // send snake head to destination
                        this->move_node(x, y);
                        return true;
                    } else {    // else, false destination, snake dies
                        return false;
                    }
                }

                case sp::PORTALR: {
                    grid_controller::set_sprite(this->coord_x, this->coord_y, this->node_sprite);
                    return true;
                }

                case sp::FOOD: {
                    audio::play_audio(clip::CHOMP);  // output none

                    // increment the food eaten tracker
                    this->food_eaten++;
                    io::output_to_SevenSeg(this->food_eaten);

                    // replace food with head
                    grid_controller::set_sprite(this->coord_x, this->coord_y, this->node_sprite);
                    
                    // spawn a new food
                    food::randomize();
                    return true;
                }

                // same as above
                case sp::FOOD_TEL: {
                    audio::play_audio(clip::CHOMP);  // output none

                    // increment the food eaten tracker
                    this->food_eaten++;
                    io::output_to_SevenSeg(this->food_eaten);

                    // replace food with head
                    grid_controller::set_sprite(this->coord_x, this->coord_y, this->node_sprite);
                    
                    // spawn a new food
                    food::randomize();
                    return true;
                }

                // default case is assumed to be a collision
                default: {
                    return false; 
                }
            }
        }

};
