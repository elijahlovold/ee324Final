#include <iostream>
#include <thread> // For std::this_thread::sleep_for
#include "usbTools.hpp"
#include "ps4Tools.hpp"

int color_presets[] = {0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00, 0xFF00FF, 0x00FFFF};

int main () {

// connect to the ps4 controller
    int num = JslConnectDevices();
    std::cout << "number of devices: " << num << std::endl; 

    int* handle = new int[num];
    JslGetConnectedDeviceHandles(handle, num);

    for (int i = 0; i < num; i++) {
        std::cout << "handle " << i << ": " << handle[i] << std::endl;
        JslSetLightColour(handle[i], color_presets[i]);
    }

    std::cout << std::fixed << std::setprecision(5);
    
    while(1) {
        read_gyro(handle[0]);
        read_gyro(handle[1]);
        usleep(300000);
    }

    delete[] handle;
     
}