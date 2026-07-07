#ifndef LAUTSPRECHER_H // ifndef heißt: "if not defined" auf Deutsch: "Falls nicht defeniert."
#define LAUTSPRECHER_H // Hier defenieren wir es.

#include <Arduino.h> // Aufrufen der Arduino Biblothek, damit wir auf verschiedene vars zugreifen können.

// extern ist ein Stichwort, den bezeichnet, dass diese var oder object wo anders defeniert wird.
extern void LCDAnMelody();
extern void LCDAusMelody();

// extern ist ein Stichwort, den bezeichnet, dass diese var oder object wo anders defeniert wird.
extern void FuerElise();
extern void HarryPotter();

#endif // ende des defenierung.

// #ifndef, #define und #endif ermöglicht uns "Header Guards" zu erschaffen.
// "Header Guards" heißt, dass wir vermeiden, viele defenitionen von den selben "Header" zu erstellen.