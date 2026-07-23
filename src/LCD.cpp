#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "Arduino.h"
#include "LEDS.hpp"
#include "States.hpp"
#include "Outputs.hpp"
#include "LCD.hpp"
#include "Thermometer.hpp"
#include "Timer.hpp"
#include "WString.h"

namespace Lcd {
        
    PowerState current_power_state = PowerState::Off;
    State current_state = State::NoAction;
    constexpr uint32_t MESSAGE_DELAY_TIME = 500;
    LiquidCrystal_I2C lcd(Config::LCD_ADDRESS, Config::LCD_WIDTH, Config::LCD_HEIGHT); 
    uint32_t last_interaction_with_lcd = 0;

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
    Menu music_menu = {1, 2, true };
    Menu leds_menu = { 1, 6, true };
    Menu turn_on_menu = { 1, 4, true };
    Menu turn_off_menu = { 1, 4, true };
    Menu change_led_state_menu = { 1, 4, true };
    Menu led_state_menu = { 1, 4, true };
    Menu rgb_led_state_menu = { 1, 6, true };
    Menu color_change_menu = { 1, 6, true };
    Menu red_stage = { 0, 0, true };
    Menu green_stage = { 0, 0, true };
    Menu blue_stage = { 0, 0, true };

    ColorStages current_stage = ColorStages::Red;

    LEDS::SimpleLED* target_led = nullptr;
    LEDS::RGBLED* target_rgb_led = nullptr;

    void on_and_off();
    void turn_off();
    void welcoming();
    void idle();
    void in_main_menu(Menu &menu);
    void in_thermometer_menu(Menu &menu);
    void in_music_menu(Menu &menu);
    void in_leds_menu(Menu &menu);
    void in_turn_on_menu(Menu &menu);
    void in_turn_off_menu(Menu &menu);
    void in_led_state_menu(Menu &menu);
    void in_rgb_led_state_menu(Menu &menu);
    void in_change_led_state_menu(Menu &menu);
    void in_color_change_menu(Menu &menu);
    void in_colors_change_stage(Menu &menu);
    bool menu_closed(Menu &menu, State target_state, Menu* target_menu = nullptr);
    void update_menu_inputs(Menu &menu);
    void shutting_off();
    void draw_scrolling_message(const __FlashStringHelper *message, uint8_t start_col = 0, uint8_t start_row = 0, uint32_t delay_time = MESSAGE_DELAY_TIME);
    void draw_message(const __FlashStringHelper *message, uint8_t col_pos, uint8_t row_pos, bool repeat = true, uint32_t delay_time = MESSAGE_DELAY_TIME);
    void draw_menu(const __FlashStringHelper *first_option, const __FlashStringHelper *second_option = nullptr, uint8_t current_pos = 0, uint8_t menu_size = 0);
    
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

    #pragma region Init
    void init() {
        lcd.init();
        lcd.createChar(arrow, arrow_binary);
        lcd.createChar(filled_pole, filled_pole_binary);
        lcd.createChar(top_pole, top_pole_binary);
        lcd.createChar(bottom_pole, bottom_pole_binary);
        lcd.createChar(degree_symbol, degree_symbol_binary);
    }
    
    void logic() {
        on_and_off();
        states();
    }
    #pragma endregion

    #pragma region Base LCD Logic

    void on_and_off() {

        if (current_power_state == PowerState::Off) {

            if (Remote::current_button != Remote::Buttons::OnOrOff)
            {
                if (LEDS::orange_led.manually_turned_off == false) {
                    LEDS::orange_led.current_mode = LEDS::LEDMode::On;
                }
                return;
            }
            if (LEDS::orange_led.manually_turned_on == false) {
                LEDS::orange_led.current_mode = LEDS::LEDMode::Off;
            }
            if (LEDS::blue_led.manually_turned_off == false) {
                LEDS::blue_led.current_mode = LEDS::LEDMode::On;
            }
            lcd.display();
            lcd.backlight();
            current_power_state = PowerState::On;
            current_state = State::ShowingWelcomeMessage;
        }
        else if (current_power_state == PowerState::On && Remote::current_button == Remote::Buttons::OnOrOff) {
            current_state = State::ShuttingOff;
        }
    }

