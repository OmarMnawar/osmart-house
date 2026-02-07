#include "RGBLED.hpp"
#include "LCDI2C.hpp"
#include "Infrarotfernbedienung.hpp"
#include "Bewegungsmelder.hpp"


// RGB LED Pins. 
const byte ROT_PIN = 10;
const byte GRUEN_PIN = 9;
const byte BLAU_PIN = 6;

const byte RGBLEDS_PINS[3] = {ROT_PIN, GRUEN_PIN, BLAU_PIN};

 
void RGBLED::InitialisiereRGBLEDS()
{
  for(byte i = 0; i < sizeof(RGBLEDS_PINS) / sizeof(RGBLEDS_PINS[0]); i++)
  {
    pinMode(RGBLEDS_PINS[i], OUTPUT);
  }
}

void FarbeEinstellen(byte rotWert,byte gruenWert,byte blauWert)
{
  analogWrite(RGBLEDS_PINS[0],rotWert);
  analogWrite(RGBLEDS_PINS[1],gruenWert);
  analogWrite(RGBLEDS_PINS[2],blauWert);
}


void RGBLED::RGBLEDLogic()
{

  if(BewegungsErkannt || AktuelleFB_Taste == AnAus)
  {
    FarbeEinstellen(255,255,255);
  }

}