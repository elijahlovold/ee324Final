#pragma once

#include "presets.hpp"

namespace colors {
    void set_color(unsigned char accent, RGB rgb) {
        unsigned int hex_color = (rgb.R << 16) | (rgb.G << 8) | (rgb.B);
        *((unsigned int *)(COLOR_CONTROLLER_BASE_ADDR + accent*4)) = hex_color;
    }

    void default_colors() {
        set_color(0, RGB(0,0,0));  // TRANSPARENT
        set_color(color::BODY_CORE, RGB(0,255,0));  // core body
        set_color(color::PUPIL, RGB(0,0,0));    // pupil
        set_color(color::IRIS_APPLE, RGB(255,0,0));    // iris
        set_color(color::BODY_ACCENT_SEND, RGB(0,110,67));
        set_color(5, RGB(0,210,234));
        set_color(6, RGB(255,0,255));
        set_color(color::RECIEVE, RGB(255,255,0));  // recieve portal 
    }
};