    void states() {
        if (current_power_state == PowerState::Off) {
            return;
        }

        switch (current_state) {

            case State::NoAction:
            // OM: No need to do anything here since other parts of the codes handle the state for us.
            break;

            case State::ShowingWelcomeMessage:
                draw_scrolling_message(F("Welcome!"));
                current_state = State::Idle;
            break;
            
            case State::Idle:
                draw_message(F("Press Any button!"), 2, 0);
            break;
            
            case State::InMainMenu:
                in_main_menu(main_menu);
            break;

            case State::InThermometerMenu:
                in_thermometer_menu(thermometer_menu);
            break;

            case State::InMusicMenu:
                in_music_menu(music_menu);
            break;

            case State::InTurnOnMenu:
                in_turn_on_menu(turn_on_menu);
            break;

            case State::InTurnOffMenu:
                in_turn_off_menu(turn_off_menu);
            break;

            case State::InLedsMenu:
                in_leds_menu(leds_menu);
            break;

            case State::InChangeLEDStateMenu:
                in_change_led_state_menu(change_led_state_menu);
            break;
            
            case State::InLEDStateMenu:
                in_led_state_menu(led_state_menu);
            break;

            case State::InColorChange:
                in_color_change_menu(color_change_menu);
            break;

            case State::InColorRedStage:
                in_color_change_menu(color_change_menu);
            break;

            case State::InColorGreenStage:
                in_color_change_menu(color_change_menu);
            break;

            case State::InColorBlueStage:
                in_color_change_menu(color_change_menu);
            break;
            
            case State::InRGBLEDStateMenu:
                in_rgb_led_state_menu(rgb_led_state_menu);
            break;

            case State::ShuttingOff:
                delay(150);
                draw_message(F("Shutting Off!"), 2, 0, false);
                turn_off();
            break;
        }
    }

    void turn_off() {
        lcd.noDisplay();
        lcd.noBacklight();
        current_power_state = PowerState::Off;
        current_state = State::NoAction;

        if (LEDS::blue_led.manually_turned_on == false) {
            LEDS::blue_led.current_mode = LEDS::LEDMode::Off;
        }
    }
    #pragma endregion

    #pragma region LCD Menus

    void in_main_menu(Menu &menu) {
        
        if (menu_closed(menu, State::Idle)) {
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
            switch (static_cast<MainMenuOptions>(menu.current_pos)) {
                case Lcd::MainMenuOptions::Thermometer:
                current_state = State::InThermometerMenu;
                break;
                
                case Lcd::MainMenuOptions::Music:
                current_state = State::InMusicMenu;
                break;;
                
                case Lcd::MainMenuOptions::LEDs:
                current_state = State::InLedsMenu;
                break;
                
                case Lcd::MainMenuOptions::Shutdown:
                menu.needs_refresh = true;
                menu.current_pos = 1;
                current_state = State::ShuttingOff;
                break;
            }

            menu.option_clicked = false;
        }
        
        update_menu_inputs(menu);
    }
    
    void in_thermometer_menu(Menu &menu) {
        
        if (menu_closed(menu, State::InMainMenu, &main_menu)) {
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
            lcd.write(degree_symbol);
        }
        
        update_menu_inputs(menu);
    }
    
    void in_music_menu(Menu &menu) {

        if (menu_closed(menu, State::InMainMenu, &main_menu)) {
            return;
        }
        
        
        if (menu.needs_refresh == true) {
            if (menu.current_pos == 1 || menu.current_pos == 2) {
                draw_menu(F("Fuer Elise"), F("Harry Potter"), menu.current_pos, menu.options_size);
            }
            menu.needs_refresh = false;
        }

        if (menu.option_clicked == true) {
            switch  (static_cast<MusicMenuOptions>(menu.current_pos)) {
                case Lcd::MusicMenuOptions::FuerElise:
                // OM ToDo: Put the music playing here.
                break;
                
                case Lcd::MusicMenuOptions::HarryPotter:
                // OM ToDo: Put the music playing here.
                break;
            }

            menu.option_clicked = false;
        }
        
        update_menu_inputs(menu);
    }
    

