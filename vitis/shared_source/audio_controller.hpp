#pragma once

#include "presets.hpp"

namespace audio {

    void enable_audio(bool on = true) {
        *((unsigned int *)(AUDIO_CONTROLLER_BASE_ADDR + 4)) = on;
    }

    void play_audio(clip cl) {
        unsigned char audio = static_cast<unsigned char>(cl);

        // set desired clip
        *((unsigned int *)(AUDIO_CONTROLLER_BASE_ADDR)) = audio & 0b111;

        // toggle on/off to start 
        enable_audio();     // enable
        usleep(100);        // wait a tiny bit
        enable_audio(0);    // disable

    }
};
	// // Add user logic here
    // assign SOUNDCHOICE = slv_reg0[1:0];
    // assign PLAY = slv_reg1[0];
    // assign STOP = slv_reg2[0];
	// // User logic ends