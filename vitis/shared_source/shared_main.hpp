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
#include "colors.hpp"

int shared_main() {    

    io::output_to_LEDs(0b111111);

    io::enable_all(1, PERIOD);    // enable RGBs, set period to PERIOD
    io::RGB_led(RGB(0,120,0), 0);
    io::RGB_led(RGB(120,0,0), 1);

	io::setup_SevenSeg();        // enable 7-seg 

    Timer::GTC_enable();
    colors::default_colors();

    audio::enable_audio(0);

    uart::setup_UART1();    // computer comms for ps4 controller

    // set number of snakes to 0
    // Snake::num_instances = 0;

    RGB game_color(255, 255, 255);

    float period = 500;
    Timer::GTC_set_period((unsigned int)(period));    // set 1000ms period

    while (1) {

        // first, clear the grid
        grid_controller::clear_grid();
        
        // grid_controller::set_borders();

        // *((unsigned int *)(GRID_CONTROLLER_BASE_ADDR + 7*4)) = 0x00000002;
        // *((unsigned int *)(GRID_CONTROLLER_BASE_ADDR + 8*4)) = 0x22222222;
        // *((unsigned int *)(GRID_CONTROLLER_BASE_ADDR + 5*4)) = 0x00000002;
        // *((unsigned int *)(GRID_CONTROLLER_BASE_ADDR + 6*4)) = 0x00000002;

        // // add side walls
        // grid_controller::draw_wall_x(MIN_Y_COORD, 0, MAX_X_COORD);
        // grid_controller::draw_wall_x(MAX_Y_COORD, 0, MAX_X_COORD);
        // // center divider
        grid_controller::draw_wall_x(15, MIN_X_COORD, MAX_X_COORD);
        grid_controller::draw_wall_y((MAX_X_COORD - MIN_X_COORD)/2, MIN_Y_COORD, MAX_Y_COORD);
        grid_controller::draw_wall_y((MAX_X_COORD - MIN_X_COORD)/2 + 5, MIN_Y_COORD, MAX_Y_COORD);


        // first, instantiate a snake object
        // will create in the center of the screen
        // create the snake starting with default length
        Snake player1 (15, 15);
        // Snake player2(45, 15);

        // wait for player to start game while seeding random generator
        io::setup_SevenSeg(3);  // custom mode
        // display "Strt" on seven-seg
        *((unsigned int *)SVN_SEG_DATA) = 0b00010010000001110010111100000111;

        unsigned int seed = 0;
        // while (!io::get_button_states() && !player1.read_controller()) {
        while (!io::get_button_states() && (uart::ps4_transfer(game_color) != 14)) {
            seed++;
        }
        audio::play_audio(clip::PING); // output none
        srand(seed);

        io::setup_SevenSeg();   // back to normal mode
        io::output_to_SevenSeg(0);  // clear

        // spawn the initial fruit on the map
        food::randomize();

        bool game_pause = false;
        bool game_play = true;
        bool btn_pressed = false;
        while (game_play) {
            usleep(100);

            // unsigned char data = controller::read_input(&player1);
            unsigned char data = player1.read_controller(game_play);

            if (data == CMDS::START) {
                game_pause = !game_pause;
                audio::play_audio(clip::PING);
            }

            if (!btn_pressed) {
                if (io::get_button_states()&1) {
                period *= 0.8;
                Timer::GTC_set_period((unsigned int)(period));    // set 1000ms period
                btn_pressed = true;
                } else if (io::get_button_states()&0b10) {
                period *= 1.25;
                Timer::GTC_set_period((unsigned int)(period));    // set 1000ms period
                btn_pressed = true;
                }             
            } else if (io::get_button_states() == 0){
                btn_pressed = false;
            }

            if (game_pause == false) {
                food::check_food(); 

                // if game update timer elapsed, increment player position
                // switch 1 must be high to run the game
                if (Timer::GTC_elapsed()) {
                    // display number of seconds played
                    game_play = player1.step_snake();
                }
            }
        }
        
        // end of game handler, display information...
        uart::ps4_transfer(RGB(255,0,0), false);

        audio::play_audio(clip::GAME_OVER); // output none
        io::setup_SevenSeg(3);  // custom mode
        // display "LOSE" on seven-seg
        *((unsigned int *)SVN_SEG_DATA) = 0b11000111110000001001001010000110;
        usleep(3000000);
    }
    return 0;
}

