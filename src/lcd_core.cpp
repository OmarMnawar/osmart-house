#include "lcd.hpp"
#include "remote.hpp"
#include "leds.hpp"
#include "outputs.hpp"


namespace lcd {
    
    LiquidCrystal_I2C lcd(config::LCD_ADDRESS, config::LCD_WIDTH, config::LCD_HEIGHT);
    power_state current_power_state = power_state::off;
    states current_state = states::no_action;
    
    uint32_t last_interaction_with_lcd = 0;


    void welcoming();
    void idle();
    void shutting_off();

    void init() {
        lcd.init();
        init_custom_chars();
    }
    
    void logic() {
        on_and_off();
        handle_states();
    }

    void on_and_off() {

        if (current_power_state == power_state::off) {

            if (remote::current_button != remote::buttons::on_or_off)
            {
                if (leds::orange_led.manually_turned_off == false) {
                    leds::orange_led.current_mode = leds::led_mode::on;
                }
                return;
            }
            if (leds::orange_led.manually_turned_on == false) {
                leds::orange_led.current_mode = leds::led_mode::off;
            }
            if (leds::blue_led.manually_turned_off == false) {
                leds::blue_led.current_mode = leds::led_mode::on;
            }
            lcd.display();
            lcd.backlight();
            current_power_state = power_state::on;
            current_state = states::showing_welcome_message;
        }
        else if (current_power_state == power_state::on && remote::current_button == remote::buttons::on_or_off) {
            current_state = states::shutting_off;
        }
    }

    void turn_off() {
        lcd.noDisplay();
        lcd.noBacklight();
        current_power_state = power_state::off;
        current_state = states::no_action;

        if (leds::blue_led.manually_turned_on == false) {
            leds::blue_led.current_mode = leds::led_mode::off;
        }
    }

    void handle_states() {

        if (current_power_state == power_state::off) {
            return;
        }

        switch (current_state) {

            case states::no_action:
            // OM: No need to do anything here since other parts of the codes handle the state for us.
            break;

            case states::showing_welcome_message:
                draw_scrolling_message(F("Welcome!"));
                current_state = states::idle;
            break;
            
            case states::idle:
                draw_message(F("Press Any button!"), 2, 0);
            break;
            
            case states::in_main_menu:
                in_main_menu(main_menu);
            break;

            case states::in_thermometer_menu:
                in_thermometer_menu(thermometer_menu);
            break;

            case states::in_music_menu:
                in_music_menu(music_menu);
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
            

            case states::shutting_off:
                delay(150);
                draw_message(F("Shutting Off!"), 2, 0, false);
                turn_off();
            break;
        }
    }


}