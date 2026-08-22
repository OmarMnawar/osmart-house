#include "lcd.hpp"
#include "remote.hpp"
#include "remote.hpp"

namespace lcd {


    void append_digit(color_stage &stage, char digit_char);

    
    void update_menu_inputs(menu &menu) {

        if (remote::current_button == remote::buttons::plus)
        {
            menu.current_pos--;
            if (menu.current_pos == (menu.options_size - menu.options_size) )
            {
                menu.current_pos = menu.options_size;
            }
            menu.needs_refresh = true;
        }
        else if (remote::current_button == remote::buttons::minus)
        {
            menu.current_pos++;
            if (menu.current_pos > menu.options_size)
            {
                menu.current_pos = menu.current_pos - menu.options_size;
            }
            menu.needs_refresh = true;
        }
        else if (remote::current_button == remote::buttons::confirm) {
            menu.option_clicked = true;
        }
        else if (remote::current_button == remote::buttons::back) {
            menu.menu_existed = true;
        }
        else if (remote::current_button == remote::buttons::on_or_off) {
            menu.needs_refresh = true;
            menu.current_pos = 1;
        }
    }

    void update_color_stage_inputs(color_stage &stage) {
        if (remote::current_button == remote::buttons::back) {
            stage.canceld = true;
        }
        else if (remote::current_button == remote::buttons::confirm) {
            stage.option_clicked = true;
            stage.needs_refresh = true;
        }
        else if (remote::current_button == remote::buttons::number_0) {
            append_digit(stage, '0');
            stage.needs_refresh = true;
        }
        else if (remote::current_button == remote::buttons::number_1) {
            append_digit(stage, '1');
            stage.needs_refresh = true;
        }
        else if (remote::current_button == remote::buttons::number_2) {
            append_digit(stage, '2');
            stage.needs_refresh = true;
        }
        else if (remote::current_button == remote::buttons::number_3) {
            append_digit(stage, '3');
            stage.needs_refresh = true;
        }
        else if (remote::current_button == remote::buttons::number_4) {
            append_digit(stage, '4');
            stage.needs_refresh = true;
        }
        else if (remote::current_button == remote::buttons::number_5) {
            append_digit(stage, '5');
            stage.needs_refresh = true;
        }
        else if (remote::current_button == remote::buttons::number_6) {
            append_digit(stage, '6');
            stage.needs_refresh = true;
        }
        else if (remote::current_button == remote::buttons::number_7) {
            append_digit(stage, '7');
            stage.needs_refresh = true;
        }
        else if (remote::current_button == remote::buttons::number_8) {
            append_digit(stage, '8');
            stage.needs_refresh = true;
        }
        else if (remote::current_button == remote::buttons::number_9) {
            append_digit(stage, '9');
            stage.needs_refresh = true;
        }
        else if (remote::current_button == remote::buttons::clear) {
            stage.value_cleard = true;
            stage.needs_refresh = true;
        }

    }

    void append_digit(color_stage &stage, char digit_char) {
        if (stage.digit_count >= 3) {
            return;
        }
        stage.display_color_value[stage.digit_count] = digit_char;
        stage.digit_count++;
        stage.display_color_value[stage.digit_count] = '\0';
    }

}