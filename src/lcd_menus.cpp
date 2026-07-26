#include "lcd.hpp"
#include "remote.hpp"
#include "thermometer.hpp"
#include "leds.hpp"

namespace lcd { 
    
    
    menu main_menu = { 1, 4 };
    menu thermometer_menu = {0, 0 };
    menu music_menu = {1, 2, };
    menu leds_menu = { 1, 6, };
    menu turn_on_menu = { 1, 4, };
    menu turn_off_menu = { 1, 4, };
  menu change_led_state_menu = { 1, 4, };
  menu led_state_menu = { 1, 4, };
  menu rgb_led_state_menu = { 1, 6, };
  menu color_change_menu = { 1, 6, };

    leds::simple_led* target_led = nullptr;
    leds::rgb_led* target_rgb_led = nullptr;

  uint8_t arrow_binary[] = {
    B01000,
    B01100,
    B01110,
    B01111,
    B01111,
    B01110,
    B01100,
    B01000,
  };
  uint8_t filled_pole_binary[] = {
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111 
  };

  uint8_t top_pole_binary[] = {
    B10001,
    B10001,
    B10001,
    B10001,
    B11111,
    B11111,
    B11111,
    B11111
  };

  uint8_t bottom_pole_binary[] = {
    B11111,
    B11111,
    B11111,
    B11111,
    B10001,
    B10001,
    B10001,
    B10001
  };

  uint8_t degree_symbol_binary[] = {
    B01100,
    B10010,
    B10010,
    B01100,
    B00000,
    B00000,
    B00000,
    B00000
  };
  
  uint8_t arrow = 0;
  uint8_t filled_pole = 1;
    uint8_t top_pole = 2;
    uint8_t bottom_pole = 3;
    uint8_t degree_symbol = 4;

    color_stage red_stage = { color_stages::red };
  color_stage green_stage = { color_stages::green };
  color_stage blue_stage = { color_stages::blue };


    void init_custom_chars() {

        lcd.createChar(arrow, arrow_binary);
        lcd.createChar(filled_pole, filled_pole_binary);
        lcd.createChar(top_pole, top_pole_binary);
        lcd.createChar(bottom_pole, bottom_pole_binary);
        lcd.createChar(degree_symbol, degree_symbol_binary);
    }

    bool menu_closed(menu &menu, states target_state, struct menu* target_menu) {
        if (menu.menu_existed == true) {
            lcd.clear();
            current_state = target_state;
            menu.needs_refresh = true;
            menu.menu_existed = false;

            if (menu.options_size > 0) {
                menu.current_pos = 1;
            }
            
            if (target_menu != nullptr){
                target_menu->option_clicked = false;
                target_menu->menu_existed = false;
                target_menu->needs_refresh = true;

                if (target_menu->options_size > 0) {
                    target_menu->current_pos = 1;
                }
            }

            return true;
        }
        return false;
    }

    void in_main_menu(menu &menu) {
        
        if (menu_closed(menu, states::idle)) {
            return;
        }
        
        if (menu.needs_refresh == true) {
            
            if (menu.current_pos == 1 || menu.current_pos == 2)
            {
                draw_menu(F("Thermometer"), F("Music"), menu.current_pos, menu.options_size);
                
            }
            else if (menu.current_pos == 3 || menu.current_pos == 4)
            {
                draw_menu(F("LEDs"), F("Shutdown"), menu.current_pos, menu.options_size);
            }
            
            menu.needs_refresh = false;
        }
        
        if (menu.option_clicked == true) {
            switch (static_cast<main_menu_options>(menu.current_pos)) {
                case main_menu_options::thermometer:
                current_state = states::in_thermometer_menu;
                break;
                
                case main_menu_options::music:
                current_state = states::in_music_menu;
                break;;
                
                case main_menu_options::leds:
                current_state = states::in_leds_menu;
                break;
                
                case main_menu_options::shutdown:
                menu.needs_refresh = true;
                menu.current_pos = 1;
                current_state = states::shutting_off;
                break;
            }

            menu.option_clicked = false;
        }
        
        update_menu_inputs(menu);
    }
    
