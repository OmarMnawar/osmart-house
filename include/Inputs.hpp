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
    constexpr uint8_t LED_LCD_ON_PIN = 7;
    constexpr uint8_t LED_LCD_OFF_PIN = 4;
}

#endif