#include <Arduino.h>
#include <avr/interrupt.h>
#include "leds.hpp"
#include "inputs.hpp"
#include "remote.hpp"
#include "sensor.hpp"


namespace leds {
    simple_led orange_led = { pins::LED_ORANGE, led_mode::off };
    simple_led blue_led = { pins::LED_BLUE, led_mode::off };
    simple_led white_led = { pins::LED_WHITE, led_mode::off };
    rgb_led main_rgb_led = {0, 0, 0, pins::RGB_LED_RED, pins::RGB_LED_GREEN, pins::RGB_LED_BLUE, rgb_mode::remote };

    void write_color(uint8_t red, uint8_t green, uint8_t blue);
    void turn_on(simple_led* led = nullptr, rgb_led* rgb_led = nullptr);
    void turn_off(simple_led* led = nullptr, rgb_led* rgb_led = nullptr);
    void quick_blink(simple_led* led = nullptr, rgb_led* rgb_led = nullptr);
    void start_blinking(rgb_led* rgb_led = nullptr, simple_led* led = nullptr);
    void start_fade(rgb_led* rgb_led = nullptr, simple_led* led = nullptr);
    void start_random(rgb_led* rgb);
    void handle_led_mode(simple_led* led);
    void handle_led_mode(rgb_led* rgb);
    void reset_led(simple_led* led = nullptr, rgb_led* rgb_led = nullptr);
    void standard_rgb_color();

    void init() {
        pinMode(white_led.pin, OUTPUT);
        pinMode(blue_led.pin, OUTPUT);
        pinMode(orange_led.pin, OUTPUT);

        for (uint8_t i = 0; i < sizeof(main_rgb_led.PINS) / sizeof(main_rgb_led.PINS[0]); i++) {
            pinMode(main_rgb_led.PINS[i], OUTPUT);
        }
    }

    void logic() {

        handle_led_mode(&white_led);
        handle_led_mode(&orange_led);
        handle_led_mode(&blue_led);
        handle_led_mode(&main_rgb_led);

        if (white_led.manually_turned_on == false) {
            
            if (sensor::current_state == sensor::states::no_movement_detected) {
                turn_off(&white_led);
            }
        }
        if (white_led.manually_turned_off == false) {

            if (sensor::current_state == sensor::states::movement_detected) {
                turn_on(&white_led);
            }
        }
    }

    void turn_on(simple_led* led, rgb_led* rgb_led) {

        if (led == nullptr && rgb_led == nullptr) {
            return;
        }

        if (led != nullptr) {
            digitalWrite(led->pin, HIGH);
            led->current_mode = led_mode::on;
        }

        if (rgb_led != nullptr) {

            if (rgb_led->red == 0 && rgb_led->green == 0 && rgb_led->blue == 0) {
                standard_rgb_color();
            }
            else {
                write_color(rgb_led->red, rgb_led->green, rgb_led->blue);
            }
            rgb_led->current_mode = rgb_mode::constant;
        }
    }

    void turn_off(simple_led* led, rgb_led* rgb_led) {
        
        if (led == nullptr && rgb_led == nullptr) {
            return;
        }

        if (led != nullptr) {
            digitalWrite(led->pin, LOW);
            led->current_mode = led_mode::off;
        }

        if (rgb_led != nullptr) {
            write_color(0, 0, 0);
            rgb_led->current_mode = rgb_mode::off;
        }
    }

    void quick_blink(simple_led* led, rgb_led* rgb_led) {
        if (led == nullptr && rgb_led == nullptr) {
            return;
        }
        if (led != nullptr) {
            if (millis() - led->last_blink_time >= led->blink_intervall) {
                led->on = !led->on;
                
                if (led->on == true) {
                    digitalWrite(led->pin, HIGH);
                }
                else {
                    digitalWrite(led->pin, LOW);
                }
                led->last_blink_time = millis();
            }
        }

        if (rgb_led != nullptr) {
            if (millis() - rgb_led->last_blink_time >= rgb_led->blink_intervall) {
                rgb_led->on = !rgb_led->on;

                if (rgb_led->on == true) {
                    if (rgb_led->red == 0 && rgb_led->green == 0 && rgb_led->blue == 0) {
                        standard_rgb_color();
                    }
                    else {
                        write_color(rgb_led->red, rgb_led->green, rgb_led->blue);
                    }
                }
                else {
                    write_color(0, 0, 0);
                }
                rgb_led->last_blink_time = millis();
            }
        }
    }

    void handle_led_mode(simple_led* led) {

        switch (led->current_mode) {
            case led_mode::off:
                turn_off(led);
            return;

            case led_mode::on:
                turn_on(led);
            break;
            
            case led_mode::blink:
                start_blinking(nullptr, led);
            break;

            case led_mode::fade:
                start_fade(nullptr, led);
            break;
        }
    }