    void in_leds_menu(Menu &menu) {

        
        if (menu_closed(menu, State::InMainMenu, &main_menu)) {
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
            switch (static_cast<LedsOptions>(menu.current_pos)) {
                case Lcd::LedsOptions::TurnOn:
                    current_state = State::InTurnOnMenu;
                break;
                
                case Lcd::LedsOptions::TurnOff:
                    current_state = State::InTurnOffMenu;
                break;
                
                case Lcd::LedsOptions::LEDsState:
                    current_state = State::InChangeLEDStateMenu;
                break;
                
                case Lcd::LedsOptions::ChangeColor:
                    current_state = State::InColorChange;
                break;
                
                case Lcd::LedsOptions::ChangeFadeDuration:
                    // OM ToDo: Build the menu für choosing the new fade duration and for which led.
                break;

                case Lcd::LedsOptions::ResetAll:
                    LEDS::reset_all();
                break;
            }
    
            menu.option_clicked = false;
        }
        update_menu_inputs(menu);
    }
    
    void in_turn_on_menu(Menu &menu) {
        if (menu_closed(menu, State::InLedsMenu, &leds_menu)) {
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
            switch (static_cast<AvailableLEDsOptions>(menu.current_pos)) {
                case Lcd::AvailableLEDsOptions::WhiteLED:
                    LEDS::white_led.current_mode = LEDS::LEDMode::On;
                    LEDS::white_led.manually_turned_on = true;
                break;

                case Lcd::AvailableLEDsOptions::OrangeLED:
                    LEDS::orange_led.current_mode = LEDS::LEDMode::On;
                    LEDS::orange_led.manually_turned_on = true;
                break;
                
                case Lcd::AvailableLEDsOptions::BlueLED:
                    LEDS::blue_led.current_mode = LEDS::LEDMode::On;
                    LEDS::blue_led.manually_turned_on = true;
                break;
                
                case Lcd::AvailableLEDsOptions::RGBLED:
                    LEDS::rgb_led.current_mode = LEDS::RGBMode::Constant;
                    LEDS::rgb_led.manually_turned_on = true;
                break;
            }

            menu.option_clicked = false;
        }
        
        update_menu_inputs(menu);
    }
    
    void in_turn_off_menu(Menu &menu) {
        
        if (menu_closed(menu, State::InLedsMenu, &leds_menu)) {
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
            switch (static_cast<AvailableLEDsOptions>(menu.current_pos)) {
                case Lcd::AvailableLEDsOptions::WhiteLED:
                    LEDS::white_led.current_mode = LEDS::LEDMode::Off;
                    LEDS::white_led.manually_turned_off = true;
                break;

                case Lcd::AvailableLEDsOptions::OrangeLED:
                    LEDS::orange_led.current_mode = LEDS::LEDMode::Off;
                    LEDS::orange_led.manually_turned_off = true;
                break;
                
                case Lcd::AvailableLEDsOptions::BlueLED:
                    LEDS::blue_led.current_mode = LEDS::LEDMode::Off;
                    LEDS::blue_led.manually_turned_off = true;
                break;
                
                case Lcd::AvailableLEDsOptions::RGBLED:
                    LEDS::rgb_led.current_mode = LEDS::RGBMode::Off;
                    LEDS::rgb_led.manually_turned_off = true;
                break;
            }
    
            menu.option_clicked = false;
        }
        update_menu_inputs(menu);
    }

    void in_change_led_state_menu(Menu &menu) {

        if (menu_closed(menu, State::InLedsMenu, &leds_menu)) {
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
            switch (static_cast<AvailableLEDsOptions>(menu.current_pos)) {

                case AvailableLEDsOptions::WhiteLED:
                    target_led = &LEDS::white_led;
                    current_state = State::InLEDStateMenu;
                break;

                case AvailableLEDsOptions::OrangeLED:
                    target_led = &LEDS::orange_led;
                    current_state = State::InLEDStateMenu;
                break;

                case AvailableLEDsOptions::BlueLED:
                    target_led = &LEDS::blue_led;
                    current_state = State::InLEDStateMenu;
                break;

                case AvailableLEDsOptions::RGBLED:
                    target_rgb_led = &LEDS::rgb_led;
                    current_state = State::InRGBLEDStateMenu;
                break;
            }

            menu.option_clicked = false;
        }

        update_menu_inputs(menu);
    }


