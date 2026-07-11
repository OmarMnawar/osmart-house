#include <Arduino.h>
#include "LCD.hpp"
#include "Sensor.hpp"
#include "Remote.hpp"


void setup()
{
  Sensor::init();
  Lcd::init();
  Remote::init();
}

void loop()
{
  Sensor::update_state();
  Remote::update_buttons();
  Lcd::logic();
}