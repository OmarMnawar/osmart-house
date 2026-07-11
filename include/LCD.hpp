#ifndef LCD_H
#define LCD_H

#include <Arduino.h>

namespace Lcd {
  void init();
  void logic();
  void turn_off_logic();
  void states();
  void show_message(const char *message, uint32_t delay_time);
  void show_message(const char *message, uint8_t start_col, uint8_t start_row, uint8_t lcd_width, uint32_t timer);
}


#endif