    void in_led_state_menu(Menu &menu) {
        if (menu_closed(menu, State::InChangeLEDStateMenu, &change_led_state_menu)) {
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
            switch (static_cast<AvailableLEDsStatesOptions>(menu.current_pos)) {
                case Lcd::AvailableLEDsStatesOptions::Off:
                    target_led->current_mode = LEDS::LEDMode::Off;
                break;

                case AvailableLEDsStatesOptions::On:
                    target_led->current_mode = LEDS::LEDMode::On;
                break;

                case AvailableLEDsStatesOptions::Blink:
                    target_led->current_mode = LEDS::LEDMode::Blink;
                break;

                case AvailableLEDsStatesOptions::Fade:
                    target_led->current_mode = LEDS::LEDMode::Fade;
                break;
            }

            menu.option_clicked = false;
        }

        update_menu_inputs(menu);
    }

    void in_rgb_led_state_menu(Menu &menu) {
        
        if (menu_closed(menu, State::InChangeLEDStateMenu, &change_led_state_menu)) {
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
            switch (static_cast<AvailableRGBLEDsStatesOptions>(menu.current_pos)) {
                case Lcd::AvailableRGBLEDsStatesOptions::Off:
                    target_rgb_led->current_mode = LEDS::RGBMode::Off;
                break;

                case AvailableRGBLEDsStatesOptions::Constant:
                    target_rgb_led->current_mode = LEDS::RGBMode::Constant;
                break;

                case AvailableRGBLEDsStatesOptions::Blink:
                    target_rgb_led->current_mode = LEDS::RGBMode::Blink;
                break;

                case AvailableRGBLEDsStatesOptions::Fade:
                    target_rgb_led->current_mode = LEDS::RGBMode::Fade;
                break;

                case AvailableRGBLEDsStatesOptions::Random:
                    target_rgb_led->current_mode = LEDS::RGBMode::Random;
                break;

                case AvailableRGBLEDsStatesOptions::Remote:
                    target_rgb_led->current_mode = LEDS::RGBMode::Remote;
                break;
            }

            menu.option_clicked = false;
        }

        update_menu_inputs(menu);
    }

    void in_color_change_menu(Menu &menu) {
        if (menu_closed(menu, State::InLedsMenu, &leds_menu)) {
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

            switch (static_cast<AvailableColorsOptions>(menu.current_pos)) {

                case Lcd::AvailableColorsOptions::Red:
                    LEDS::change_color(255, 0, 0);
                break;

                case Lcd::AvailableColorsOptions::Green:
                    LEDS::change_color(0, 255, 0);
                break;

                case Lcd::AvailableColorsOptions::Blue:
                    LEDS::change_color(0, 0, 255);
                break;

                case Lcd::AvailableColorsOptions::Cyan:
                    LEDS::change_color(0, 255, 255);
                break;

                case Lcd::AvailableColorsOptions::Pink:
                    LEDS::change_color(255, 0, 255);
                break;

                case Lcd::AvailableColorsOptions::Custom:
                    current_state = State::InColorRedStage;
                    in_colors_change_stage(menu);
                break;
            
            }
            menu.option_clicked = false;
        }

        update_menu_inputs(menu);
    }

    void in_colors_change_stage(Menu &menu) {
        if (menu_closed(menu, State::InLedsMenu, &leds_menu)) {
            return;
        }

        if (menu.needs_refresh == true) {
            
        }
    }
    #pragma endregion

    #pragma region Helper Methods
    
    bool menu_closed(Menu &menu, State target_state, Menu* target_menu) {
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
        uint8_t lcd_width_end = Config::LCD_WIDTH - 1;
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
    
    
    void draw_scrolling_message(const __FlashStringHelper *message, uint8_t start_col, uint8_t start_row, uint32_t delay_time) {

        if (start_col < Config::LCD_WIDTH) {

            if (Remote::current_button != Remote::Buttons::None) {
                lcd.clear();
                // OM ToDo: later make this actually gets back to the current menu
                if (Remote::current_button != Remote::Buttons::OnOrOff) {
                    current_state = State::InMainMenu;
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
        Timer::start();
        
        do {
            if (Remote::current_button != Remote::Buttons::None) {
                lcd.clear();
                // OM ToDo: later make this actually gets back to the current menu
                if (Remote::current_button != Remote::Buttons::OnOrOff) {
                    current_state = State::InMainMenu;
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
        } while (Timer::is_time_reached(delay_time) == false);

        lcd.clear();
        Timer::stop();
    }
    #pragma endregion
}