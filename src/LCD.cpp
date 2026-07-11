#include "Arduino.h"
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "States.hpp"
#include "Outputs.hpp"
#include "LCD.hpp"

namespace Lcd {
    
    PowerState current_power_state = PowerState::Off;
    State current_state = State::NoAction;

    LiquidCrystal_I2C lcd(Config::LCD_ADDRESS, Config::LCD_WIDTH, Config::LCD_HEIGHT); 

    void init() {
        lcd.init();
    }

    void logic() {
        turn_off_logic();
        states();
    }

    void turn_off_logic() {
        if (current_power_state != PowerState::On) {
            if (Sensor::current_state != Sensor::States::MovementDetected && Remote::current_button != Remote::Buttons::OnOrOff)
            {
                return;
            }

            lcd.backlight();
            current_power_state = PowerState::On;
            current_state = State::ShowingWelcomeMessage;
        }
        else if (current_power_state == PowerState::On) {

            if (Remote::current_button == Remote::Buttons::OnOrOff)
            {
                if (Sensor::current_state != Sensor::States::MovementDetected)
                {
                    lcd.noBacklight();
                    current_power_state = PowerState::Off;
                    current_state = State::TurningOff;
                }
                
            }
            else if (Sensor::current_state == Sensor::States::MovementDetected) {
                if (Remote::current_button != Remote::Buttons::OnOrOff)
                {
                    lcd.noBacklight();
                    current_power_state = PowerState::Off;
                    current_state = State::TurningOff;
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
            show_message("Welcome", 0, 0, 16, 750);
            current_state = State::Idle;
            break;
            
            case State::Idle:
            show_message("Press Any button!", 750);
            break;
            
            case State::InMainMenu:

            break;

            case State::InTemperatureMenu:

            break;

            case State::InMusicMenu:

            break;

            case State::InRgbLedMenu:

            break;

            case State::TurningOff:

            break;
        }
    }

    

    // Messages helper.
    void show_message(const char *message, uint32_t delay_time) {
        do {
            lcd.clear();
            lcd.print(message);
            delay(delay_time);

            if (Remote::current_button != Remote::Buttons::None)
            {
                lcd.clear();
                current_state = State::InMainMenu;
                break;
            }

        } while (true);
    }
    void show_message(const char *message, uint8_t start_col, uint8_t start_row, uint8_t lcd_width, uint32_t timer) {
        uint8_t message_len = 0;

        while (message[message_len] != '\0') {
            message_len++;
        }

        do {
            lcd.clear();
            lcd.setCursor(start_col, start_row);
            lcd.print(message);
            delay(timer);
            lcd.clear();
            lcd.setCursor(start_col, start_row);
            lcd.print(" ");
            start_col++;

        } while(start_col < lcd_width);

        lcd.clear();
    }
}