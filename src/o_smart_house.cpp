#include <Arduino.h>
#include "lcd.hpp"
#include "leds.hpp"
#include "sensor.hpp"
#include "remote.hpp"
#include "thermometer.hpp"


void setup()
{
  sensor::init();
  lcd::init();
  remote::init();
  leds::init();
}

void loop()
{
  sensor::update_state();
  remote::update_buttons();
  leds::logic();
  lcd::logic();
  thermometer::update_thermometer();
}