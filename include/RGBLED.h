#ifndef RGB_LED_H
#define RGB_LED_H

#include <stdint.h>

void InitLED();
void LEDLogic();

void ChangeColor(uint8_t red,uint8_t green,uint8_t blue);

#endif