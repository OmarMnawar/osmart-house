#include <Arduino.h>
#include "lcd.hpp"
#include "leds.hpp"
#include "sensor.hpp"
#include "remote.hpp"
#include "speaker.hpp"
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
  speaker::handle_sounds_playing();
  thermometer::update_thermometer();
}