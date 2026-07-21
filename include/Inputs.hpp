#ifndef INPUTS_H
#define INPUTS_H
#include <Arduino.h>
namespace Pins {
    constexpr uint8_t REMOTE_PIN = 11;    
    constexpr uint8_t THERMOMETER_PIN = A0;    
    constexpr uint8_t SENSOR_PIN = 8;
    constexpr uint8_t LED_RED_PIN = 10;
    constexpr uint8_t LED_GREEN_PIN = 9;
    constexpr uint8_t LED_BLUE_PIN = 6;
    constexpr uint8_t LED_PINS[3] = { LED_RED_PIN, LED_GREEN_PIN, LED_BLUE_PIN };
    constexpr uint8_t LED_BLUE_ON_PIN = 5;
    constexpr uint8_t LED_ORANGE_OFF_PIN = 3;
    constexpr uint8_t LED_WHITE_PIN = 13;
}

#endif