#pragma once

/************************************************************
* Description:  Draws a light ray with freq equal to color. *
*               Ray will bounce of walls.                   *
* Usages: demo and verify display hardware works            *
************************************************************/
#include "presets.hpp"
#include "snake.hpp"
#include "grid_controller.hpp"
#include "time_controller.hpp"
#include "uart_controller.hpp"

int shared_main() {    
    while (1) {
        // // first, clear the grid
        // grid_controller::clear_grid();

        for (unsigned int i = 0; i < 1296; i += 4) {
            *((unsigned int *)(GRID_CONTROLLER_BASE_ADDR + i)) = 0x00010101;
        }

        // first, instantiate a snake object
        // will create in the center of the screen
        // create the snake starting with default length
        // Snake player1;

        // // create timer with 0.8s period
        // Timer input(0.8);

        bool game_play = true;
        while (game_play) {
            usleep(1000);
            // // check user inputs
            // player1.read_inputs();

            // // if game update timer elapsed, increment player position
            // if (input.check_timer()) {
            //     // update the snake
            //     // if the player collided, end game
            //     game_play = player1.step_snake();
                
            // }
        }

        // end of game handler, display information...
    }
    return 0;
}

