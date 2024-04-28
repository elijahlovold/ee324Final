#pragma once

/************************************************************
* Description:  Draws a light ray with freq equal to color. *
*               Ray will bounce of walls.                   *
* Usages: demo and verify display hardware works            *
************************************************************/
#include "presets.hpp"
#include "input_controller.hpp"
#include "snake.hpp"
#include "grid_controller.hpp"
#include "time_controller.hpp"
#include "uart_controller.hpp"
#include "basic_io.hpp"
#include "food.hpp"
#include "audio_controller.hpp"

int shared_main() {    
    io::output_to_LEDs(0b111111);

    io::enable_all(1, PERIOD);    // enable RGBs, set period to PERIOD
    io::RGB(0, 120, 0, 0);
    io::RGB(120, 0, 0, 1);

	io::setup_SevenSeg();        // enable 7-seg 

    Timer::GTC_enable();
    Timer::GTC_set_period(500);    // set 1000ms period

    uart::setup_UART1();    // computer comms for ps4 controller

    while (1) {
        // first, clear the grid
        grid_controller::clear_grid();
        
        // *((unsigned int *)(GRID_CONTROLLER_BASE_ADDR + 7*4)) = 0x00000002;
        // *((unsigned int *)(GRID_CONTROLLER_BASE_ADDR + 8*4)) = 0x22222222;
        // *((unsigned int *)(GRID_CONTROLLER_BASE_ADDR + 5*4)) = 0x00000002;
        // *((unsigned int *)(GRID_CONTROLLER_BASE_ADDR + 6*4)) = 0x00000002;

        grid_controller::set_sprite(9, 15, sp::WALL);
        grid_controller::set_sprite(9, 16, sp::WALL);
        grid_controller::set_sprite(9, 17, sp::WALL);
        grid_controller::set_sprite(9, 18, sp::WALL);


        // first, instantiate a snake object
        // will create in the center of the screen
        // create the snake starting with default length
        Snake player1;
        // Snake player2(20, 15);

        // wait for player to start game while seeding random generator
        io::setup_SevenSeg(3);  // custom mode
        // display "Strt" on seven-seg
        *((unsigned int *)SVN_SEG_DATA) = 0b00010010000001110010111100000111;

        unsigned int seed = 0;
        while (!io::get_button_states()) {
            seed++;
        }
        srand(seed);

        io::setup_SevenSeg();   // back to normal mode
        io::output_to_SevenSeg(0);  // clear

        // spawn the initial fruit on the map
        spawn_food();

        bool game_pause = false;
        bool game_play = true;
        while (game_play) {
            usleep(100);
            player1.dev = static_cast<input_device>(io::get_switch(1));
            game_pause = io::get_switch(0);

            unsigned char data = controller::read_input(&player1);

            // if game update timer elapsed, increment player position
            // switch 1 must be high to run the game
            if (Timer::GTC_elapsed() && (game_pause != true)) {
                // display number of seconds played
                game_play = player1.step_snake();
            }
        }
        
        // end of game handler, display information...

        io::setup_SevenSeg(3);  // custom mode
        // display "LOSE" on seven-seg
        *((unsigned int *)SVN_SEG_DATA) = 0b11000111110000001001001010000110;
        usleep(3000000);
    }
    return 0;
}

