#pragma once

#include "presets.hpp"
#include "grid_controller.hpp"
#include "time_controller.hpp"

namespace food {
    unsigned int x = 0;
    unsigned int y = 0;
    unsigned int spawn_time = 0;
    unsigned int period = 10;
    unsigned int elapsed = 0;

    void randomize() {
        while (1) {
            food::x = MIN_X_COORD + rand()%(MAX_X_COORD - MIN_X_COORD + 1);
            food::y = MIN_Y_COORD + rand()%(MAX_Y_COORD - MIN_Y_COORD + 1);

            sp res = grid_controller::set_sprite(x, y, sp::FOOD, true);

            // will equal transparent if success
            if (res == sp::TRANSPARENT) {
                break;
            }
        } 

        food::spawn_time = Timer::GTC_get_time_ms();
        food::elapsed = 0;
    }   

    bool check_food() {
        if (Timer::GTC_get_time_ms() - food::spawn_time > food::period) {
            food::spawn_time = 0;
            food::elapsed++;
        }

        // if time elapsed, teleport
        if (elapsed > 800) {
            // clear food
            grid_controller::set_sprite(x, y, sp::TRANSPARENT);    
            audio::play_audio(clip::PORTAL_TRAVEL);
            // move
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
