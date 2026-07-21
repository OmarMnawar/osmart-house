#ifndef LCD_H
#define LCD_H

#include <Arduino.h>

namespace Lcd {
  void init();
  void logic();
  void states();

  enum class PowerState
  {
      Off,
      On,
  };

  enum class State
  {
      NoAction,
      ShowingWelcomeMessage,
      Idle,
      InMainMenu,
      InThermometerMenu,
      InMusicMenu,
      InLedsMenu,
      InTurnOnMenu,
      InTurnOffMenu,
      ShuttingOff
  };

  
  enum class MainMenuOptions : uint8_t {
    Thermometer = 1,
    Music = 2,
    LEDs = 3,
    Shutdown = 4
  };
  
  enum class MusicMenuOptions : uint8_t {
    FuerElise = 1,
    HarryPotter = 2
  };
  
  enum class LedsOptions : uint8_t {
    TurnOn = 1,
    TurnOff = 2,
    ChangeColor = 3,
    ChangeLEDState = 4,
    ChangeRGBState = 5,
    ChangeFadeDuration = 6,
    ResetAll = 7
  };
  
  
  enum class AvailableLEDsOptions : uint8_t {
    OrangeLED = 1,
    BlueLED = 2,
    WhiteLED = 3,
    RGBLED = 4,
  };
  
  extern PowerState current_power_state;
  extern State current_state;
}


#endif