    void handle_led_mode(rgb_led* rgb_led) {

        switch (rgb_led->current_mode) {
            case rgb_mode::off:
                turn_off(nullptr, rgb_led);
            return;

            case rgb_mode::constant:
                turn_on(nullptr, rgb_led);
            break;
            
            case rgb_mode::blink:
                start_blinking(rgb_led);
            break;

            case rgb_mode::fade:
                start_fade(rgb_led);
            break;

            case rgb_mode::random:
                start_random(rgb_led);
            break;

            case rgb_mode::remote:
                if (remote::current_button != remote::buttons::none && rgb_led->current_mode == rgb_mode::remote) {
                    quick_blink(nullptr, rgb_led);
                }

                if (rgb_led->on == true) {
                    quick_blink(nullptr, rgb_led);
                }
            break;
        }
    }

    void start_blinking(rgb_led* rgb_led, simple_led* led) {

        if (rgb_led == nullptr && led == nullptr) {
            return;
        }

        quick_blink(led, rgb_led);
    }

 
    void start_fade(rgb_led* rgb_led, simple_led* led) {
        if (rgb_led == nullptr && led == nullptr) {
            return;
        }

        if (led != nullptr) {
            if (millis() - led->last_fade_time >= led->fade_duration) {
                if (led->fading_up == true) {
                    if (led->brightness_level >= 255) {
                        led->brightness_level = 255;
                        led->fading_up = false;
                    }
                    else {
                        led->brightness_level++;
                    }
                }
                else {
                    if (led->brightness_level <= 0) {
                        led->brightness_level = 0;
                        led->fading_up = true;
                    } 
                    else {
                        led->brightness_level--;
                    }
                }

                analogWrite(led->pin, led->brightness_level);

                led->last_fade_time = millis();
            }
        }

        if (rgb_led != nullptr) {
            if (millis() - rgb_led->last_fade_time >= rgb_led->fade_duration) {
                if (rgb_led->fading_up == true) {
                    if (rgb_led->brightness_level >= 255) {
                        rgb_led->brightness_level = 255;
                        rgb_led->fading_up = false;
                    }
                    else {
                        rgb_led->brightness_level++;
                    }
                }
                else {
                    if (rgb_led->brightness_level <= 0) {
                        rgb_led->brightness_level = 0;
                        rgb_led->fading_up = true;
                    } 
                    else {
                        rgb_led->brightness_level--;
                    }
                }

                if (rgb_led->red == 0 && rgb_led->green == 0 && rgb_led->blue) {
                    standard_rgb_color();
                }

                uint8_t scaled_red = (uint16_t)rgb_led->red * rgb_led->brightness_level / 255;
                uint8_t scaled_green = (uint16_t)rgb_led->green * rgb_led->brightness_level / 255;
                uint8_t scaled_blue = (uint16_t)rgb_led->blue * rgb_led->brightness_level / 255;
                
                write_color(scaled_red, scaled_green, scaled_blue);
                rgb_led->last_fade_time = millis();
            }

        }


    }


    void reset_all() {
        reset_led(&white_led);
        reset_led(&orange_led);
        reset_led(&blue_led);
        reset_led(nullptr, &main_rgb_led);
    }

    void reset_led(simple_led* led, rgb_led* rgb_led) {

        if (led == nullptr && rgb_led == nullptr) {
            return;
        }

        if (led != nullptr) {
            led->on = true;
            led->manually_turned_off = false;
            led->manually_turned_on = false;
            led->current_mode = led_mode::off;
            led->fade_duration = 30;
            led->blink_intervall = 500;
            led->brightness_level = 0;
            led->last_blink_time = 0;
            led->fading_up = 0;
        }

        if (rgb_led == nullptr) {
            rgb_led->on = true;
            rgb_led->manually_turned_off = false;
            rgb_led->manually_turned_on = false;
            rgb_led->current_mode = rgb_mode::off;
            rgb_led->fade_duration = 10;
            rgb_led->blink_intervall = 500;
            rgb_led->random_change_intervall = 1000;
            rgb_led->brightness_level = 0;
            rgb_led->last_blink_time = 0;
            rgb_led->fading_up = 0;
            standard_rgb_color();
        }
    }

    void start_random(rgb_led* rgb_led) {

        if (rgb_led == nullptr) {
            return;
        }

        if (rgb_led->red == 0 && rgb_led->green == 0 && rgb_led->blue == 0) {
            standard_rgb_color();
        }

        if (millis() - rgb_led->last_fade_time >= rgb_led->fade_duration) {
            rgb_led->target_red = random(0, 255);
            rgb_led->target_green = random(0, 255);
            rgb_led->target_blue = random(0, 255);
            write_color(rgb_led->target_red, rgb_led->target_green, rgb_led->target_blue);
            rgb_led->last_fade_time = millis();
        }
    }
    
    void write_color(uint8_t red, uint8_t green, uint8_t blue) {
        analogWrite(main_rgb_led.PINS[0], red);
        analogWrite(main_rgb_led.PINS[1], green);
        analogWrite(main_rgb_led.PINS[2], blue);
    }

    void change_color(uint8_t red, uint8_t green, uint8_t blue) {
        write_color(red, green, blue);
        main_rgb_led.red = red;
        main_rgb_led.green = green;
        main_rgb_led.blue = blue;
    }

    void standard_rgb_color() {
        change_color(255,0,255);
    }
}