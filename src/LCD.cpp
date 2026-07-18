#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "Arduino.h"
#include "States.hpp"
#include "Outputs.hpp"
#include "LCD.hpp"
#include "Thermometer.hpp"
#include "Timer.hpp"

namespace Lcd {
        
    PowerState current_power_state = PowerState::Off;
    State current_state = State::NoAction;
    constexpr uint32_t MESSAGE_DELAY_TIME = 500;
    LiquidCrystal_I2C lcd(Config::LCD_ADDRESS, Config::LCD_WIDTH, Config::LCD_HEIGHT); 

    struct Menu  {
        uint8_t current_pos = 1;
        uint8_t options_size = 3;
        bool needs_refresh = true;
        bool option_clicked = false;
        bool menu_existed = false;

        Menu(uint8_t pos, uint8_t max_s,bool refresh) {
            current_pos = pos;
            options_size = max_s;
            needs_refresh = refresh;
        }
    };

    Menu main_menu = { 1, 4, true };
    Menu thermometer_menu = {0, 0 , true };
    
    void on_and_off();
    void turn_off();
    void welcoming();
    void idle();
    void in_main_menu(Menu &menu);
    void in_thermometer_menu(Menu &menu);
    bool menu_closed(Menu &menu, State target_state);
    void update_menu_inputs(Menu &menu);
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
                    current_state = State::ShuttingOff;
                }
                
            }
            else if (Sensor::current_state == Sensor::States::MovementDetected) {
                if (Remote::current_button != Remote::Buttons::OnOrOff)
                {
                    current_state = State::ShuttingOff;
                }
            }
        }
    }

    void states() {
        if (current_power_state == PowerState::Off) {
            current_state = State::NoAction;
            return;
        }

        switch (current_state) {

            case State::NoAction:
            // OM: No need to do anything here since other parts of the codes handle the state for us.
            break;

            case State::ShowingWelcomeMessage:
                message_scroll("Welcome!");
                current_state = State::Idle;
            break;
            
            case State::Idle:
                message("Press Any button!", 2, 0);
            break;
            
            case State::InMainMenu:
                in_main_menu(main_menu);
            break;

            case State::InThermometerMenu:
                in_thermometer_menu(thermometer_menu);
            break;

            case State::InMusicMenu:

            break;

            case State::InRgbLedMenu:

            break;

            case State::ShuttingOff:
                message("Shutting Off!", 2, 0, false, 2000);
                turn_off();
            break;
        }
    }


    void turn_off() {
        lcd.noDisplay();
        lcd.noBacklight();
        current_power_state = PowerState::Off;
        current_state = State::NoAction;
    }


    void in_main_menu(Menu &menu) {

        update_menu_inputs(menu);

        if (menu.option_clicked == true) {

            switch (menu.current_pos) {
                case 1:
                    current_state = State::InThermometerMenu;
                return;

                case 2:
                    current_state = State::InMusicMenu;
                return;

                case 3:
                    current_state = State::InRgbLedMenu;
                return;

                case 4:
                    current_state = State::ShuttingOff;
                return;
            }
        }

        if (menu_closed(menu, State::Idle)) {
            return;
        }

        if (menu.needs_refresh == true) {

            if (menu.current_pos == 1 || menu.current_pos == 2)
            {
                menu_logic("Thermometer", "Music", menu.current_pos, menu.options_size);

            }
            else if (menu.current_pos == 3 || menu.current_pos == 4)
            {
                menu_logic("RGB LED", "Shutdown", menu.current_pos, menu.options_size);
            }

            menu.needs_refresh = false;
        }
    }

    void in_thermometer_menu(Menu &menu) {

        update_menu_inputs(menu);
        
        if (menu_closed(menu, State::InMainMenu)) {
            return;
        }

        if (Thermometer::temperature_updated == true) {
            menu.needs_refresh = true;
        }

        if (menu.needs_refresh == true) {
            lcd.clear();
            lcd.setCursor(0, 0);        
            lcd.print("Temperature:");
            lcd.setCursor(0, 1);
            menu.needs_refresh = false;
            lcd.print(Thermometer::temperature_celsius);
        }
    }

    bool menu_closed(Menu &menu, State target_state) {
        if (menu.menu_existed == true) {
            lcd.clear();
            current_state = target_state;
            menu.needs_refresh = true;
            menu.menu_existed = false;
            return true;
        }
        return false;
    }
    
    void update_menu_inputs(Menu &menu) {

        if (Remote::current_button == Remote::Buttons::Plus)
        {
            menu.current_pos--;
            if (menu.current_pos == (menu.options_size - menu.options_size) )
            {
                menu.current_pos = menu.options_size;
            }
            menu.needs_refresh = true;
        }
        else if (Remote::current_button == Remote::Buttons::Minus)
        {
            menu.current_pos++;
            if (menu.current_pos > menu.options_size)
            {
                menu.current_pos = menu.current_pos - menu.options_size;
            }
            menu.needs_refresh = true;
        }
        else if (Remote::current_button == Remote::Buttons::Confirm) {
            menu.option_clicked = true;
        }
        else if (Remote::current_button == Remote::Buttons::Back) {
            menu.menu_existed = true;
        }
        else if (Remote::current_button == Remote::Buttons::OnOrOff) {
            menu.needs_refresh = true;
            menu.current_pos = 1;
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
        do {
            Timer::start();
            lcd.clear();
            lcd.setCursor(start_col, start_row);
            lcd.print(message);
            // OM ToDo: we should convert this to also use millis instead of blocking the arduin microcontroller.
            delay(delay_time);
            lcd.clear();
            lcd.setCursor(start_col, start_row);
            lcd.print(" ");
            Timer::stop();
            start_col++;
            
        } while(start_col < Config::LCD_WIDTH);
        
        lcd.clear();
        lcd.setCursor(0, 0);
    }

    void message(const char *message, uint8_t col_pos, uint8_t row_pos, bool repeat, uint32_t delay_time) {
        // OM ToDo: later make the message actually appear based on the lentgh of the message.

        bool text_visible = true;
        uint32_t last_blink_time = millis();
                
        lcd.clear();
        lcd.setCursor(col_pos, row_pos);
        lcd.print(message);
        Timer::start();
        
        do {
            if (Remote::current_button != Remote::Buttons::None) {
                lcd.clear();
                // OM ToDo: later make this actually gets back to the current menu
                if (Remote::current_button != Remote::Buttons::OnOrOff) {
                    current_state = State::InMainMenu;
                }
                return;
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
        } while (Timer::is_time_reached(delay_time) == false);

        lcd.clear();
        Timer::stop();
    }
}