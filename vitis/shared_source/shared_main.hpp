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
    // Timer::GTC_set_period(1000);    // set 1000ms period
    Timer::GTC_set_period(400);    // set 1000ms period

    uart::setup_UART1();    // computer comms for ps4 controller

    while (1) {
        // first, clear the grid
        grid_controller::clear_grid(sp::TRANSPARENT);

        
        // *((unsigned int *)(GRID_CONTROLLER_BASE_ADDR + 7*4)) = 0x00000002;
        // *((unsigned int *)(GRID_CONTROLLER_BASE_ADDR + 8*4)) = 0x22222222;
        // *((unsigned int *)(GRID_CONTROLLER_BASE_ADDR + 5*4)) = 0x00000002;
        // *((unsigned int *)(GRID_CONTROLLER_BASE_ADDR + 6*4)) = 0x00000002;

        // grid_controller::set_sprite(4, 15, sp::BODY);
        // grid_controller::set_sprite(4, 16, sp::BODY);
        // grid_controller::set_sprite(4, 17, sp::BODY);


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
        bool game_play = true;
        
        // // wait for controller to send over a byte before starting loop
        // uart::read_char_UART1(); 

        int i = 0;
        while (game_play) {

            usleep(100);

            // output switches to a sprite for testing...
            unsigned int temp = io::get_switch_states();
            *((unsigned int *)(GRID_CONTROLLER_BASE_ADDR + 240*4)) = temp & 0xFF;

            // check user inputs
            player1.read_controller();

            // if game update timer elapsed, increment player position
            if (Timer::GTC_elapsed() && io::get_switch_states()) {
                // display number of seconds played
                io::output_to_SevenSeg(i);

                switch (io::get_switch_states()) {
                    case 0b0001:
                        player1.head.set_direction(dir::HORI, player1.head.increment);
                        break;
                    case 0b0010: 
                        player1.head.set_direction(dir::VERT, player1.head.increment);
                        break;
                }

                i++;
                game_play = player1.step_snake();
            }
        }
        
        // end of game handler, display information...
        io::output_to_SevenSeg(9999);
    }
    return 0;
}

