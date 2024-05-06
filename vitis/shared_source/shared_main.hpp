#pragma once

/************************************************************
* Description:  game of multiplayer snake with portals      *
* Usages: for fun times                                     *
************************************************************/

// include all our files
#include "presets.hpp"
#include "snake.hpp"
#include "grid_controller.hpp"
#include "time_controller.hpp"
#include "uart_controller.hpp"
#include "basic_io.hpp"
#include "food.hpp"
#include "audio_controller.hpp"
#include "colors.hpp"

// make a main file in vitis project and call this function
int shared_main() {    

    // set the leds on
    io::output_to_LEDs(0b111111);

    // enable a bunch of stuff

    io::enable_all(1, PERIOD);    // enable RGBs, set period to PERIOD
    io::RGB_led(RGB(0,120,0), 0);
    io::RGB_led(RGB(120,0,0), 1);

	io::setup_SevenSeg();        // enable 7-seg 

    Timer::GTC_enable();
    colors::default_colors();

    audio::enable_audio(0);

    uart::setup_UART1();    // computer comms for ps4 controller

    // set the default game color
    RGB game_color(255, 255, 255);

    float period = 500;
    Timer::GTC_set_period((unsigned int)(period));    // set 1000ms period

    // main game loop
    while (1) {
        // game status 
        bool game_pause = false;
        bool game_play = true;
        bool btn_pressed = false;

        // load the map selected by the switches...
        grid_controller::load_map(io::get_switch_states() & 0b111);
        
        // first, instantiate a snake object
        // will create in the center of the screen
        // create the snake starting with default length
        Snake player1 (13, 31, SNAKE_LENGTH, 1);
        // always spawn in snake 1
        player1.reset_snake();

        Snake player2(58, 4, SNAKE_LENGTH, 2);
       // spawn in player 2 when joined...

        io::setup_SevenSeg(3);  // custom mode
        // display "Strt" on seven-seg
        *((unsigned int *)SVN_SEG_DATA) = 0b00010010000001110010111100000111;

        // wait for player to start game while seeding random generator
        unsigned int seed = 0;
        unsigned char data = 0;
        while (data != 14) {
            // dummy transfer while waiting
            uart::ps4_write(game_color);
            uart::ps4_write(game_color);

            // read in response
            // only care about player 1 controller
            data = uart::ps4_read();
            uart::ps4_read();

            seed++;
            // if btn(3) is pressed, reload the map
            if (io::get_button(3)) {
                grid_controller::load_map(io::get_switch_states());

                player1.reset_snake();
                // btn release...
                while (io::get_button(3) == 1) { 
                    usleep (100);
                }
            }
        }
        
        // ping start of game
        audio::play_audio(clip::PING);
        srand(seed);
        // seed rand for food generation

        io::setup_SevenSeg();   // back to normal mode
        io::output_to_SevenSeg(0);  // clear

        // spawn the initial fruit on the map
        food::randomize();

        // game play loop
        while (game_play) {
            usleep(100);

            // transfer uart data
            player1.ps4_write();
            player2.ps4_write();
            unsigned char data1 = uart::ps4_read();
            unsigned char data2 = uart::ps4_read();

            // can only pause if alive...
            if ((player1.alive && data1 == CMDS::START) || (player2.alive && data2 == CMDS::START)) {
                game_pause = !game_pause;
                audio::play_audio(clip::PING);
            }

            // decode the inputs 
            player1.decode_inputs(data1);
            player2.decode_inputs(data2);

            // check if button was pressed to change game speed
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

            // if not paused...
            if (game_pause == false) {
                // check if food will teleport
                food::check_food(); 

                // if game update timer elapsed, increment player position
                if (Timer::GTC_elapsed()) {
                    if (player1.alive) {
                        player1.step_snake();
                    }
                    if (player2.alive) {
                        player2.step_snake();
                    }
                }
                
                // combine scores onto seven seg
                unsigned int combined = player1.head.food_eaten + 100*player2.head.food_eaten;
                io::output_to_SevenSeg(combined);
            }

            // keep playing as long as one is alive
            game_play = (player1.alive || player2.alive) && (io::get_button(3) != 1);
        }
        // end of game handler, display information...

        // tell controller to vibrate 
        uart::ps4_write(RGB(255,0,0), false);
        uart::ps4_write(RGB(255,0,0), false);
        uart::ps4_read();
        uart::ps4_read();
         
        // display lose info
        audio::play_audio(clip::GAME_OVER); // output none
        io::setup_SevenSeg(3);  // custom mode
        // display "LOSE" on seven-seg
        *((unsigned int *)SVN_SEG_DATA) = 0b11000111110000001001001010000110;
        usleep(3000000);

        uart::ps4_write(RGB(255,0,0), false);
        uart::ps4_write(RGB(255,0,0), false);
        uart::ps4_read();
        uart::ps4_read();
    }
    return 0;
}