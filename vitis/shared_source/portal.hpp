#pragma once

#include "presets.hpp"
#include "grid_controller.hpp"

class portal {
    public: 
        unsigned int x;
        unsigned int y;

        bool set;

        int network;

        // portal_type type;
        sp type;

        portal* correspoding_portal;

        portal(sp t): type(t), set(false) {} 

        bool shoot_portal(unsigned int tar_x, unsigned int tar_y);

        void set_corresponding(portal* cp);
};

bool portal::shoot_portal(unsigned int tar_x, unsigned int tar_y) {
    sp set_res = grid_controller::set_sprite(tar_x, tar_y, this->type, true);
    this->set = (set_res == sp::TRANSPARENT);
    return this->set;
}

void portal::set_corresponding(portal* cp) {
    this->correspoding_portal = cp;
}