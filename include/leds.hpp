#ifndef LEDS_H
#define LEDS_H

#include <stdint.h>


namespace leds {

    void init();

    void logic();


    enum class led_mode {
        off,
        on,
        blink,
        fade
    };

    enum class rgb_mode {
        off,
        constant,
        blink,
        fade,
        random,
        remote
    };


    struct simple_led {
        const uint8_t pin;
        uint8_t brightness_level;
        led_mode current_mode;
        bool fading_up;
        bool on = true;
        bool manually_turned_on;
        bool manually_turned_off;
        uint32_t blink_intervall;
        uint32_t fade_duration;
        uint32_t last_blink_time;
        uint32_t last_fade_time;
        
        simple_led(uint8_t pin, led_mode mode, uint32_t blink_intervall = 500, uint32_t fade_duration = 10) : pin(pin) {
            current_mode = mode;
            this->blink_intervall = blink_intervall;
            this->fade_duration = fade_duration;
        }
    };

    
    struct rgb_led {
        /// Index 0 = red, 1 = green, 2 = blue
        const uint8_t PINS[3];

        uint8_t red;
        uint8_t green;
        uint8_t blue;
        uint8_t brightness_level;
        rgb_mode current_mode;
        bool fading_up;
        bool on = true;
        bool manually_turned_on;
        bool manually_turned_off;
        uint32_t blink_intervall;
        uint32_t fade_duration;
        uint32_t random_change_intervall;
        uint32_t last_fade_time;
        uint32_t last_change_time;
        uint32_t last_blink_time;
        uint8_t target_red;
        uint8_t target_green;
        uint8_t target_blue;

        rgb_led(uint8_t red, uint8_t green, uint8_t blue, uint8_t red_pin, uint8_t green_pin, uint8_t blue_pin, rgb_mode mode, uint32_t blink_intervall = 500, uint32_t fade_duration = 10, uint32_t random_change_intervall = 1000) 
            : PINS { red_pin, green_pin, blue_pin }
         {
            current_mode = mode;
            this->blink_intervall = blink_intervall;
            this->fade_duration = fade_duration;
            this->random_change_intervall = random_change_intervall;

        }
    };

    void reset_all();
    void change_color(uint8_t red, uint8_t green, uint8_t blue);
    
    extern simple_led white_led;
    extern simple_led orange_led;
    extern simple_led blue_led;
    extern rgb_led main_rgb_led;
}

#endif