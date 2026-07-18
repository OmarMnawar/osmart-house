#include <Arduino.h>
#include "Timer.hpp"

namespace Timer {

    uint32_t start_time = 0;

    void start() {
        start_time = millis();
    }
    void stop(){
        start_time = 0;
    }

    bool is_time_reached(uint32_t goal_time) {

        

        return millis() - start_time >= goal_time;
    }
}