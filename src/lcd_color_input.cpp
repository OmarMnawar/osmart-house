#include "leds.hpp"
#include "WString.h"
#include "lcd.hpp"
#include <avr/interrupt.h>


namespace lcd {

    color_stages current_stage = color_stages::red;
    bool show_error_message = true;
    bool show_succeded_message = false;
    bool remote_message_shown = false;


    bool is_color_valid_and_apply(const char *color_input, color_stage &stage);
    void reset_all_stages(); 
    void reset_stage(color_stage &stage);
    void stages_completed(color_stage &stage, states target_state, menu &target_menu);


    void in_colors_change_stage(color_stage &stage) {

        if (stage_canceld(stage, states::in_color_change_menu, &color_change_menu) == true) {
            return;
        }

        if (stage.option_clicked == true) {
            switch (current_stage) {

            case color_stages::red:
                if (is_color_valid_and_apply(stage.display_color_value, stage) == false) {
                    draw_normal_message(F("Not valid Color"), F("value"));
                }
                else {
                    current_stage = color_stages::green;
                    current_state = states::in_color_green_stage;
                }
            break;

            case color_stages::green:
                if (is_color_valid_and_apply(stage.display_color_value, stage) == false) {
                    draw_normal_message(F("Not valid Color"), F("value"));
                }
                else {
                    current_stage = color_stages::blue;
                    current_state = states::in_color_blue_stage;
                }
            break;

            case color_stages::blue:
                if (is_color_valid_and_apply(stage.display_color_value, stage) == false) {
                    draw_normal_message(F("Not valid Color"), F("value"));
                }
                else {
                    draw_normal_message(F("New RGB color"), F("was set"));
                    leds::change_color(red_stage.red_value, green_stage.green_value, blue_stage.blue_value);
                    stages_completed(stage, states::in_color_change_menu, color_change_menu);
                    return;
                }
              break;
            }

            stage.option_clicked = false;
        }
        
        if (remote_message_shown == false) {
            draw_normal_message(F("Use the remote"), F("value 0-255"));
            remote_message_shown = true;
            reset_stage(stage);
        }

        if (stage.needs_refresh == true && remote_message_shown == true) {
            
            if (stage.value_cleard == true) {
                reset_stage(stage);
                stage.value_cleard = false;
            }
            
            switch (current_stage) {
                case color_stages::red:
                    draw_stage(F("Red:"), stage.display_color_value);
                break;
                    
                case color_stages::green:
                    draw_stage(F("Green:"), stage.display_color_value);
                break;
                    
                case color_stages::blue:
                    draw_stage(F("Blue:"), stage.display_color_value);
                break;
            }
                
                
            lcd.blink_on();
            stage.needs_refresh = false;
        }

        update_color_stage_inputs(stage);
    }

    bool stage_canceld(color_stage &stage, states target_state, menu* target_menu) {

        if (target_menu == nullptr) {
            return false;
        }

        if (stage.canceld == true) {
            lcd.clear();
            remote_message_shown = false;
            reset_all_stages();
            lcd.noCursor();
            current_state = target_state;
            current_stage = color_stages::red;
            target_menu->needs_refresh = true;
            target_menu->menu_existed = false;
            target_menu->option_clicked = false;
            return true;
        }

        return false;
    }

    void append_digit(color_stage &stage, char digit_char) {
        if (stage.digit_count >= 3) {
            return;
        }
        stage.display_color_value[stage.digit_count] = digit_char;
        stage.digit_count++;
        stage.display_color_value[stage.digit_count] = '\0';
    }

    bool is_color_valid_and_apply(const char *color_input, color_stage &stage) {

        long parsed = atol(color_input);

        if (parsed > 255 || parsed < 0) {
            return false;
        }

        switch (stage.stage) {

            case color_stages::red:
                stage.red_value = (uint8_t)parsed;
            return true;

            case color_stages::green:
                stage.green_value = (uint8_t)parsed;
            return true;

            case color_stages::blue:
                stage.blue_value = (uint8_t)parsed;
            return true;
        }
    }

    void reset_all_stages() {
        reset_stage(red_stage);
        reset_stage(green_stage);
        reset_stage(blue_stage);
    }

    void reset_stage(color_stage &stage) {
        stage.needs_refresh = true;
        stage.option_clicked = false;
        stage.canceld = false;
        lcd.noCursor();
        stage.digit_count = 0;
        stage.display_color_value[0] = '\0';
        stage.red_value = 0;
        stage.green_value = 0;
        stage.blue_value = 0;
    }

    void stages_completed(color_stage &stage, states target_state, menu &target_menu) {
        reset_all_stages();
        lcd.noCursor();
        current_stage = color_stages::red;
        current_state = target_state;
        target_menu.needs_refresh = true;
        target_menu.menu_existed = false;
        target_menu.option_clicked = false;
        remote_message_shown = false;
    }

}