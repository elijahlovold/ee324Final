#include <iostream>
#include <thread> // For std::this_thread::sleep_for
#include "usbTools.hpp"
#include "ps4Tools.hpp"


int main() {

    // setup connection to board
    USBManager test;
    if (test.AutoConnect() == false) {
        return 0;
    }

    // clear any existing data in the FIFO buffer
    test.ClearInputBuffer();
     
    unsigned char write_array[8];
    write_array[0] = 4;
    test.write_raw(write_array, 1);

   
    // init read_array to zeros
    unsigned char read_array[8];
    memset(read_array, 0, 8);

    test.read_raw(read_array);

    for (int i = 0; i < 8; i++) {
        std::cerr << (int)(read_array[i]) << std::endl;
    }

    // unsigned char write_array[8];

    // first, connect to the ps4 controller
    std::cout << JslConnectDevices();
    int handle;
    JslGetConnectedDeviceHandles(&handle, 1);
    std::cout << handle << std::endl;

    // if (handle == 0) {
    //     std::cout << "no controller detected, aborting...\n";
    //     return 0;
    // } else {
    //     std::cout << "controller detected on port: !!\n";
    // }

    std::cout << std::fixed << std::setprecision(5);
    // set the hex color of the controller 
    JslSetLightColour(handle, 0xFF00FF);

    // set rumble to max
    // JslSetRumble(handle, 255, 255);
    // usleep(1200000);
    // JslSetRumble(handle, 0, 0);
    int i = 0;
    while(1) {
        // read_simple(controller_id);
        // read_gyro(controller_id);
        // read_touch(handle);

        JOY_SHOCK_STATE info = JslGetSimpleState(handle);
        // std::cout << info.stickLX << std::endl; 

        // bumpers and triggers
        if ((info.buttons & JSMASK_ZR) != 0) {          // right trigger
            write_array[0] = 8;
        } else if ((info.buttons & JSMASK_ZL) != 0) {   // left trigger
            write_array[0] = 7;
        } else if ((info.buttons & JSMASK_R) != 0) {    // right bumper
            write_array[0] = 6;
        } else if ((info.buttons & JSMASK_L) != 0) {    // left bumper
            write_array[0] = 5;
        } 
        // buttons 
        else if ((info.buttons & JSMASK_S) != 0) {    // x button
            write_array[0] = 1;
        } else if ((info.buttons & JSMASK_W) != 0) {    // square button
            write_array[0] = 2;
        } else if ((info.buttons & JSMASK_E) != 0) {    // circle button 
            write_array[0] = 3;
        } else if ((info.buttons & JSMASK_N) != 0) {    // tri button 
            write_array[0] = 4;
        } 
        // d-pad
        else if ((info.buttons & JSMASK_LEFT) != 0) {    // left d-pad
            write_array[0] = 9;
        } else if ((info.buttons & JSMASK_RIGHT) != 0) {    // right d-pad
            write_array[0] = 10;
        } else if ((info.buttons & JSMASK_UP) != 0) {    // up d-pad
            write_array[0] = 11;
        } else if ((info.buttons & JSMASK_DOWN) != 0) {    // down d-pad
            write_array[0] = 12;
        }         
        // control
        else if ((info.buttons & JSMASK_HOME) != 0) {    // home button
            write_array[0] = 13;
        } else if ((info.buttons & JSMASK_OPTIONS) != 0) {    // left bumper
            write_array[0] = 14;
        } else if ((info.buttons & JSMASK_MINUS) != 0) {    // left bumper
            write_array[0] = 15;
        } else if (JslGetTouchDown(handle)) {               // touchpad pressed
            write_array[0] = 16;
        }
        // check left stick 
        else if (info.stickLX > 0.3) {  // left stick right              
            write_array[0] = 20;
        } else if (info.stickLX < -0.3) {  // left stick left              
            write_array[0] = 21;
        } else if (info.stickLY > 0.3) {  // left stick up              
            write_array[0] = 22;
        } else if (info.stickLY < -0.3) {  // left stick down              
            write_array[0] = 23;
        }     
        // check right stick 
        else if (info.stickRX > 0.3) {  // right stick right              
            write_array[0] = 24;
        } else if (info.stickRX < -0.3) {  // right stick left              
            write_array[0] = 25;
        } else if (info.stickRY > 0.3) {  // right stick up              
            write_array[0] = 26;
        } else if (info.stickRY < -0.3) {  // right stick down              
            write_array[0] = 27;
        }     
        
        // nothing
        else {
            write_array[0] = 0;
        }

        write_array[1] = 0;

        test.write_raw(write_array, 2);

        test.read_raw(read_array);
        usleep(1000);
    }

    // call this at the end to disconnect all devices
    JslDisconnectAndDisposeAll();

    return 0;
}
