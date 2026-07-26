#ifndef LCD_H
#define LCD_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <sys/types.h>



namespace lcd {
  
  enum class power_state
  {
    off,
    on,
  };

  enum class states : uint8_t
  {
    no_action = 0,
    showing_welcome_message = 1,
    idle = 2,
    in_main_menu = 3,
    in_thermometer_menu = 4,
    in_music_menu = 5,
    in_leds_menu = 6,
    in_turn_on_menu = 7,
    in_turn_off_menu = 8,
    in_change_led_state_menu = 9,
    in_led_state_menu = 10,
    in_rgb_led_state_menu = 11,
    in_color_change_menu = 12,
    in_color_red_stage = 13,
    in_color_green_stage = 14,
    in_color_blue_stage = 15,
    shutting_off = 16
  };

  
  enum class main_menu_options : uint8_t {
    thermometer = 1,
    music = 2,
    leds = 3,
    shutdown = 4
  };
  
  enum class music_menu_options : uint8_t {
    fuer_elise = 1,
    harry_potter = 2
  };
  
  enum class leds_Options : uint8_t {
    turn_on = 1,
    turn_off = 2,
    leds_state = 3,
    change_color = 4,
    change_fade_duration = 5,
    reset_all = 6
  };
  
  
  enum class available_leds_options : uint8_t {
    white_led = 1,
    orange_led = 2,
    blue_led = 3,
    rgb_led = 4,
  };

  enum class available_leds_states_options : uint8_t {
    off = 1,
    on = 2,
    blink = 3,
    fade = 4
  };

  enum class available_rgb_leds_states_options : uint8_t {
    off = 1,
    constant = 2,
    blink = 3,
    fade = 4,
    random = 5,
    remote = 6
  };

  enum class available_colors_options : uint8_t {
    red = 1,
    green = 2,
    blue = 3,
    cyan = 4,
    pink = 5,
    custom = 6
  };

  enum class color_stages : uint8_t {
    red = 0,
    green = 1,
    blue = 2
  };

  constexpr uint32_t MESSAGE_DELAY_TIME = 500;
  
  struct menu  {
    uint8_t current_pos = 1;
    uint8_t options_size = 3;
    bool needs_refresh;
    bool option_clicked = false;
    bool menu_existed = false;
    
    menu(uint8_t pos, uint8_t max_s,bool refresh = true) {
      current_pos = pos;
      options_size = max_s;
      needs_refresh = refresh;
    }
  };
  
  
  struct color_stage {
    char display_color_value[4];
    uint8_t red_value;
    uint8_t green_value;
    uint8_t blue_value;
    uint8_t digit_count = 0;
    bool value_cleard = false;
    bool needs_refresh = true;
    bool option_clicked = false;
    bool canceld = false;
    
    color_stages stage;
    
    color_stage (color_stages stage) {
      this->stage = stage;
    } 
  };
  
  void init();
  void logic();
  void handle_states();
  void on_and_off();
  void turn_off();
  void init_custom_chars();
  void in_main_menu(menu &menu);
  void in_thermometer_menu(menu &menu);
  void in_music_menu(menu &menu);
  void in_leds_menu(menu &menu);
  void in_turn_on_menu(menu &menu);
  void in_turn_off_menu(menu &menu);
  void in_led_state_menu(menu &menu);
  void in_rgb_led_state_menu(menu &menu);
  void in_change_led_state_menu(menu &menu);
  void in_color_change_menu(menu &menu);
  void in_colors_change_stage(color_stage &stage);
  void draw_stage(const __FlashStringHelper *current_color, const char *input);
  void append_digit(color_stage &stage, char digit_char);
  void update_color_stage_inputs(color_stage &stage);
  char parse_color_value(uint8_t numbers[4]);
  bool stage_canceld(color_stage &stage, states target_state, menu* targe_menu);
  bool menu_closed(menu &menu, states target_state, struct menu* target_menu = nullptr);
  void update_menu_inputs(menu &menu);
  void draw_scrolling_message(const __FlashStringHelper *message, uint8_t start_col = 0, uint8_t start_row = 0, uint32_t delay_time = MESSAGE_DELAY_TIME);
  void draw_message(const __FlashStringHelper *message, uint8_t col_pos, uint8_t row_pos, bool repeat = true, uint32_t delay_time = MESSAGE_DELAY_TIME);
  void draw_menu(const __FlashStringHelper *first_option, const __FlashStringHelper *second_option = nullptr, uint8_t current_pos = 0, uint8_t menu_size = 0);
  void draw_normal_message(const __FlashStringHelper *first_row, const __FlashStringHelper *second_row, uint32_t message_duration = 750);
  
  extern LiquidCrystal_I2C lcd;
  extern uint8_t arrow;
  extern uint8_t filled_pole;
  extern uint8_t top_pole;
  extern uint8_t bottom_pole;
  extern uint8_t degree_symbol;
  extern power_state current_power_state;
  extern states current_state;
  extern menu main_menu;
  extern menu thermometer_menu;
  extern menu music_menu;
  extern menu leds_menu;
  extern menu turn_on_menu;
  extern menu turn_off_menu;
  extern menu change_led_state_menu;
  extern menu led_state_menu;
  extern menu rgb_led_state_menu;
  extern menu color_change_menu;
  extern color_stages current_stage;
  extern color_stage red_stage;
  extern color_stage green_stage;
  extern color_stage blue_stage;
  
}

#endif