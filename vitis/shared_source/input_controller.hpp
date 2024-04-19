#pragma once

#include "presets.hpp"

namespace input_controller {
    
    // // depricated... use check_input...
    // std::vector<bool> read_inputs_controller() {
    //     unsigned int command = read_raw_inputs();

    //     // make 32 for each bit
    //     std::vector<bool> commands (32);
        
    //     for (int i = 0; i < 32; i++) {
    //         // mask each bit and store in the vector
    //         commands[i] = 1 & (command >> i);
    //     }
    //     // cast to a valid input type and return
    //     return commands;
    // } 

    bool check_input(unsigned int commands, inputs cntr) {
        unsigned int index = static_cast<unsigned int>(cntr);
        unsigned int mask = 1 << index;
        
        return (commands & mask) != 0;
    }

    unsigned int read_raw_inputs() {
        return *((unsigned int *)(INPUT_CONTROLLER_BASE_ADDR));
    }
};