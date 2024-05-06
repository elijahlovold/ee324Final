#pragma once

#include "presets.hpp"


namespace Timer {
    // period in ms
    void GTC_enable(unsigned int val = 0b111);
    void GTC_set_period(double period);
    bool GTC_elapsed(); 
    unsigned int GTC_get_time_ms(); 


    // set the period in ms
    void GTC_set_period(double period) {
        unsigned int gtc_period = static_cast<unsigned int>((period/3.0)*1000000);
        // store in GTC compare regs
        *((unsigned int *)(GTC_BASE_ADDR + 0x210)) = gtc_period;    // set lower 
        *((unsigned int *)(GTC_BASE_ADDR + 0x214)) = 0;             // clear upper 32 

        GTC_enable(0);                                              // disable
        *((unsigned int *)(GTC_BASE_ADDR + 0x20C)) = 1;             // clear event flag 
        *((unsigned int *)(GTC_BASE_ADDR + 0x200)) = 0;             // clear current counter lower
        *((unsigned int *)(GTC_BASE_ADDR + 0x204)) = 0;             // clear current counter upper
        GTC_enable();                                               // enable
    }

    // check if elapsed
    bool GTC_elapsed() {
        unsigned int flag = *((unsigned int *)(GTC_BASE_ADDR + 0x20C)); // grab interrupt flag
        if (flag != 0) {
            GTC_enable(0);                                              // disable
            *((unsigned int *)(GTC_BASE_ADDR + 0x20C)) = 1;             // clear event flag 
            *((unsigned int *)(GTC_BASE_ADDR + 0x200)) = 0;             // clear current counter lower 
            *((unsigned int *)(GTC_BASE_ADDR + 0x204)) = 0;             // clear current counter upper
            GTC_enable();                                               // enable
            return true;
        }
        return false;
    }

    // set enable
    void GTC_enable(unsigned int val) {
        // enable: interrupt, comparator, GTC
        *((unsigned int *)(GTC_BASE_ADDR + 0x208)) = val; 
    }

    // get time in ms
    unsigned int GTC_get_time_ms() {
        unsigned int lower = *((unsigned int *)(GTC_BASE_ADDR + 0x200));  // lower 32 bits of GTC
        return lower/333333;    // divide by 333*10^3 to get time in ms, base period is 3ns
    }

};