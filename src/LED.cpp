#include <Arduino.h>
#include "LED.hpp"
#include "Inputs.hpp"
#include "Timer.hpp"
#include "digitalWriteFast.h"


namespace LED {

    Mode current_led_mode = Mode::Off;
    Mode current_orange_led_mode = Mode::Off;
    Mode current_blue_led_mode = Mode::Off;

    uint8_t target_red = 0;
    uint8_t target_green = 0;
    uint8_t target_blue = 0;
    
    
    uint32_t loop_speed = 0;
    uint32_t last_loop_time = 0;
    
    void write_color(uint8_t red, uint8_t green, uint8_t blue);
    void set_target_colors(uint8_t red, uint8_t green, uint8_t blue);
    void start_blinking();
    void start_fade();
    void start_rainbow();
    void start_random();


    void init() {
        for (uint8_t i = 0; i < sizeof(Pins::LED_PINS) / sizeof(Pins::LED_PINS[0]); i++) {
            pinMode(Pins::LED_PINS[i], OUTPUT);
        }

        pinMode(Pins::LED_LCD_ON_PIN, OUTPUT);
        pinMode(Pins::LED_LCD_OFF_PIN, OUTPUT);
    }

    void logic() {

        switch (current_led_mode) {

            case Mode::Off:
                // OM: We simply exit the method without changing the colors.
            return;

            case Mode::Constant:
                // OM: We simply exit the method without changing the colors.
            break;
            
            case Mode::Blink:
                start_blinking();
            break;

            case Mode::Fade:
                start_fade();
            break;

            case Mode::Rainbow:
                start_rainbow();
            break;

            case Mode::Random:
                start_random();
            break;
        }
    }

    void blue_led(Mode mode) {

        // OM ToDo: Add more LEDs modes for the orange and blue.

        if (mode == Mode::Constant) {
            digitalWrite(Pins::LED_LCD_ON_PIN, HIGH);
        } else {
            digitalWrite(Pins::LED_LCD_ON_PIN, LOW);
        }
    }

    void orange_led(Mode mode) {
        // OM ToDo: Add more LEDs modes for the orange and blue.

        if (mode == Mode::Constant) {
            digitalWrite(Pins::LED_LCD_OFF_PIN, HIGH);
        } else {
            digitalWrite(Pins::LED_LCD_OFF_PIN, LOW);
        }
    }

    void turn_on(uint8_t red,uint8_t green,uint8_t blue) {
        if (current_led_mode != Mode::Off) {
            return;
        }
        write_color(red, green, blue);
        set_target_colors(red, green, blue);
        current_led_mode = Mode::Constant;
    }
    
    void turn_off() {
        write_color(0, 0, 0);
        current_led_mode = Mode::Off;
    }

    void quick_blink(uint8_t red, uint8_t green, uint8_t blue, uint32_t blink_speed) {
        Timer::start();
        write_color(red, green, blue);
        if (Timer::is_time_reached(blink_speed) == false) {
            write_color(0, 0, 0);
        }
        Timer::stop();
        set_target_colors(red, green, blue);
    }

    void change_color(uint8_t red, uint8_t green, uint8_t blue) {
        if (current_led_mode != Mode::Constant && current_led_mode != Mode::Blink && current_led_mode != Mode::Fade) {
            return;
        }
        write_color(red, green, blue);
        set_target_colors(red, green, blue);
    }

    void change_color(uint8_t red, uint8_t green, uint8_t blue, Mode mode) {
        write_color(red, green, blue);
        set_target_colors(red, green, blue);
        current_led_mode = mode;
    }
    
    
    void change_state(Mode mode) {
        current_led_mode = mode;
    }


    void start_blinking() {
        uint32_t blink_speed = 50;
        Timer::start();
        bool blink_visible = true;
        
        if (Timer::is_time_reached(blink_speed) == false) {
            blink_visible = !blink_visible;

            if (blink_visible == true) {
                write_color(target_red, target_green, target_blue);
            } else {
                write_color(0, 0, 0);
            }
        }
        Timer::stop();
    }


    void start_fade() {
        uint32_t fade_duration = 1000;
        Timer::start();


    }

    void start_rainbow() {
        
    }


    void start_random() {
        
    }
    
    void write_color(uint8_t red, uint8_t green, uint8_t blue) {
        analogWrite(Pins::LED_PINS[0], red);
        analogWrite(Pins::LED_PINS[1], green);
        analogWrite(Pins::LED_PINS[2], blue);
    }

    void set_target_colors(uint8_t red, uint8_t green, uint8_t blue) {
        target_red = red;
        target_green = green;
        target_blue = blue;
    }
}