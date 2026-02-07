#ifndef ZEITMESSER_H // ifndef heißt: "if not defined" auf Deutsch: "Falls nicht defeniert."
#define ZEITMESSER_H // Hier defenieren wir es.


// Aufrufen Arduino Bibliotheken.
#include <Arduino.h>


// Hier notivieren wir den Kompiler, dass diese Methode und Variable wo anders definiert wird.
extern void StartZeitMessen();
extern bool ZeitAbgelaufenMitZiel(unsigned long gewuenschteZeit);

#endif // Ende des defenierung.

// #ifndef, #define und #endif  sind Stichwortern und es ermöglicht uns "Header Guards" zu erschaffen.
// "Header Guards" heißt, dass wir vermeiden, viele defenitionen von den selben "Header" zu erstellen.