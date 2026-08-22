#include "lcd.hpp"
#include "thermometer.hpp"
#include "leds.hpp"

namespace lcd { 
    
    menu main_menu = { 1, 4 };
    menu thermometer_menu = {0, 0 };
    menu leds_menu = { 1, 5, };
    menu turn_on_menu = { 1, 4, };
    menu turn_off_menu = { 1, 4, };
    menu change_led_state_menu = { 1, 4, };
    menu led_state_menu = { 1, 4, };
    menu rgb_led_state_menu = { 1, 6, };
    menu color_change_menu = { 1, 6, };
    menu auto_shutdown_menu = {1, 4 };

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

    void reset_menu(menu &menu);


    void init_custom_chars() {
        lcd.createChar(arrow, arrow_binary);
        lcd.createChar(filled_pole, filled_pole_binary);
        lcd.createChar(top_pole, top_pole_binary);
        lcd.createChar(bottom_pole, bottom_pole_binary);
        lcd.createChar(degree_symbol, degree_symbol_binary);
    }

    bool menu_closed(menu &current_menu, states target_state, struct menu* target_menu) {
        if (current_menu.menu_existed == true) {
            lcd.clear();
            current_state = target_state;
            current_menu.needs_refresh = true;
            current_menu.menu_existed = false;

            if (current_menu.options_size > 0) {
                current_menu.current_pos = 1;
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
                draw_menu(F("Thermometer"), F("LEDs"), menu.current_pos, menu.options_size);
                
            }
            else if (menu.current_pos == 3 || menu.current_pos == 4)
            {
                draw_menu(F("Auto shutdown"), F("Shutdown"), menu.current_pos, menu.options_size);
            }
            
            menu.needs_refresh = false;
        }
        
        if (menu.option_clicked == true) {
            switch (static_cast<main_menu_options>(menu.current_pos)) {
                case main_menu_options::thermometer:
                    current_state = states::in_thermometer_menu;
                break;
                
                case main_menu_options::leds:
                    current_state = states::in_leds_menu;
                break;

                case main_menu_options::auto_shutdown:
                    current_state = states::in_auto_shutdown;
                break;

                break;
                
                case main_menu_options::shutdown:
                    menu.needs_refresh = true;
                    menu.current_pos = 1;
                    current_state = states::shutting_down;
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
            else if (menu.current_pos == 5) {
                draw_menu(F("Reset All"), nullptr, menu.current_pos, menu.options_size);
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
                    leds::white_led.manually_turned_off = false;
                    draw_normal_message(F("LED turned"), F("on!"), 2, 6);
                break;

                case available_leds_options::orange_led:
                    leds::orange_led.current_mode = leds::led_mode::on;
                    leds::orange_led.manually_turned_on = true;
                    leds::orange_led.manually_turned_off = false;
                    draw_normal_message(F("LED turned"), F("on!"), 2, 6);
                break;
                
                case available_leds_options::blue_led:
                    leds::blue_led.current_mode = leds::led_mode::on;
                    leds::blue_led.manually_turned_on = true;
                    leds::blue_led.manually_turned_off = false;
                    draw_normal_message(F("LED turned"), F("on!"), 2, 6);
                break;
                
                case available_leds_options::rgb_led:
                    leds::main_rgb_led.current_mode = leds::rgb_mode::constant;
                    leds::main_rgb_led.manually_turned_on = true;
                    leds::main_rgb_led.manually_turned_off = false;
                    draw_normal_message(F("RGB turned"), F("on!"), 2, 6);
                break;
            }

            menu.option_clicked = false;
            menu.needs_refresh = true;
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
                    draw_normal_message(F("LED turned"), F("off!"), 2, 6);
                break;

                case available_leds_options::orange_led:
                    leds::orange_led.current_mode = leds::led_mode::off;
                    leds::orange_led.manually_turned_off = true;
                    draw_normal_message(F("LED turned"), F("off!"), 2, 6);
                break;
                
                case available_leds_options::blue_led:
                    leds::blue_led.current_mode = leds::led_mode::off;
                    leds::blue_led.manually_turned_off = true;
                    draw_normal_message(F("LED turned"), F("off!"), 2, 6);
                break;
                
                case available_leds_options::rgb_led:
                    leds::main_rgb_led.current_mode = leds::rgb_mode::off;
                    leds::main_rgb_led.manually_turned_off = true;
                    draw_normal_message(F("RGB turned"), F("off!"), 2, 6);
                break;
            }

            menu.option_clicked = false;
            menu.needs_refresh = true;
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

            draw_normal_message(F("LED"), F("updated!"), 6, 4);
            menu.option_clicked = false;
            menu.needs_refresh = true;
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

            draw_normal_message(F("RGB  state"), F("updated!"), 2, 3);
            menu.option_clicked = false;
            menu.needs_refresh = true;
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
            draw_normal_message(F("RGB  Color"), F("updated!"), 2 , 3);
            menu.option_clicked = false;
            menu.needs_refresh = true;
        }

        update_menu_inputs(menu);
    }

    void in_auto_shutdown_menu(menu &menu) {
        if (menu_closed(menu, states::in_main_menu, &main_menu)) {
            return;
        }

        if (menu.needs_refresh == true) {
            if (menu.current_pos == 1 || menu.current_pos == 2) {
                draw_menu(F("Three minutes"), F("Two minutes"), menu.current_pos, menu.options_size);
            } 
            else if (menu.current_pos == 3 || menu.current_pos == 4) {
                draw_menu(F("One minute"), F("Thiry seconds"), menu.current_pos, menu.options_size);
            }

            menu.needs_refresh = false;
        }

        if (menu.option_clicked == true) {

            switch (static_cast<available_auto_shutdown_options>(menu.current_pos)) {

                case available_auto_shutdown_options::three_minutes:
                    auto_shutdown_time = 180000;
                break;
                    
                case available_auto_shutdown_options::two_minutes:
                    auto_shutdown_time = 120000;
                
                break;
                    
                case available_auto_shutdown_options::one_minute:
                    auto_shutdown_time = 60000;
                
                break;
                    
                case available_auto_shutdown_options::thirty_seconds:
                    auto_shutdown_time = 30000;
                
                break;
                    
            }

            draw_normal_message(F("Auto Shutdown"), F("updated!"), 2, 4);
            menu.option_clicked = false;
            menu.needs_refresh = true;
        }

        update_menu_inputs(menu);
    }

    void reset_all_menus() {
        reset_menu(main_menu);
        reset_menu(thermometer_menu);
        reset_menu(leds_menu);
        reset_menu(turn_on_menu);
        reset_menu(turn_off_menu);
        reset_menu(change_led_state_menu);
        reset_menu(led_state_menu);
        reset_menu(rgb_led_state_menu);
        reset_menu(color_change_menu);
    }

    void reset_menu(menu &menu) {
        menu.menu_existed = false;
        menu.current_pos = 1;
        menu.needs_refresh = true;
        menu.option_clicked = false;
    }
}