    void in_thermometer_menu(menu &menu) {
        
        if (menu_closed(menu, states::in_main_menu, &main_menu)) {
            return;
        }
        
        if (thermometer::temperature_updated == true) {
            menu.needs_refresh = true;
        }
        
        if (menu.needs_refresh == true) {
            lcd.clear();
            lcd.setCursor(0, 0);        
            lcd.print("Temperature:");
            lcd.setCursor(0, 1);
            menu.needs_refresh = false;
            lcd.print(thermometer::temperature_celsius);
            lcd.write(degree_symbol);
        }
        
        update_menu_inputs(menu);
    }
    
    void in_music_menu(menu &menu) {

        if (menu_closed(menu, states::in_main_menu, &main_menu)) {
            return;
        }
        
        
        if (menu.needs_refresh == true) {
            if (menu.current_pos == 1 || menu.current_pos == 2) {
                draw_menu(F("Fuer Elise"), F("Harry Potter"), menu.current_pos, menu.options_size);
            }
            menu.needs_refresh = false;
        }

        if (menu.option_clicked == true) {
            switch  (static_cast<music_menu_options>(menu.current_pos)) {
                case music_menu_options::fuer_elise:
                // OM ToDo: Put the music playing here.
                break;
                
                case music_menu_options::harry_potter:
                // OM ToDo: Put the music playing here.
                break;
            }

            menu.option_clicked = false;
        }
        
        update_menu_inputs(menu);
    }
    

    void in_leds_menu(menu &menu) {

        
        if (menu_closed(menu, states::in_main_menu, &main_menu)) {
            return;
        }
        
        
        if (menu.needs_refresh == true) {
            
            if (menu.current_pos == 1 || menu.current_pos == 2) {
                draw_menu(F("Turn On"), F("Turn Off"), menu.current_pos, menu.options_size);
            }
            else if (menu.current_pos == 3 || menu.current_pos == 4) {
                draw_menu(F("LEDs state"), F("Change color"), menu.current_pos, menu.options_size);
            }
            else if (menu.current_pos == 5 || menu.current_pos == 6) {
                draw_menu(F("Fade duration"), F("Reset All"), menu.current_pos, menu.options_size);
            }

            menu.needs_refresh = false;
        }
        
        if (menu.option_clicked == true) {
            switch (static_cast<leds_Options>(menu.current_pos)) {
                case leds_Options::turn_on:
                    current_state = states::in_turn_on_menu;
                break;
                
                case leds_Options::turn_off:
                    current_state = states::in_turn_off_menu;
                break;
                
                case leds_Options::leds_state:
                    current_state = states::in_change_led_state_menu;
                break;
                
                case leds_Options::change_color:
                    current_state = states::in_color_change_menu;
                break;
                
                case leds_Options::change_fade_duration:
                    // OM ToDo: Build the menu für choosing the new fade duration and for which led.
                break;

                case leds_Options::reset_all:
                    leds::reset_all();
                break;
            }
    
            menu.option_clicked = false;
        }
        update_menu_inputs(menu);
    }
    
    void in_turn_on_menu(menu &menu) {
        if (menu_closed(menu, states::in_leds_menu, &leds_menu)) {
            return;
        } 
        
        if (menu.needs_refresh == true) {
            if (menu.current_pos == 1 || menu.current_pos == 2) {
                draw_menu(F("White LED"), F("Orange LED"), menu.current_pos, menu.options_size);
            }
            else if (menu.current_pos == 3 || menu.current_pos == 4) {
                draw_menu(F("Blue LED"), F("RGB LED"), menu.current_pos, menu.options_size);
            }
            menu.needs_refresh = false;
        }

        if (menu.option_clicked == true) {
            switch (static_cast<available_leds_options>(menu.current_pos)) {
                case available_leds_options::white_led:
                    leds::white_led.current_mode = leds::led_mode::on;
                    leds::white_led.manually_turned_on = true;
                break;

                case available_leds_options::orange_led:
                    leds::orange_led.current_mode = leds::led_mode::on;
                    leds::orange_led.manually_turned_on = true;
                break;
                
                case available_leds_options::blue_led:
                    leds::blue_led.current_mode = leds::led_mode::on;
                    leds::blue_led.manually_turned_on = true;
                break;
                
                case available_leds_options::rgb_led:
                    leds::main_rgb_led.current_mode = leds::rgb_mode::constant;
                    leds::main_rgb_led.manually_turned_on = true;
                break;
            }

            menu.option_clicked = false;
        }
        
        update_menu_inputs(menu);
    }
    
