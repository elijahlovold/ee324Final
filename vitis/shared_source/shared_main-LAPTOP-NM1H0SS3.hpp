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
#include "basic_io.hpp"

int shared_main() {    

    // io::enable_all(1, PERIOD);    // enable RGBs, set period to PERIOD
	io::setup_SevenSeg(1);        // enable 7-seg 

    uart::setup_UART1();

    while (1) {

        // first, clear the grid
        grid_controller::clear_grid(sp::TRANSPARENT);

        // *((unsigned int *)(GRID_CONTROLLER_BASE_ADDR + 0*4)) = 0x22222044;
        *((unsigned int *)(GRID_CONTROLLER_BASE_ADDR + 0*4)) = 0x10000000;
        // *((unsigned int *)(GRID_CONTROLLER_BASE_ADDR_LOWER + 31*4)) = 0x00000002;
        // *((unsigned int *)(GRID_CONTROLLER_BASE_ADDR + 5*4)) = 0x00000002;
        // *((unsigned int *)(GRID_CONTROLLER_BASE_ADDR + 6*4)) = 0x00000002;

        // sp temp = sp::BODY;
        // grid_controller::set_sprite(30, 15, temp);
        // grid_controller::set_sprite(4, 16, temp);
        // grid_controller::set_sprite(4, 17, temp);

        // temp = sp::TAIL;
        // grid_controller::set_sprite(4, 18, temp);

        // first, instantiate a snake object
        // will create in the center of the screen
        // create the snake starting with default length
        Snake player1;

        // for (int i = 0; i < 10; i++) {
        //     usleep(1000000);
        //     player1.step_snake();
        // }
        // player1.set_direction(dir::HORI, inc::NEG);

        // for (int i = 0; i < 2; i++) {
        //     usleep(1000000);
        //     player1.step_snake();
        // }

        // player1.set_direction(dir::VERT, inc::NEG);
        // for (int i = 0; i < 10; i++) {
        //     usleep(1000000);
        //     player1.step_snake();
        //


        // create timer with 0.8s period
        // Timer input(0.8);

        bool game_play = true;
        
        // wait for controller to send over a byte before starting loop
        uart::read_char_UART1(); 

        while (game_play) {
            for (int i = 0; i < 8; i++) {
                uart::send_char_UART1(56);

            }

            usleep(50000);

            // check user inputs
            player1.read_inputs();

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

