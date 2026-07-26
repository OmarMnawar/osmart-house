#include "Arduino.h"
#include "WString.h"
#include "lcd.hpp"
#include "remote.hpp"
#include "outputs.hpp"

namespace lcd {

    uint32_t message_timer = 0;
    uint32_t last_blink_time = 0;
    bool message_drawn = false;
    uint32_t last_scroll_time = 0;
    uint8_t scroll_col = 0;

    void draw_scrolling_message(const __FlashStringHelper *message, states target_state, bool repeat, uint32_t delay_time) {

        // We need to check at what position we currently are.
        // Then we need to print the message on that position.
        // We need to wait at the delay time.
        // after that we need to clear the screen.
        // then we need to print the message on the next position.
        if (remote::current_button != remote::buttons::none) {
            lcd.clear();
            lcd.setCursor(0, 0);
            scroll_col = 0;
            // OM ToDo: later make this actually gets back to the current menu
            if (remote::current_button != remote::buttons::on_or_off) {
                current_state = target_state;
            }
            return;
        }

        if (last_scroll_time == 0) { 
            last_scroll_time = millis();
            lcd.setCursor(scroll_col, 0);
            lcd.print(message);
        }

        if (scroll_col < config::LCD_WIDTH) {
            if (millis() - last_scroll_time >= delay_time) {
                lcd.setCursor(scroll_col, 0);
                lcd.print(" ");
                scroll_col++;
                lcd.setCursor(scroll_col, 0);
                lcd.print(message);
                last_scroll_time = millis();

            }
        }
        else {
            scroll_col = 0;
            last_scroll_time = 0;
            if (repeat == false) {
                current_state = target_state;
                return;
            }
        }
    }

    void draw_message(const __FlashStringHelper *first_message, const __FlashStringHelper *second_message, uint8_t first_col_pos, uint8_t first_row_pos, uint8_t second_col_pos, uint8_t second_row_pos, states target_state, bool repeat, uint32_t delay_time) {
        
        if (first_col_pos > config::LCD_WIDTH || second_col_pos > config::LCD_WIDTH) {
            return;
        }

        if (first_row_pos > config::LCD_HEIGHT || second_row_pos > config::LCD_HEIGHT) {
            return;
        }

        if (remote::current_button != remote::buttons::none) {
            lcd.clear();
            // OM ToDo: later make this actually gets back to the current menu
            if (remote::current_button != remote::buttons::on_or_off) {
                current_state = target_state;
            }
            return;
        }
        
        if (last_blink_time == 0) {
            last_blink_time = millis();
        }

        // OM: Message always gets printed with 75% value of given delay time.
        if ((millis() - last_blink_time >= (delay_time * 3) / 4 ) && repeat == true) {
            message_drawn = !message_drawn;
            
            if (message_drawn == true) {
                lcd.setCursor(first_col_pos, first_row_pos);
                lcd.print(first_message);
                lcd.setCursor(second_col_pos, second_row_pos);
                lcd.print(second_message);
            } 
            else {
                lcd.clear();
            }

            last_blink_time = 0;
        }
    }
        
    void draw_menu(const __FlashStringHelper *first_option, const __FlashStringHelper *second_option, uint8_t current_pos, uint8_t menu_size)
    {
        if (current_pos > config::LCD_WIDTH) {
            return;
        }

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
        lcd.setCursor(5, 0);
        lcd.print(input);
    }

    void draw_normal_message(const __FlashStringHelper *first_row, const __FlashStringHelper *second_row, uint8_t first_row_col, uint8_t second_row_col, uint32_t message_duration) {
        
        if (first_row_col > config::LCD_WIDTH || second_row_col > config::LCD_WIDTH) {
            return;
        }
        
        
        if (message_timer == 0) {
            lcd.clear();
            message_timer = millis();
            lcd.noCursor();
        }

        do {
            lcd.noBlink();
            lcd.cursor_off();
            lcd.setCursor(first_row_col, 0);
            lcd.print(first_row);
            lcd.setCursor(second_row_col, 1);
            lcd.print(second_row);

        } while((millis() - message_timer >= message_duration) == false );
                    
        message_timer = 0;
    }
}