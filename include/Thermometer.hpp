#ifndef THERMOMETER_HPP
#define THERMOMETER_HPP

#include <Arduino.h>

namespace Thermometer {
    void update_thermometer();
    extern bool temperature_updated;
    extern int16_t temperature_celsius;
}


#endif