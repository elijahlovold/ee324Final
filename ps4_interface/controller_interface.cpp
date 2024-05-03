#include <iostream>
#include <thread> // For std::this_thread::sleep_for
#include "usbTools.hpp"
#include "ps4Tools.hpp"
#include <chrono>

using namespace std::chrono;

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
    bRGB p1_color(255,255,255);
    bRGB p2_color(255,0,0);

    // setup connection to board
    USBManager blk_brd_handle;
    if (blk_brd_handle.AutoConnect() == false) {
        return 0;
    }

    // clear any existing data in the FIFO buffer
    blk_brd_handle.ClearInputBuffer();
     
    unsigned char p1_write = 0;
    unsigned char p2_write = 0;
    blk_brd_handle.write_raw(&p1_write, 1);
    blk_brd_handle.write_raw(&p2_write, 1);

    unsigned char p1_read[8];
    unsigned char p2_read[8];
    memset(p1_read, 0, 8);
    memset(p2_read, 0, 8);
    blk_brd_handle.read_raw(p1_read);
    blk_brd_handle.read_raw(p2_read);

    for (int i = 0; i < 8; i++) {
        std::cerr << (int)(p1_read[i]) << std::endl;
    }

    while (1) {
        int num;
        do {
            num = JslConnectDevices();
            std::cout << "number of devices: " << num << std::endl; 
            usleep(500000);
        } while (num < 2);

        int handle[2];
        JslGetConnectedDeviceHandles(handle, 2);
        
        int p1_handle = handle[0];
        int p2_handle = handle[1];

        unsigned char p1_prev_cmd, p1_cmd;
        unsigned char p2_prev_cmd, p2_cmd;

        input_mode p1_mode = STICKS;
        input_mode p2_mode = STICKS;

        bool p1_alive = false;
        bool p2_alive = false;

        auto p1_s_rumble = steady_clock::now();
        bool p1_rumble = false;
        auto p2_s_rumble = steady_clock::now();
        bool p2_rumble = false;

        auto rumble_length = seconds(4);

        while(1) {
            switch (p1_mode) {
                case STICKS:
                    p1_cmd = decode_sticks(p1_handle, p1_mode, &p1_prev_cmd);
                    break;
                case GYRO:
                    p1_cmd = decode_gyro(p1_handle, p1_mode, &p1_prev_cmd);
                    break;
                case PAD: 
                    p1_cmd = decode_sticks(p1_handle, p1_mode, &p1_prev_cmd);
                    break;
            }
            if (p1_cmd == p1_prev_cmd) {
                p1_write = 0;
            } else {
                p1_write = p1_cmd;
                p1_prev_cmd = p1_cmd;
            }

            switch (p2_mode) {
                case STICKS:
                    p2_cmd = decode_sticks(p2_handle, p2_mode, &p2_prev_cmd);
                    break;
                case GYRO:
                    p2_cmd = decode_gyro(p2_handle, p2_mode, &p2_prev_cmd);
                    break;
                case PAD: 
                    p2_cmd = decode_sticks(p2_handle, p2_mode, &p2_prev_cmd);
                    break;
            }
            if (p2_cmd == p2_prev_cmd) {
                p2_write = 0;
            } else {
                p2_write = p2_cmd;
                p2_prev_cmd = p2_cmd;
            }


            blk_brd_handle.write_raw(&p1_write, 1);
            blk_brd_handle.write_raw(&p2_write, 1);

            blk_brd_handle.read_raw(p1_read);
            blk_brd_handle.read_raw(p2_read);

            // handle colors
            bRGB new_color(p1_read[0], p1_read[1], p1_read[2]);
            if (comp(new_color, p1_color) == false) {
                p1_color = new_color;
                JslSetLightColour(p1_handle, rgb_to_hex(p1_color));
            }

            // // if game over, set rumble
            // if (p1_alive && p1_read[3] == 0) {
            //     JslSetRumble(p1_handle, 255, 255);
            //     p1_s_rumble = steady_clock::now();
            //     p1_rumble = true;
            //     p1_alive = false;
            // } else if (p1_read[3] == 1) {
            //     p1_alive = true;
            //     JslSetRumble(p1_handle, 0, 0);
            //     p1_rumble = false;
            // }
            // if (p1_rumble && (steady_clock::now() - p1_s_rumble >= rumble_length)) {
            //     JslSetRumble(p1_handle, 0, 0);
            //     p1_rumble = false;
            // }
            if (p1_read[3] == 0) {
                JslSetRumble(p1_handle, 255, 255);
            } else {
                JslSetRumble(p1_handle, 0, 0);
            }

            // handle colors
            bRGB p2_new_color(p2_read[0], p2_read[1], p2_read[2]);
            if (comp(p2_new_color, p2_color) == false) {
                p2_color = p2_new_color;
                JslSetLightColour(p2_handle, rgb_to_hex(p2_color));
            }

            // // if game over, set rumble
            // if (p2_alive && p2_read[3] == 0) {
            //     JslSetRumble(p2_handle, 255, 255);
            //     p2_s_rumble = steady_clock::now();
            //     p2_rumble = true;
            //     p2_alive = false;
            // } else if (p2_read[3] == 1) {
            //     p2_alive = true;
            //     JslSetRumble(p2_handle, 0, 0);
            //     p2_rumble = false;        
            // }
            // if (p2_rumble && (steady_clock::now() - p2_s_rumble >= rumble_length)) {
            //     JslSetRumble(p2_handle, 0, 0);
            //     p2_rumble = false;
            // }
            // if game over, set rumble
            if (p2_read[3] == 0) {
                JslSetRumble(p2_handle, 255, 255);
            } else {
                JslSetRumble(p2_handle, 0, 0);
            }


            usleep(1000);
            if (JslStillConnected(p1_handle) == false) {
                std::cerr << "warning, controller 1 DC'd!" << std::endl;
                break;
            } 
            if (JslStillConnected(p2_handle) == false) {
                std::cerr << "warning, controller 2 DC'd!" << std::endl;
                break;
            }
            if (blk_brd_handle.IsConnected() == false) {
                std::cerr << "warning, black board DC'd!" << std::endl;
                break;
            }
        }

        // call this at the end to disconnect all devices
        JslDisconnectAndDisposeAll();
    }
    return 0;
}
