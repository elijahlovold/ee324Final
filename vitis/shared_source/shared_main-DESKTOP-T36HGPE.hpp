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

    Timer::GTC_enable();
    Timer::GTC_set_period(1000);    // set 1000ms period

    uart::setup_UART1();    // computer comms for ps4 controller

    while (1) {
        // first, clear the grid
        grid_controller::clear_grid(sp::TRANSPARENT);
        // for (int i = 0; i < 2; i++) {
        //     unsigned int val = 0;
        //     for (int j = 0; j < 8; j++) {
        //         val |= (2 << 4*j);
        //     }
        //     *((unsigned int *)(GRID_CONTROLLER_BASE_ADDR + i*4)) = val;
        // }

 
        
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
        // Snake player1;

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
        bool game_play = true;
        
        // // wait for controller to send over a byte before starting loop
        // uart::read_char_UART1(); 

        int i = 0;
        while (game_play) {
            // temp = io::get_switch_states();
            // io::output_to_SevenSeg(temp);
            usleep(100);
            unsigned int temp = io::get_switch_states();
            *((unsigned int *)(GRID_CONTROLLER_BASE_ADDR + 7*4)) = temp & 0xFF;

            
            // check user inputs
            // player1.read_inputs();

            // if game update timer elapsed, increment player position
            if (Timer::GTC_elapsed()) {
                io::output_to_SevenSeg(i);
                i++;
            //     // update the snake
            //     // if the player collided, end game
            //     game_play = player1.step_snake();
                
            }
        }

        // end of game handler, display information...
    }
    return 0;
}

