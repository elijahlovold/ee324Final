#pragma once

#include "JoyShockLibrary.h"
#include <unistd.h>
#include <iomanip>
#include <iostream>

// enum for possible input modes
enum input_mode {
    STICKS = 0,
    GYRO = 1,
    PAD = 2, // not yet implemented
};

// all possible commands
enum CMDS {
    UP = 22,
    DOWN = 23, 
    RIGHT = 20, 
    LEFT = 21, 

    PORTAL1 = 8, 
    PORTAL2 = 7, 

    START = 14,
    MINUS = 15,

    RANGE = 16,

    INC_COLOR = 18,
    DEC_COLOR = 19
};

// debug read simple data
void read_simple(int controller_id) {
    JOY_SHOCK_STATE info = JslGetSimpleState(controller_id);

    std::cout << "Buttons: " << std::setw(5) << info.buttons;
    std::cout << "    L_trigger: " << std::setw(7) << info.lTrigger;
    std::cout << "    R_trigger: " << std::setw(7) << info.rTrigger;
    std::cout << "    R_Stick_X: " << std::setw(7) << info.stickRX;
    std::cout << "    R_Stick_Y: " << std::setw(7) << info.stickRY;
    std::cout << "    L_Stick_X: " << std::setw(7) << info.stickLX;
    std::cout << "    L_Stick_Y: " << std::setw(7) << info.stickLY << std::endl;
}

// debug read gyro data
void read_gyro(int controller_id) {
    std::cout << "aX: " << std::setw(7) << JslGetAccelX(controller_id);
    std::cout << "    aY: " << std::setw(7) << JslGetAccelY(controller_id);
    std::cout << "    aZ: " << std::setw(7) << JslGetAccelZ(controller_id);
    std::cout << "    gX: " << std::setw(7) << JslGetGyroX(controller_id);
    std::cout << "    gY: " << std::setw(7) << JslGetGyroY(controller_id);
    std::cout << "    gZ: " << std::setw(7) << JslGetGyroZ(controller_id) << std::endl;
}

// debug read touch data
void read_touch(int controller_id) {
    std::cout << "touch: " << JslGetTouchDown(controller_id);
    std::cout << "    tx: " << std::setw(7) << JslGetTouchX(controller_id);
    std::cout << "    ty: " << std::setw(7) << JslGetTouchY(controller_id) << std::endl;
}

// decode general data - used in all modes
unsigned char decode_general(int controller_id, input_mode &mode, unsigned char *prev_cmd) {
    JOY_SHOCK_STATE info = JslGetSimpleState(controller_id);

    // buttons 
    if ((info.buttons & JSMASK_S) != 0) {    // x button
       return 1;
    } else if ((info.buttons & JSMASK_W) != 0) {    // square button
        return 2;
    } else if ((info.buttons & JSMASK_E) != 0) {    // circle button 
        std::cout << int(*prev_cmd) << std::endl;
        if (*prev_cmd == 3) {
            return 3;
        } else {
            *prev_cmd = 3;

            if (mode == STICKS) {
                std::cout << "entering GYRO" << std::endl;
                mode = GYRO;
            } else {
                std::cout << "entering STICKS" << std::endl;
                mode = STICKS;
            }
            return 3;
        }
    } else if ((info.buttons & JSMASK_N) != 0) {    // tri button 
        return CMDS::RANGE;
    }

    // bumpers and triggers
    else if ((info.buttons & JSMASK_ZR) != 0) {          // right trigger
        return CMDS::PORTAL1;
    } else if ((info.buttons & JSMASK_ZL) != 0) {   // left trigger
        return CMDS::PORTAL2;
    } else if ((info.buttons & JSMASK_R) != 0) {    // right bumper
        return CMDS::INC_COLOR;
    } else if ((info.buttons & JSMASK_L) != 0) {    // left bumper
        return CMDS::DEC_COLOR;
    }
    else if ((info.buttons & JSMASK_OPTIONS) != 0) {    // options button
        return CMDS::START;
    } else if ((info.buttons & JSMASK_MINUS) != 0) {    // minus button
        return CMDS::MINUS;
    }
    return 0;
}

// decode stick movements
unsigned char decode_sticks(int controller_id, input_mode &mode, unsigned char *prev_cmd) {
    // always decode general controls
    unsigned char temp = decode_general(controller_id, mode, prev_cmd);
    if (temp != 0) {
        return temp;
    }
    
    JOY_SHOCK_STATE info = JslGetSimpleState(controller_id);
    
    // d-pad
    if ((info.buttons & JSMASK_LEFT) != 0) {    // left d-pad
        return CMDS::LEFT;
    } else if ((info.buttons & JSMASK_RIGHT) != 0) {    // right d-pad
        return CMDS::RIGHT;
    } else if ((info.buttons & JSMASK_UP) != 0) {    // up d-pad
        return CMDS::UP;
    } else if ((info.buttons & JSMASK_DOWN) != 0) {    // down d-pad
        return CMDS::DOWN;
    }         
    // control
    else if ((info.buttons & JSMASK_HOME) != 0) {    // home button
        return 13;
    } else if (JslGetTouchDown(controller_id)) {               // touchpad pressed
        return 16;
    }
    // check left stick 
    else if (info.stickLX > 0.3) {  // left stick right              
        return CMDS::RIGHT;
    } else if (info.stickLX < -0.3) {  // left stick left              
        return CMDS::LEFT;
    } else if (info.stickLY > 0.3) {  // left stick up              
        return CMDS::UP;
    } else if (info.stickLY < -0.3) {  // left stick down              
        return CMDS::DOWN;
    }     
    // check right stick 
    else if (info.stickRX > 0.3) {  // right stick right              
        return CMDS::RIGHT;
    } else if (info.stickRX < -0.3) {  // right stick left              
        return CMDS::LEFT;
    } else if (info.stickRY > 0.3) {  // right stick up              
        return CMDS::UP;
    } else if (info.stickRY < -0.3) {  // right stick down              
        return CMDS::DOWN;
    }     
    
    // nothing
    return 0;
}

// decodes the gyro data
unsigned char decode_gyro(int controller_id, input_mode &mode, unsigned char *prev_cmd) {
    // always check general controls
    unsigned char temp = decode_general(controller_id, mode, prev_cmd);
    if (temp != 0) {
        return temp;
    }  
  
    // returns values in range of [-1,1]
    double x = JslGetAccelX(controller_id);
    double z = JslGetAccelZ(controller_id);
    // decide if gyro is turned far enough 
    if (x > 0.3) {
        return CMDS::LEFT;
    } else if (x < -0.3) {
        return CMDS::RIGHT;
    } else if (z > 0.3) {
        return CMDS::UP;
    } else if (z < -0.3) {
        return CMDS::DOWN;
    }

    // nothing
    return 0;
}

// decode pad - not yet implemented
unsigned char decode_pad(int controller_id, input_mode &mode, unsigned char *prev_cmd) {
    // always check general controls
    unsigned char temp = decode_general(controller_id, mode, prev_cmd);
    if (temp != 0) {
        return temp;
    }

    // nothing
    return 0;
}