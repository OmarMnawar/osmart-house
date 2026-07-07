// Aufrufen der benutzer defenierte Bibliotheke
#include "Temperaturmesser.hpp"
// Ein int8_t var ist ein var, der nur ein wert von -127 bis 128 speicher kann. 
int8_t CelsiusTemperatur;


// Hier ist eine Methode, um den Temperator zu rechnen.
void Temperaturmesser::UpdateTemperaturmesser()
{
  const float BETA = 3950;
  _temperaturmesserWert = analogRead(TEMPERATURMESSER_PIN);
  //CelsiusTemperatur = 1 / (log(1 / (1023. / _temperaturmesserWert - 1)) / BETA + 1.0 / 298.15) - 273.15;
   CelsiusTemperatur = map(_temperaturmesserWert, 0, 358, -50, 125);

}