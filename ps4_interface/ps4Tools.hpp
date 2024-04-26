#pragma once

#include "JoyShockLibrary.h"
#include <unistd.h>
#include <iomanip>
#include <iostream>

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

void read_gyro(int controller_id) {
    std::cout << "aX: " << std::setw(7) << JslGetAccelX(controller_id);
    std::cout << "    aY: " << std::setw(7) << JslGetAccelY(controller_id);
    std::cout << "    aZ: " << std::setw(7) << JslGetAccelZ(controller_id);
    std::cout << "    gX: " << std::setw(7) << JslGetGyroX(controller_id);
    std::cout << "    gY: " << std::setw(7) << JslGetGyroY(controller_id);
    std::cout << "    gZ: " << std::setw(7) << JslGetGyroZ(controller_id) << std::endl;
}

void read_touch(int controller_id) {

    std::cout << "touch: " << JslGetTouchDown(controller_id);
    std::cout << "    tx: " << std::setw(7) << JslGetTouchX(controller_id);
    std::cout << "    ty: " << std::setw(7) << JslGetTouchY(controller_id) << std::endl;
}


