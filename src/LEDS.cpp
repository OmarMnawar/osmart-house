#include <Arduino.h>
#include <avr/interrupt.h>
#include "LEDS.hpp"
#include "Inputs.hpp"
#include "States.hpp"


namespace LEDS {
    SimpleLED orange_led = { Pins::LED_ORANGE_OFF_PIN, LEDMode::Off };
    SimpleLED blue_led = { Pins::LED_BLUE_ON_PIN, LEDMode::Off };
    SimpleLED white_led = { Pins::LED_WHITE_PIN, LEDMode::Off };
    RGBLED rgb_led = {Pins::LED_RED_PIN, Pins::LED_GREEN_PIN, Pins::LED_BLUE_PIN, RGBMode::Off };
    

    void turn_on(SimpleLED* led = nullptr, RGBLED* rgb_led = nullptr);
    void turn_off(SimpleLED* led = nullptr, RGBLED* rgb_led = nullptr);
    void quick_blink(SimpleLED* led = nullptr, RGBLED* rgb_led = nullptr);
    void start_blinking(RGBLED* rgb_led = nullptr, SimpleLED* led = nullptr);
    void start_fade(RGBLED* rgb_led = nullptr, SimpleLED* led = nullptr);
    void start_random(RGBLED* rgb_led);
    void handle_led_mode(SimpleLED* led);
    void handle_led_mode(RGBLED* rgb_led);
    void reset_led(SimpleLED* led = nullptr, RGBLED* rgb_led = nullptr);


    void init() {
        pinMode(white_led.pin, OUTPUT);
        pinMode(blue_led.pin, OUTPUT);
        pinMode(orange_led.pin, OUTPUT);

        for (uint8_t i = 0; i < sizeof(rgb_led.PINS) / sizeof(rgb_led.PINS[0]); i++) {
            pinMode(rgb_led.PINS[i], OUTPUT);
        }
    }

    void logic() {

        handle_led_mode(&white_led);
        handle_led_mode(&orange_led);
        handle_led_mode(&blue_led);
        handle_led_mode(&rgb_led);

        // OM ToDo: Come back to this later.

        if (Sensor::current_state == Sensor::States::NoMovementDetected) {
            if (white_led.manually_turned_on == false) {
                turn_off(&white_led);
            }
        }
        else if (Sensor::current_state == Sensor::States::MovementDetected) {
            if (white_led.manually_turned_off == false) {
                turn_on(&white_led);
            }
        }
    }

    void turn_on(SimpleLED* led, RGBLED* rgb_led) {

        if (led == nullptr && rgb_led == nullptr) {
            return;
        }

        if (led != nullptr) {
            digitalWrite(led->pin, HIGH);
            led->current_mode = LEDMode::On;
        }

        if (rgb_led != nullptr) {
            change_color(rgb_led->PINS[0], rgb_led->PINS[1], rgb_led->PINS[2]);
            rgb_led->current_mode = RGBMode::Constant;
        }
    }

    void turn_off(SimpleLED* led, RGBLED* rgb_led) {
        
        if (led == nullptr && rgb_led == nullptr) {
            return;
        }

        if (led != nullptr) {
            digitalWrite(led->pin, LOW);
            led->current_mode = LEDMode::Off;
        }

        if (rgb_led != nullptr) {
            change_color(0, 0, 0);
            rgb_led->current_mode = RGBMode::Off;
        }
    }

