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

        void delete_portal();
};

bool portal::shoot_portal(unsigned int tar_x, unsigned int tar_y) {
    if (grid_controller::check_coords(tar_x, tar_y) == false) {
        return false;
    }
    // if already set, clear
    if (this->set) {
        this->delete_portal();
    }

    sp set_res = grid_controller::set_sprite(tar_x, tar_y, this->type, true);
    this->set = (set_res == sp::TRANSPARENT);

    // if successful, update coords
    if (this->set) {
        this->x = tar_x;
        this->y = tar_y;
    }

    return this->set;
}

void portal::set_corresponding(portal* cp) {
    this->correspoding_portal = cp;
}


void portal::delete_portal() {
    grid_controller::set_sprite(this->x, this->y, sp::TRANSPARENT);
}