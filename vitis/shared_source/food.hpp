#pragma once

#include "presets.hpp"
#include "grid_controller.hpp"

void spawn_food() {
    while (1) {
        unsigned int x = rand()%MAX_X_COORDS;
        unsigned int y = rand()%MAX_Y_COORDS;

        sp res = grid_controller::set_sprite(x, y, sp::FOOD, true);
        // will equal transparent if success
        if (res == sp::TRANSPARENT) {
            break;
        }
    } 
}