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
        bool game_pause = false;
        bool game_play = true;
        bool btn_pressed = false;

        // load the map selected by the switches...
        grid_controller::load_map(io::get_switch_states() & 0b111);
        
        // first, instantiate a snake object
        // will create in the center of the screen
        // create the snake starting with default length
        Snake player1 (13, 31);
        player1.head.direction = dir::HORI;
        player1.head.increment = inc::POS;
        player1.head.set_sprite(sp::HEAD_RIGHT);
        player1.tail.set_sprite(sp::TAIL_RIGHT);

        // always spawn in snake 1
        player1.reset_snake();

        Snake player2(58, 4);
        player2.head.direction = dir::HORI;
        player2.head.increment = inc::NEG;
        player2.head.set_sprite(sp::HEAD_LEFT);
        player2.tail.set_sprite(sp::TAIL_LEFT);
        // spawn in player 2 when joined...

        // wait for player to start game while seeding random generator
        io::setup_SevenSeg(3);  // custom mode
        // display "Strt" on seven-seg
        *((unsigned int *)SVN_SEG_DATA) = 0b00010010000001110010111100000111;

        unsigned int seed = 0;
        unsigned char data = 0;
        while (data != 14) {
            uart::ps4_write(game_color);
            uart::ps4_write(game_color);

            data = uart::ps4_read();
            uart::ps4_read();

            seed++;

            if (io::get_button(3)) {
                grid_controller::load_map(io::get_switch_states());

                player1.reset_snake();
                while (io::get_button(3) == 1) { 
                    usleep (100);
                }
            }
        }
        
        audio::play_audio(clip::PING);
        srand(seed);

        io::setup_SevenSeg();   // back to normal mode
        io::output_to_SevenSeg(0);  // clear

        // spawn the initial fruit on the map
        food::randomize();

        while (game_play) {
            usleep(100);

            player1.ps4_write();
            player2.ps4_write();
            unsigned char data1 = uart::ps4_read();
            unsigned char data2 = uart::ps4_read();
            // can only pause if alive...
            if ((player1.alive && data1 == CMDS::START) || (player2.alive && data2 == CMDS::START)) {
                game_pause = !game_pause;
                audio::play_audio(clip::PING);
            }

            player1.decode_inputs(data1);
            player2.decode_inputs(data2);

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
                if (Timer::GTC_elapsed()) {
                    // display number of seconds played
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
        uart::ps4_write(RGB(255,0,0), false);
        uart::ps4_write(RGB(255,0,0), false);
        uart::ps4_read();
        uart::ps4_read();
         
        // end of game handler, display information...
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

