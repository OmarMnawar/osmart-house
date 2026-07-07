// Aufren benutzer defneierte Bibliothek.
#include "Zeitmesser.h"

unsigned long _startZeit = 0;

// Hier machen wir ein Methode, die jedes mal, wenn aufgerufen wird, wird das variable zu null setzte dann zu millis();
void StartZeitMessen()
{
  _startZeit = 0;
  _startZeit = millis();
}

// Methode die boolean wert zurück gibt.
bool ZeitAbgelaufenMitZiel(unsigned long gewuenschteZeit)
{
  return millis() - _startZeit >= gewuenschteZeit;
}