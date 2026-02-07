// Aufrufen alle benutzer defnierte Bibliotheken
#include "Bewegungsmelder.hpp"
#include "Infrarotfernbedienung.hpp"
#include "RGBLED.hpp"
#include "LCDI2C.hpp"
#include "Temperaturmesser.hpp"

// Hier Speicher wir den reference einer Klasse unter eine bestimmte Namen, um über dessen inhalt, die als public defeniert sind, zu zugreifen.
RGBLED _RGBLED_Instance;
Bewegungsmelder _bewegungsmelder_Instance;
Infrarotfernbedienung _infrarotfernbedienung_Instance;
LCDI2C _LCDI2C_Instance;
Temperaturmesser _temperaturmesser_Instance;

// Wird nur einmal aufgerufen.
void setup()
{
  Serial.begin(9600);
  _bewegungsmelder_Instance.InitialisiereBewegungsmelder();
  _infrarotfernbedienung_Instance.InitialisiereInfrarotfernbedienung();
  _RGBLED_Instance.InitialisiereRGBLEDS();
  _LCDI2C_Instance.InitialisiereLCD();
}

// Wird mehrmals aufgerufen.
void loop()
{
    _bewegungsmelder_Instance.UpdateBewegungsmelder();
    _infrarotfernbedienung_Instance.UpdateInfrarotfernbedienung();
    _RGBLED_Instance.RGBLEDLogic();
    _LCDI2C_Instance.LCDLogic();
    _temperaturmesser_Instance.UpdateTemperaturmesser();
}