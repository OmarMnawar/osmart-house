#ifndef INPUTS_H
#define INPUTS_H
#include <Arduino.h>

namespace pins {
    constexpr uint8_t REMOTE = 11;    
    constexpr uint8_t THERMOMETER = A0;    
    constexpr uint8_t SENSOR = 8;
    constexpr uint8_t RGB_LED_RED = 10;
    constexpr uint8_t RGB_LED_GREEN = 9;
    constexpr uint8_t RGB_LED_BLUE = 6;
    constexpr uint8_t RGB_LEDS[3] = { RGB_LED_RED, RGB_LED_GREEN, RGB_LED_BLUE };
    constexpr uint8_t LED_BLUE = 5;
    constexpr uint8_t LED_ORANGE = 3;
    constexpr uint8_t LED_WHITE = 13;
    constexpr uint8_t SPEAKER = 2;
}

#endif