#pragma once

#include "presets.hpp"

// controls snake colors
namespace colors {

    // sets one of the 8 accents to an RGB color
    void set_color(unsigned char accent, RGB rgb) {
        // convert to hex
        unsigned int hex_color = (rgb.R << 16) | (rgb.G << 8) | (rgb.B);
        // store
        *((unsigned int *)(COLOR_CONTROLLER_BASE_ADDR + accent*4)) = hex_color;
    }

    // sets default colors for everything
    void default_colors() {
        set_color(0, RGB(0,0,0));                           // TRANSPARENT
        set_color(color::BODY_CORE, RGB(0,255,0));          // core body
        set_color(color::PUPIL, RGB(0,0,0));                // pupil
        set_color(color::IRIS_APPLE, RGB(255,0,0));         // iris
        set_color(color::BODY_ACCENT_SEND, RGB(0,110,67));  // body accent
        set_color(5, RGB(0,210,234));                       // portal accent
        set_color(6, RGB(255,0,255));                       // portal accent
        set_color(color::RECIEVE, RGB(255,255,0));          // recieve portal 
    }
};