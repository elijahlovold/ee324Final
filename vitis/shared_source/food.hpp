#pragma once

#include "presets.hpp"
#include "grid_controller.hpp"
#include "time_controller.hpp"

// controls food object
// note, only one food on the map currently...
namespace food {
    unsigned int x = 0;
    unsigned int y = 0;
    unsigned int spawn_time = 0;
    unsigned int period = 10;
    unsigned int elapsed = 0;

    // function to randomize food position
    void randomize() {
        // try placing food until success
        while (1) {
            // get a random value between the max and min coords
            food::x = MIN_X_COORD + rand()%(MAX_X_COORD - MIN_X_COORD + 1);
            food::y = MIN_Y_COORD + rand()%(MAX_Y_COORD - MIN_Y_COORD + 1);

            // attempt to set
            sp res = grid_controller::set_sprite(x, y, sp::FOOD, true);

            // will equal transparent if success
            if (res == sp::TRANSPARENT) {
                break;
            }
        } 

        // update the spawn time
        food::spawn_time = Timer::GTC_get_time_ms();
        food::elapsed = 0;
    }   

    // check if food is expiring
    bool check_food() {
        // if time is greater than period, increment elapsed time
        // need to use this since GTC resets faster than food
        if (Timer::GTC_get_time_ms() - food::spawn_time > food::period) {
            food::spawn_time = 0;
            food::elapsed++;
        }

        // if time elapsed, teleport
        if (elapsed > 800) {
            // clear food
            grid_controller::set_sprite(x, y, sp::TRANSPARENT);    
            // move
            audio::play_audio(clip::PORTAL_TRAVEL);
            food::randomize();

            return true;
        }

        // if getting close, switch sprite
        if (elapsed > 600 && elapsed < 650) {
            grid_controller::set_sprite(x, y, sp::FOOD_TEL);    
        } else if (elapsed > 650 && elapsed < 700) {
            grid_controller::set_sprite(x, y, sp::FOOD);    
        } else if (elapsed > 700 && elapsed < 740) {
            grid_controller::set_sprite(x, y, sp::FOOD_TEL);    
        } else if (elapsed > 740 && elapsed < 770) {
            grid_controller::set_sprite(x, y, sp::FOOD);    
        } else if (elapsed > 770) {
            grid_controller::set_sprite(x, y, sp::FOOD_TEL);    
        }
        return false;
    }
};
