#pragma once

#include "presets.hpp"


class Timer {
    public:
        // // unsigned int time_period;
        // static bool time_state;

        // static bool timer_elapsed();
        // static void clear_timer_flag();

        // period in seconds theoretically...
        double period; 
        clock_t start_time;
        void start_counter();
        bool check_timer();

        Timer(double p): period(p) {
            this->start_counter();
        }
};


// bool Timer::timer_elapsed() {
//     time_state = *((unsigned int *)(TIMER_BASE_ADDR));
//     return time_state;
// }

// void Timer::clear_timer_flag() {
//     // clear the timer elapsed flag 
//     *((unsigned int *)(TIMER_BASE_ADDR)) = 0;
// }

void Timer::start_counter() {
    this->start_time = clock();
}

bool Timer::check_timer() {
    clock_t current_time = clock();

    double elapsedTime = static_cast<double>(current_time - this->start_time)/CLOCKS_PER_SEC;
    // if elapsed, reset and return
    if (elapsedTime > this->period) {
        this->start_time = current_time;
        return true;
    }
    return false;
}