#include <Arduino.h>
#include "LCD.hpp"
#include "LED.hpp"
#include "Sensor.hpp"
#include "Remote.hpp"
#include "Thermometer.hpp"


void setup()
{
  Sensor::init();
  Lcd::init();
  Remote::init();
  LED::init();
}

void loop()
{
  Sensor::update_state();
  Remote::update_buttons();
  Lcd::logic();
  Thermometer::update_thermometer();
  LED::logic();
}