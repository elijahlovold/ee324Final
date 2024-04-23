#include <iostream>
#include <thread> // For std::this_thread::sleep_for
#include "JoyShockLibrary.h"
#include <unistd.h>
#include <iomanip>

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


int main() {
    std::cout << JslConnectDevices();

    int handles [4] = {0, 0, 0, 0};
    JslGetConnectedDeviceHandles(handles, 4);

    for (int i = 0; i < 4; i++) {
        std::cout << handles[i] << " "; 
    } 

    std::cout << std::endl;

    int controller_id = handles[0];  
    std::cout << controller_id << std::endl;
    if (controller_id == 0) {
        std::cout << "nothing detected, returning\n";
    }
            
    std::cout << std::fixed << std::setprecision(5);
    // set the hex color of the controller 
    JslSetLightColour(controller_id, 0xFF00FF);

    // set rumble to max
    JslSetRumble(controller_id, 255, 255);
    usleep(1200000);
    JslSetRumble(controller_id, 0, 0);

    while(1) {
        // read_simple(controller_id);
        // read_gyro(controller_id);
        read_touch(controller_id);

        usleep(100000);
    }

    // call this at the end to disconnect all devices
    JslDisconnectAndDisposeAll();
}
