#pragma once

#include "presets.hpp"
#include "grid_controller.hpp"
#include "audio_controller.hpp"

// class for creating sending and recieving portals
class portal {
    public: 
        // location
        unsigned int x;
        unsigned int y;

        // set flag
        bool set;

        // which network it's on - currently not needed
        int network;

        // type of portal
        sp type;

        // pointer to the corresponding portal
        portal* correspoding_portal;

        // constructor
        portal(sp t): type(t), set(false) {} 

        // helpful functions...
        bool shoot_portal(unsigned int tar_x, unsigned int tar_y);
        void set_corresponding(portal* cp);
        void delete_portal();
};

// shoot a portal to a location
bool portal::shoot_portal(unsigned int tar_x, unsigned int tar_y) {
    if (grid_controller::check_coords(tar_x, tar_y) == false) {
        return false;
    }

    // if already set, clear
    if (this->set) {
        this->delete_portal();
    }

    // set to the target
    sp set_res = grid_controller::set_sprite(tar_x, tar_y, this->type, true);
    // check if successful, update the set status
    this->set = (set_res == sp::TRANSPARENT);

    // if successful, update coords
    if (this->set) {
        this->x = tar_x;
        this->y = tar_y;
    }

    // play audio
    audio::play_audio(clip::PORTAL_PLACE);
    return this->set;
}

// set corresponding portal pointer
void portal::set_corresponding(portal* cp) {
    this->correspoding_portal = cp;
}

// delete the portal
void portal::delete_portal() {
    grid_controller::set_sprite(this->x, this->y, sp::TRANSPARENT);
}