    void in_turn_off_menu(menu &menu) {
        
        if (menu_closed(menu, states::in_leds_menu, &leds_menu)) {
            return;
        } 
        
        if (menu.needs_refresh == true) {
            if (menu.current_pos == 1 || menu.current_pos == 2) {
                draw_menu(F("White LED"), F("Orange LED"), menu.current_pos, menu.options_size);
            }
            else if (menu.current_pos == 3 || menu.current_pos == 4) {
                draw_menu(F("Blue LED"), F("RGB LED"), menu.current_pos, menu.options_size);
            }
            menu.needs_refresh = false;
        }
        
        if (menu.option_clicked == true) {
            switch (static_cast<available_leds_options>(menu.current_pos)) {
                case available_leds_options::white_led:
                    leds::white_led.current_mode = leds::led_mode::off;
                    leds::white_led.manually_turned_off = true;
                break;

                case available_leds_options::orange_led:
                    leds::orange_led.current_mode = leds::led_mode::off;
                    leds::orange_led.manually_turned_off = true;
                break;
                
                case available_leds_options::blue_led:
                    leds::blue_led.current_mode = leds::led_mode::off;
                    leds::blue_led.manually_turned_off = true;
                break;
                
                case available_leds_options::rgb_led:
                    leds::main_rgb_led.current_mode = leds::rgb_mode::off;
                    leds::main_rgb_led.manually_turned_off = true;
                break;
            }
    
            menu.option_clicked = false;
        }
        update_menu_inputs(menu);
    }

    void in_change_led_state_menu(menu &menu) {

        if (menu_closed(menu, states::in_leds_menu, &leds_menu)) {
            return;
        }

        if (menu.needs_refresh == true) {
            if (menu.current_pos == 1 || menu.current_pos == 2) {
                draw_menu(F("White LED"), F("Orange LED"), menu.current_pos, menu.options_size);
            }
            else if (menu.current_pos == 3 || menu.current_pos == 4) {
                draw_menu(F("Blue LED"), F("RGB LED"), menu.current_pos, menu.options_size);
            }

            menu.needs_refresh = false;
        }

        if (menu.option_clicked == true) {
            switch (static_cast<available_leds_options>(menu.current_pos)) {

                case available_leds_options::white_led:
                    target_led = &leds::white_led;
                    current_state = states::in_led_state_menu;
                break;

                case available_leds_options::orange_led:
                    target_led = &leds::orange_led;
                    current_state = states::in_led_state_menu;
                break;

                case available_leds_options::blue_led:
                    target_led = &leds::blue_led;
                    current_state = states::in_led_state_menu;
                break;

                case available_leds_options::rgb_led:
                    target_rgb_led = &leds::main_rgb_led;
                    current_state = states::in_rgb_led_state_menu;
                break;
            }

            menu.option_clicked = false;
        }

        update_menu_inputs(menu);
    }


