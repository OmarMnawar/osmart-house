#include "Arduino.h"
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <avr/io.h>
#include "States.hpp"
#include "Outputs.hpp"
#include "LCD.hpp"
#include "Timer.hpp"

namespace Lcd {
    
    
    
    
    PowerState current_power_state = PowerState::Off;
    State current_state = State::NoAction;
    constexpr uint32_t MESSAGE_DELAY_TIME = 50;
    LiquidCrystal_I2C lcd(Config::LCD_ADDRESS, Config::LCD_WIDTH, Config::LCD_HEIGHT); 

    struct Menu  {
        uint8_t current_pos = 1;
        uint8_t options_size = 3;
        bool needs_refresh = true;

        Menu(uint8_t pos, uint8_t max_s,bool refresh) {
            current_pos = pos;
            options_size = max_s;
            needs_refresh = refresh;
        }
    };

    Menu main_menu = { 1, 4, true };
    
    void on_and_off();
    void welcoming();
    void idle();
    void in_main_menu(Menu &main_menu);
    void shutting_off();
    void message_scroll(const char *message, uint8_t start_col = 0, uint8_t start_row = 0, uint32_t delay_time = MESSAGE_DELAY_TIME);
    void message(const char *message, uint8_t col_pos, uint8_t row_pos, bool repeat = true, uint32_t delay_time = MESSAGE_DELAY_TIME);
    void menu_logic(const char *first_option, const char *second_option, uint8_t current_pos, uint8_t menu_size);
    
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

    uint8_t arrow = 0;
    uint8_t filled_pole = 1;
    uint8_t top_pole = 2;
    uint8_t bottom_pole = 3;


    void init() {
        lcd.init();
        lcd.createChar(arrow, arrow_binary);
        lcd.createChar(filled_pole, filled_pole_binary);
        lcd.createChar(top_pole, top_pole_binary);
        lcd.createChar(bottom_pole, bottom_pole_binary);

    }

    void logic() {
        on_and_off();
        states();
    }

    void on_and_off() {
        if (current_power_state != PowerState::On) {
            if (Sensor::current_state != Sensor::States::MovementDetected && Remote::current_button != Remote::Buttons::OnOrOff)
            {
                return;
            }

            lcd.display();
            lcd.backlight();
            current_power_state = PowerState::On;
            current_state = State::ShowingWelcomeMessage;
        }
        else if (current_power_state == PowerState::On) {

            if (Remote::current_button == Remote::Buttons::OnOrOff)
            {
                if (Sensor::current_state != Sensor::States::MovementDetected)
                {
                    lcd.noDisplay();
                    lcd.noBacklight();
                    current_power_state = PowerState::Off;
                    current_state = State::ShuttingOff;
                }
                
            }
            else if (Sensor::current_state == Sensor::States::MovementDetected) {
                if (Remote::current_button != Remote::Buttons::OnOrOff)
                {
                    lcd.noDisplay();
                    lcd.noBacklight();
                    current_power_state = PowerState::Off;
                    current_state = State::ShuttingOff;
                }
            }
        }
    }

    void states() {

        switch (current_state) {

            case State::NoAction:
            // OM: No need to do anything here since other parts of the codes handle the state for us.
            break;

            case State::ShowingWelcomeMessage:
                message_scroll("Welcome!");
                current_state = State::Idle;
            break;
            
            case State::Idle:
                message("Press Any button!", 2, 0, true, 500);
            break;
            
            case State::InMainMenu:
            in_main_menu(main_menu);
            break;

            case State::InTemperatureMenu:

            break;

            case State::InMusicMenu:

            break;

            case State::InRgbLedMenu:

            break;

            case State::ShuttingOff:
            message("Shutting Off!", 2, 0, false);
            break;
        }
    }


    void in_main_menu(Menu &menu) {
        
        if (Remote::current_button == Remote::Buttons::Plus)
        {
            menu.current_pos--;
            if (menu.current_pos < (menu.options_size - menu.options_size))
            {
                menu.current_pos = 4;
            }
            menu.needs_refresh = true;
        }
        else if (Remote::current_button == Remote::Buttons::Minus)
        {
            menu.current_pos++;
            if (menu.current_pos > menu.options_size)
            {
                menu.current_pos = 1;
            }
            menu.needs_refresh = true;
        }

        if (menu.needs_refresh) {
            if (menu.current_pos == 1 || menu.current_pos == 2)
            {
                menu_logic("Thermometer", "Music", menu.current_pos, menu.options_size);

            }
            else if (menu.current_pos == 3)
            {
                menu_logic("RGB LED", "Shutdown", menu.current_pos, menu.options_size);
            }

            menu.needs_refresh = false;
        }
    }

    void menu_logic(const char *first_option, const char *second_option, uint8_t current_pos, uint8_t menu_size)
    {
        lcd.clear();

        uint8_t arrow_pos = (current_pos % 2 != 0) ? 0 : 1;

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
        lcd.print(second_option);

        uint8_t lcd_width_end = Config::LCD_WIDTH - 1;
        lcd.setCursor(lcd_width_end, 0);
        if (current_pos < menu_size) {
            lcd.write(top_pole);
        }
        else if (current_pos == menu_size) {
            lcd.write(filled_pole);
        }
        lcd.setCursor(lcd_width_end, 1);
        if (current_pos < menu_size) {
            lcd.write(filled_pole);
        }
        else if (current_pos == menu_size) {
            lcd.write(bottom_pole);
        }
    }
    
    
    void message_scroll(const char *message, uint8_t start_col, uint8_t start_row, uint32_t delay_time) {
        uint8_t message_len = 0;
        
        while (message[message_len] != '\0') {
            message_len++;
        }
        
        do {
            lcd.clear();
            lcd.setCursor(start_col, start_row);
            lcd.print(message);
            delay(delay_time);
            lcd.clear();
            lcd.setCursor(start_col, start_row);
            lcd.print(" ");
            start_col++;
            
        } while(start_col < Config::LCD_WIDTH);
        
        lcd.clear();
    }

    void message(const char *message, uint8_t col_pos, uint8_t row_pos, bool repeat, uint32_t delay_time) {
        do {
            lcd.clear();
            lcd.setCursor(col_pos, row_pos);
            lcd.print(message);
            Timer::start();

            if (Remote::current_button != Remote::Buttons::None) {
                lcd.clear();
                // OM ToDo: later make this actually gets back to the current menu
                current_state = State::InMainMenu;
                break;
            }
            if (repeat == false)
            {
                lcd.clear();
                break;
            }
            
        } while (Timer::is_time_reached(delay_time));
    }
}