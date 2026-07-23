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
      NoAction = 0,
      ShowingWelcomeMessage = 1,
      Idle = 2,
      InMainMenu = 3,
      InThermometerMenu = 4,
      InMusicMenu = 5,
      InLedsMenu = 6,
      InTurnOnMenu = 7,
      InTurnOffMenu = 8,
      InChangeLEDStateMenu = 9,
      InLEDStateMenu = 10,
      InRGBLEDStateMenu = 11,
      InColorChange = 12,
      InColorRedStage = 13,
      InColorGreenStage = 14,
      InColorBlueStage = 15,
      ShuttingOff = 16
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
    LEDsState = 3,
    ChangeColor = 4,
    ChangeFadeDuration = 5,
    ResetAll = 6
  };
  
  
  enum class AvailableLEDsOptions : uint8_t {
    WhiteLED = 1,
    OrangeLED = 2,
    BlueLED = 3,
    RGBLED = 4,
  };

  enum class AvailableLEDsStatesOptions : uint8_t {
    Off = 1,
    On = 2,
    Blink = 3,
    Fade = 4
  };

  enum class AvailableRGBLEDsStatesOptions : uint8_t {
    Off = 1,
    Constant = 2,
    Blink = 3,
    Fade = 4,
    Random = 5,
    Remote = 6
  };

  enum class AvailableColorsOptions : uint8_t {
    Red = 1,
    Green = 2,
    Blue = 3,
    Cyan = 4,
    Pink = 5,
    Custom = 6
  };

  enum class ColorStages : uint8_t {
    Red = 0,
    Green = 1,
    Blue = 2
  };
  
  extern PowerState current_power_state;
  extern State current_state;
}


#endif