    void in_led_state_menu(menu &menu) {
        if (menu_closed(menu, states::in_change_led_state_menu, &change_led_state_menu)) {
            return;
        }

        if (menu.needs_refresh == true) {
            if (menu.current_pos == 1 || menu.current_pos == 2) {
                draw_menu(F("Off"), F("On"), menu.current_pos, menu.options_size);
            } 
            else if (menu.current_pos == 3 || menu.current_pos == 4) {
                draw_menu(F("Blink"), F("Fade"), menu.current_pos, menu.options_size);
            }

            menu.needs_refresh = false;
        }

        if (menu.option_clicked == true && target_led != nullptr) {
            switch (static_cast<available_leds_states_options>(menu.current_pos)) {
                case available_leds_states_options::off:
                    target_led->current_mode = leds::led_mode::off;
                break;

                case available_leds_states_options::on:
                    target_led->current_mode = leds::led_mode::on;
                break;

                case available_leds_states_options::blink:
                    target_led->current_mode = leds::led_mode::blink;
                break;

                case available_leds_states_options::fade:
                    target_led->current_mode = leds::led_mode::fade;
                break;
            }

            menu.option_clicked = false;
        }

        update_menu_inputs(menu);
    }

    void in_rgb_led_state_menu(menu &menu) {
        
        if (menu_closed(menu, states::in_change_led_state_menu, &change_led_state_menu)) {
            return;
        }

        if (menu.needs_refresh == true) {
            if (menu.current_pos == 1 || menu.current_pos == 2) {
                draw_menu(F("Off"), F("Constant"), menu.current_pos, menu.options_size);
            } 
            else if (menu.current_pos == 3 || menu.current_pos == 4) {
                draw_menu(F("Blink"), F("Fade"), menu.current_pos, menu.options_size);
            }
            else if (menu.current_pos == 5 || menu.current_pos == 6) {
                draw_menu(F("Random"), F("Remote"), menu.current_pos, menu.options_size);
            }

            menu.needs_refresh = false;
        }

        if (menu.option_clicked == true && target_rgb_led != nullptr) {
            switch (static_cast<available_rgb_leds_states_options>(menu.current_pos)) {
                case available_rgb_leds_states_options::off:
                    target_rgb_led->current_mode = leds::rgb_mode::off;
                break;

                case available_rgb_leds_states_options::constant:
                    target_rgb_led->current_mode = leds::rgb_mode::constant;
                break;

                case available_rgb_leds_states_options::blink:
                    target_rgb_led->current_mode = leds::rgb_mode::blink;
                break;

                case available_rgb_leds_states_options::fade:
                    target_rgb_led->current_mode = leds::rgb_mode::fade;
                break;

                case available_rgb_leds_states_options::random:
                    target_rgb_led->current_mode = leds::rgb_mode::random;
                break;

                case available_rgb_leds_states_options::remote:
                    target_rgb_led->current_mode = leds::rgb_mode::remote;
                break;
            }

            menu.option_clicked = false;
        }

        update_menu_inputs(menu);
    }

    void in_color_change_menu(menu &menu) {
        if (menu_closed(menu, states::in_leds_menu, &leds_menu)) {
            return;
        }

        if (menu.needs_refresh == true) {
            if (menu.current_pos == 1 || menu.current_pos == 2) {
                draw_menu(F("Red"), F("Green"), menu.current_pos, menu.options_size);
            } 
            else if (menu.current_pos == 3 || menu.current_pos == 4) {
                draw_menu(F("Blue"), F("Cyan"), menu.current_pos, menu.options_size);
            }
            else if (menu.current_pos == 5 || menu.current_pos == 6) {
                draw_menu(F("Pink"), F("Custom"), menu.current_pos, menu.options_size);
            }

            menu.needs_refresh = false;
        }

        if (menu.option_clicked == true) {

            switch (static_cast<available_colors_options>(menu.current_pos)) {

                case available_colors_options::red:
                    leds::change_color(255, 0, 0);
                break;

                case available_colors_options::green:
                    leds::change_color(0, 255, 0);
                break;

                case available_colors_options::blue:
                    leds::change_color(0, 0, 255);
                break;

                case available_colors_options::cyan:
                    leds::change_color(0, 255, 255);
                break;

                case available_colors_options::pink:
                    leds::change_color(255, 0, 255);
                break;

                case available_colors_options::custom:
                    current_state = states::in_color_red_stage;                    
                break;
            
            }
            menu.option_clicked = false;
        }

        update_menu_inputs(menu);
    }
}