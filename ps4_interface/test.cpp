#include <iostream>
#include <thread> // For std::this_thread::sleep_for
#include "usbTools.hpp"
#include "ps4Tools.hpp"

struct bRGB {
    unsigned char R;
    unsigned char G;
    unsigned char B;

    bRGB(unsigned char r, unsigned char g, unsigned char b) : R(r), G(g), B(b) {}
};

bool comp(bRGB f, bRGB l) {
    if (f.R != l.R) return false;
    if (f.G != l.G) return false;
    if (f.B != l.B) return false;
    return true;
}

unsigned int rgb_to_hex(bRGB c) {
    return (c.R << 16) | (c.G << 8) | (c.B);
}

int main() {
    bRGB current_color(0,0,0);

    // setup connection to board
    USBManager test;
    if (test.AutoConnect() == false) {
        return 0;
    }

    // clear any existing data in the FIFO buffer
    test.ClearInputBuffer();
     
    unsigned char write_array[8];
    memset(write_array, 0, 8);
    test.write_raw(write_array, 2);

    unsigned char read_array[8];
    memset(read_array, 0, 8);
    test.read_raw(read_array);

    for (int i = 0; i < 8; i++) {
        std::cerr << (int)(read_array[i]) << std::endl;
    }

    // connect to the ps4 controller
    std::cout << JslConnectDevices();
    int handle;
    JslGetConnectedDeviceHandles(&handle, 1);

    std::cout << handle << std::endl;
    std::cout << std::fixed << std::setprecision(5);

    JslSetLightColour(handle, rgb_to_hex(current_color));
     
    input_mode mode = STICKS;
    unsigned char cmd;
    unsigned char prev_cmd = 0;

    while(1) {
        switch (mode) {
            case STICKS:
                cmd = decode_sticks(handle, mode);
                break;
            case GYRO:
                cmd = decode_gyro(handle, mode);
                break;
            case PAD: 
                // write_array[0] = decode_pad(handle, mode);
                cmd = decode_sticks(handle, mode);
                break;
        }

        // if previous cmd, skip
        if (cmd == prev_cmd) {
            write_array[0] = 0;
        } else {
            write_array[0] = cmd;
            prev_cmd = cmd;
        }

        // write_array[0] = decode_sticks(handle, mode);
        // dummy data
        write_array[1] = 0;

        test.write_raw(write_array, 2);

        test.read_raw(read_array);

        // handle colors
        bRGB new_color(read_array[0], read_array[1], read_array[2]);
        if (comp(new_color, current_color) == false) {
            current_color = new_color;
            JslSetLightColour(handle, rgb_to_hex(current_color));
        }

        // if game over, set rumble
        if (read_array[3] == 0) {
            JslSetRumble(handle, 255, 255);
        } else {
            JslSetRumble(handle, 0, 0);
        }

        usleep(1000);
    }

    // call this at the end to disconnect all devices
    JslDisconnectAndDisposeAll();

    return 0;
}
