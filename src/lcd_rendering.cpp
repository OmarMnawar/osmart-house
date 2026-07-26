#include "lcd.hpp"
#include "remote.hpp"
#include "outputs.hpp"
#include "timer.hpp"

namespace lcd {

    uint32_t message_timer = 0;

    void draw_scrolling_message(const __FlashStringHelper *message, uint8_t start_col, uint8_t start_row, uint32_t delay_time) {

        if (start_col < config::LCD_WIDTH) {

            if (remote::current_button != remote::buttons::none) {
                lcd.clear();
                // OM ToDo: later make this actually gets back to the current menu
                if (remote::current_button != remote::buttons::on_or_off) {
                    current_state = states::in_main_menu;
                }
            }
            
            uint32_t last_print_time = millis();

            lcd.clear();
            lcd.setCursor(start_col, start_row);
            lcd.print(message);
            if (millis() - last_print_time >= delay_time) {
                
            }
            
            lcd.clear();
            lcd.setCursor(start_col, start_row);
            lcd.print(" ");
            start_col++;

        }

        lcd.clear();
        lcd.setCursor(0, 0);
    }

    void draw_message(const __FlashStringHelper *message, uint8_t col_pos, uint8_t row_pos, bool repeat, uint32_t delay_time) {
        // OM ToDo: later make the message actually appear based on the lentgh of the message.
        bool text_visible = true;
        uint32_t last_blink_time = millis();
                
        lcd.clear();
        lcd.setCursor(col_pos, row_pos);
        lcd.print(message);
        timer::start();
        
        do {
            if (remote::current_button != remote::buttons::none) {
                lcd.clear();
                // OM ToDo: later make this actually gets back to the current menu
                if (remote::current_button != remote::buttons::on_or_off) {
                    current_state = states::in_main_menu;
                }
                break;
            }

            // OM: Message always gets printed with 75% value of given delay time.
            if ((millis() - last_blink_time >= (delay_time * 3) / 4 ) && repeat == true) {
                text_visible = !text_visible;
                lcd.setCursor(col_pos, row_pos);
                
                if (text_visible == true) {
                    lcd.print(message);
                } 
                else {
                    lcd.clear();
                }
                last_blink_time = millis();
            }
        } while (timer::is_time_reached(delay_time) == false);

        lcd.clear();
        timer::stop();
    }
        
    void draw_menu(const __FlashStringHelper *first_option, const __FlashStringHelper *second_option, uint8_t current_pos, uint8_t menu_size)
    {
        lcd.clear();

        uint8_t arrow_pos = (current_pos % 2 != 0) ? 0 : 1;
        uint8_t top_char;
        
        lcd.setCursor(0, 0);
        if (arrow_pos == 0)
        {
            lcd.write(arrow);
        }
        lcd.print(first_option);
        lcd.setCursor(0, 1);
        if (arrow_pos == 1)
        {
            lcd.write(arrow);
        }

        if (second_option != nullptr) {
            lcd.print(second_option);
        }

        // OM: We exit since there is only two options.
        if (menu_size <= 2) {
            return;
        }
        
        uint8_t total_pairs = (menu_size + 1) / 2;
        uint8_t current_pair = (current_pos + 1) / 2;
        uint8_t bottom_char;
        uint8_t lcd_width_end = config::LCD_WIDTH - 1;
        if (current_pair == 1) {
            top_char = top_pole;
            bottom_char = filled_pole;
        }
        else if (current_pair == total_pairs) {
            top_char = filled_pole;
            bottom_char = bottom_pole;
        }
        else {
            top_char = bottom_pole;
            bottom_char = top_pole;
        }
        lcd.setCursor(lcd_width_end, 0);
        lcd.write(top_char);
        lcd.setCursor(lcd_width_end, 1);
        lcd.write(bottom_char);
    }
    

    void draw_stage(const __FlashStringHelper *current_color, const char *input) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(current_color);
        lcd.setCursor(0, 1);
        lcd.print(input);
    }

    void draw_normal_message(const __FlashStringHelper *first_row, const __FlashStringHelper *second_row, uint32_t message_duration) {
        if (message_timer == 0) {
            lcd.clear();
            message_timer = millis();
            lcd.noCursor();
        }

        do {
            lcd.noBlink();
            lcd.cursor_off();
            lcd.setCursor(2, 0);
            lcd.print(first_row);
            lcd.setCursor(2, 1);
            lcd.print(second_row);

        } while((millis() - message_timer >= message_duration) == false );
                    
        message_timer = 0;
    }
}