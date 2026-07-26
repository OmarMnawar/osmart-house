#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>

namespace timer {
    void start();
    void stop();
    bool is_time_reached(uint32_t goal_time);
}


#endif