    void quick_blink(SimpleLED* led, RGBLED* rgb_led) {
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
                    change_color(rgb_led->PINS[0], rgb_led->PINS[1], rgb_led->PINS[2]);
                }
                else {
                    change_color(0, 0, 0);
                }
                rgb_led->last_blink_time = millis();
            }
        }
    }
    
    void change_state(LEDMode new_mode, SimpleLED* led) {

        if (led == nullptr) {
            return;
        }

        led->current_mode = new_mode;
    }

    void change_state(RGBMode new_mode, RGBLED* rgb_led) {

        if (rgb_led == nullptr) {
            return;
        }
        
        rgb_led->current_mode = new_mode;
    }
    
    void change_fade_duration(uint32_t new_fade_duration, SimpleLED* led, RGBLED* rgb_led) {
        if (led == nullptr && rgb_led == nullptr) {
            return;
        }

        if (led != nullptr) {
            led->fade_duration = new_fade_duration;
        }

        if (rgb_led != nullptr) {
            rgb_led->fade_duration = new_fade_duration;
        }
    }


    void handle_led_mode(SimpleLED* led) {

        switch (led->current_mode) {
            case LEDS::LEDMode::Off:
                turn_off(led);
            return;

            case LEDMode::On:
                turn_on(led);
            break;
            
            case LEDMode::Blink:
                start_blinking(nullptr, led);
            break;

            case LEDMode::Fade:
                start_fade(nullptr, led);
            break;
        }
    }

    void handle_led_mode(RGBLED* rgb_led) {

        switch (rgb_led->current_mode) {
            case RGBMode::Off:
            return;

            case RGBMode::Constant:
                // OM: We simply exit the method without changing the colors.
            break;
            
            case RGBMode::Blink:
                start_blinking(rgb_led);
            break;

            case RGBMode::Fade:
                start_fade(rgb_led);
            break;

            case RGBMode::Random:
                start_random(rgb_led);
            break;
        }
    }

    void start_blinking(RGBLED* rgb_led, SimpleLED* led) {

        if (rgb_led == nullptr && led == nullptr) {
            return;
        }

        quick_blink(led, rgb_led);
    }

 
    void start_fade(RGBLED* rgb_led, SimpleLED* led) {
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

                uint8_t scaled_red = (uint16_t)rgb_led->PINS[0] * rgb_led->brightness_level / 255;
                uint8_t scaled_green = (uint16_t)rgb_led->PINS[1] * rgb_led->brightness_level / 255;
                uint8_t scaled_blue = (uint16_t)rgb_led->PINS[2] * rgb_led->brightness_level / 255;
                
                change_color(scaled_red, scaled_green, scaled_blue);
                rgb_led->last_fade_time = millis();
            }

        }


    }


    void reset_all() {
        reset_led(&white_led);
        reset_led(&orange_led);
        reset_led(&blue_led);
        reset_led(nullptr, &rgb_led);
    }

    void reset_led(SimpleLED* led, RGBLED* rgb_led) {

        if (led == nullptr && rgb_led == nullptr) {
            return;
        }

        if (led != nullptr) {
            led->manually_turned_off = false;
            led->on = true;
            led->manually_turned_off = false;
            led->manually_turned_on = false;
            led->current_mode = LEDMode::Off;
            led->fade_duration = 10;
            led->blink_intervall = 500;
            led->brightness_level = 0;
            led->last_blink_time = 0;
            led->fading_up = 0;
        }

        if (rgb_led == nullptr) {
            rgb_led->manually_turned_off = false;
            rgb_led->on = true;
            rgb_led->manually_turned_off = false;
            rgb_led->manually_turned_on = false;
            rgb_led->current_mode = RGBMode::Off;
            rgb_led->fade_duration = 10;
            rgb_led->blink_intervall = 500;
            rgb_led->random_change_intervall = 1000;
            rgb_led->brightness_level = 0;
            rgb_led->last_blink_time = 0;
            rgb_led->fading_up = 0;
            change_color(255, 0, 255);
        }
    }

    void start_random(RGBLED* rgb_led) {
        if (millis() - rgb_led->last_fade_time >= rgb_led->fade_duration) {
            rgb_led->target_red = random(0, 255);
            rgb_led->target_green = random(0, 255);
            rgb_led->target_blue = random(0, 255);
            change_color(rgb_led->target_red, rgb_led->target_green, rgb_led->target_blue);
            rgb_led->last_fade_time = millis();
        }
    }
    
    void change_color(uint8_t red, uint8_t green, uint8_t blue) {
        analogWrite(rgb_led.PINS[0], red);
        analogWrite(rgb_led.PINS[1], green);
        analogWrite(rgb_led.PINS[2], blue);
    }
}