#ifndef LEDS_H
#define LEDS_H

#include <stdint.h>


namespace LEDS {

    void init();

    void logic();


    enum class LEDMode {
        Off,
        On,
        Blink,
        Fade
    };

    enum class RGBMode {
        Off,
        Constant,
        Blink,
        Fade,
        Rainbow,
        Random
    };


    struct SimpleLED {
        const uint8_t pin;
        uint8_t brightness_level;
        LEDMode current_mode;
        bool fading_up;
        bool on = true;
        uint32_t blink_intervall;
        uint32_t fade_duration;
        uint32_t last_blink_time;
        uint32_t last_fade_time;
        
        SimpleLED(uint8_t pin, LEDMode mode, uint32_t blink_intervall = 500, uint32_t fade_duration = 10) : pin(pin) {
            current_mode = mode;
            this->blink_intervall = blink_intervall;
            this->fade_duration = fade_duration;
        }
    };

    
    struct RGBLED {
        /// Index 0 = red, 1 = green, 2 = blue
        const uint8_t PINS[3];
        uint8_t red;
        uint8_t green;
        uint8_t blue;
        uint8_t brightness_level;
        RGBMode current_mode;
        bool fading_up;
        bool on = true;
        uint32_t blink_intervall;
        uint32_t fade_duration;
        uint32_t random_change_intervall;
        uint32_t last_fade_time;
        uint32_t last_change_time;
        uint32_t last_blink_time;
        uint8_t target_red;
        uint8_t target_green;
        uint8_t target_blue;

        RGBLED(uint8_t red_pin, uint8_t green_pin, uint8_t blue_pin, uint8_t red, uint8_t green, uint8_t blue, RGBMode mode, uint32_t blink_intervall = 500, uint32_t fade_duration = 10, uint32_t random_change_intervall = 1000) 
            : PINS { red_pin, green_pin, blue_pin }
         {
            this->red = red;
            this->green = green;
            this->blue = blue;
            current_mode = mode;
            this->blink_intervall = blink_intervall;
            this->fade_duration = fade_duration;
            this->random_change_intervall = random_change_intervall;

        }
    };

    void turn_on(SimpleLED* led = nullptr, RGBLED* rgb_led = nullptr);

    void turn_off(SimpleLED* led = nullptr, RGBLED* rgb_led = nullptr);

    void quick_blink(SimpleLED* led = nullptr, RGBLED* rgb_led = nullptr);
    
    void change_color(uint8_t red, uint8_t green, uint8_t blue);
    
    void change_state(LEDMode new_mode, SimpleLED* led);

    void change_state(RGBMode new_mode, RGBLED* rgb_led);
    
    void change_fade_duration(uint32_t new_fade_duration, SimpleLED* led = nullptr, RGBLED* rgb_led = nullptr);
    
    extern SimpleLED orange_led;
    extern SimpleLED blue_led;
    extern RGBLED rgb_led;
}

#endif