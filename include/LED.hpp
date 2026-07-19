#ifndef LED_H
#define LED_H

#include <stdint.h>


namespace LED {

    void init();

    void logic();

    void turn_on(uint8_t red,uint8_t green,uint8_t blue);

    void turn_off();

    void quick_blink(uint8_t red,uint8_t green,uint8_t blue, uint32_t blink_duration);
    
    enum Mode {
        Off,
        Constant,
        Blink,
        Fade,
        Rainbow,
        Random
    };

    void orange_led(Mode mode);
    void blue_led(Mode mode);

    void change_color(uint8_t red,uint8_t green,uint8_t blue);
    void change_color(uint8_t red,uint8_t green,uint8_t blue, Mode mode);
    
    void change_state(Mode mode);
    
    extern Mode current_orange_led_mode;
    extern Mode current_blue_led_mode;
    extern Mode current_rgd_led_mode;
}

#endif