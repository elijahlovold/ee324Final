#pragma once

#include "presets.hpp"

// audio control functions
namespace audio {
    // controls if audio can be played or not
    void enable_audio(bool on = true) {
        *((unsigned int *)(AUDIO_CONTROLLER_BASE_ADDR + 4)) = on;
    }

    // play an audio clip
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