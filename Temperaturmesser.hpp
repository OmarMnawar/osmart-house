#ifndef TEMPERATUR_MESSER_HPP // ifndef heißt: "if not defined" auf Deutsch: "Falls nicht defeniert."
#define TEMPERATUR_MESSER_HPP // Hier defenieren wir es.


// Aufrufen der Arduino Bibliothek.
#include <Arduino.h>

// Hier erstellen wir ein class, um zu ermöglichen, einen instanc zu erstellen.
class Temperaturmesser
{
  private:
  const byte TEMPERATURMESSER_PIN = A0; // analog input pin 0
  unsigned int _temperaturmesserWert;

  public:
  void UpdateTemperaturmesser();
};

extern int8_t CelsiusTemperatur;




#endif // Ende des defenierung.

// #ifndef, #define und #endif  sind Stichwortern und es ermöglicht uns "Header Guards" zu erschaffen.
// "Header Guards" heißt, dass wir vermeiden, viele defenitionen von den selben "Header" zu erstellen.