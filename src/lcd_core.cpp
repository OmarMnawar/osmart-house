#include "Arduino.h"
#include "lcd.hpp"
#include "remote.hpp"
#include "leds.hpp"
#include "outputs.hpp"
#include "speaker.hpp"


namespace lcd {
    
    LiquidCrystal_I2C lcd(config::LCD_ADDRESS, config::LCD_WIDTH, config::LCD_HEIGHT);
    power_state current_power_state = power_state::off;
    states current_state = states::no_action;
    // OM: Auto turn off after 3 minutes of inactivity in the screen.
    // Can be edited in the lcd screen later.
    // To a fixed presetet time.
    uint32_t auto_shutdown_time = 180000;
    uint32_t last_interaction_with_lcd = 0;

    void welcoming();
    void idle();
    void handle_no_interaction();

    void init() {
        lcd.init();
        init_custom_chars();
    }
    
    void logic() {
        on_and_off();
        handle_no_interaction();
        handle_states();
    }

    void on_and_off() {

        if (current_power_state == power_state::off) {

            if (remote::current_button != remote::buttons::on_or_off)
            {
                if (leds::orange_led.manually_turned_off == false || leds::orange_led.manually_turned_on == false) {
                    leds::orange_led.current_mode = leds::led_mode::on;
                }
                return;
            }
            if (leds::orange_led.manually_turned_on == false || leds::orange_led.manually_turned_off == false) {
                leds::orange_led.current_mode = leds::led_mode::off;
            }
            if (leds::blue_led.manually_turned_off == false || leds::blue_led.manually_turned_on == false) {
                leds::blue_led.current_mode = leds::led_mode::on;
            }
            lcd.display();
            lcd.backlight();
            speaker::current_state = speaker::states::playing_lcd_on_sound;
            current_power_state = power_state::on;
            current_state = states::showing_welcome_message;
        }
        else if (current_power_state == power_state::on && remote::current_button == remote::buttons::on_or_off) {
            current_state = states::shutting_down;
        }
    }

    void turn_off() {
        // OM: In this case we want to block any remote input so that the turning off wont get caneld.
        speaker::current_state = speaker::states::playing_lcd_off_sound;
        delay(1000);
        lcd.noDisplay();
        lcd.noBacklight();
        current_power_state = power_state::off;
        current_state = states::no_action;

        if (leds::blue_led.manually_turned_on == false || leds::blue_led.manually_turned_off == false) {
            leds::blue_led.current_mode = leds::led_mode::off;
        }
    }

    void handle_no_interaction() {
        if (current_power_state == power_state::off) {
            last_interaction_with_lcd = 0;
            return;
        }

        if (current_power_state == power_state::on) {
            if (remote::current_button == remote::buttons::none) {
                if (last_interaction_with_lcd == 0) {
                    last_interaction_with_lcd = millis();
                }
    
                if (millis() - last_interaction_with_lcd >= auto_shutdown_time) {
                    reset_all_menus();
                    reset_all_stages();
                    current_state = states::auto_shutting_down;
                }
            }
            else {
                if (last_interaction_with_lcd > 0) {
                    last_interaction_with_lcd = 0;
                }
            }
        }
    }

    void handle_states() {

        switch (current_state) {

            case states::no_action:
            // OM: No need to do anything here since other parts of the codes handle the state for us.
            break;

            case states::showing_welcome_message:
                draw_scrolling_message(F("Welcome!"), states::idle);
            break;
            
            case states::idle:
                draw_message(F("Press Any"), F("button!"), 3, 0, 4, 1, states::in_main_menu);
            break;
            
            case states::in_main_menu:
                in_main_menu(main_menu);
            break;

            case states::in_thermometer_menu:
                in_thermometer_menu(thermometer_menu);
            break;

            case states::in_turn_on_menu:
                in_turn_on_menu(turn_on_menu);
            break;

            case states::in_turn_off_menu:
                in_turn_off_menu(turn_off_menu);
            break;

            case states::in_leds_menu:
                in_leds_menu(leds_menu);
            break;

            case states::in_change_led_state_menu:
                in_change_led_state_menu(change_led_state_menu);
            break;
            
            case states::in_led_state_menu:
                in_led_state_menu(led_state_menu);
            break;

            case states::in_rgb_led_state_menu:
                in_rgb_led_state_menu(rgb_led_state_menu);
            break;

            case states::in_color_change_menu:
                in_color_change_menu(color_change_menu);
            break;

            case states::in_color_red_stage:
                in_colors_change_stage(red_stage);
            break;

            case states::in_color_green_stage:
                in_colors_change_stage(green_stage);
            break;

            case states::in_color_blue_stage:
                in_colors_change_stage(blue_stage);
            break;

            case states::in_auto_shutdown:
                in_auto_shutdown_menu(auto_shutdown_menu);
            break;

            case states::shutting_down:
                lcd.clear();
                lcd.setCursor(2, 0);
                lcd.print(F("Shutting off!"));
                turn_off();
            break;

            case states::auto_shutting_down:
                lcd.clear();
                lcd.setCursor(2, 0);
                lcd.print(F("Auto shutting"));
                lcd.setCursor(6, 1);
                lcd.print(F("off!"));
                turn_off();
            break;
        }
    }
}