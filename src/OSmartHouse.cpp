#include <Arduino.h>
#include "LCD.hpp"
#include "LEDS.hpp"
#include "Sensor.hpp"
#include "Remote.hpp"
#include "Thermometer.hpp"


void setup()
{
  Sensor::init();
  Lcd::init();
  Remote::init();
  LEDS::init();
}

void loop()
{
  Sensor::update_state();
  Remote::update_buttons();
  Lcd::logic();
  Thermometer::update_thermometer();
  LEDS::logic();
}