#ifndef RGB_LED_HPP // ifndef heißt: "if not defined" auf Deutsch: "Falls nicht defeniert."
#define RGB_LED_HPP // Hier defenieren wir es.

#include <Arduino.h>

// Hier erstellen wir ein class, um zu ermöglichen, einen instanc zu erstellen.
class RGBLED
{
  public:
  void InitialisiereRGBLEDS();
  void RGBLEDLogic();

};

// Hier notivieren wir den Kompiler, dass diese Methode wo anders implementiert wird.
extern void FarbeEinstellen(byte rotWert,byte gruenWert,byte blauWert);

#endif // Ende des defenierung.

// #ifndef, #define und #endif  sind Stichwortern und es ermöglicht uns "Header Guards" zu erschaffen.
// "Header Guards" heißt, dass wir vermeiden, viele defenitionen von den selben "Header" zu erstellen.