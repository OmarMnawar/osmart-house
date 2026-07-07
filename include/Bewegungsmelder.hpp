#ifndef BEWEGUNGSMELDER_HPP // ifndef heißt: "if not defined" auf Deutsch: "Falls nicht defeniert."
#define BEWEGUNGSMELDER_HPP // Hier defenieren wir es.

// Aufrufen Arduino Bibliothek.
#include <Arduino.h>


// Hier erstellen wir ein class, um zu ermöglichen, einen instanc zu erstellen.
class Bewegungsmelder
{
	private:
	const byte BEWEGUNGSMELDER_PIN = 8;

	public:
	void InitialisiereBewegungsmelder();
	void UpdateBewegungsmelder();

};

extern bool BewegungsErkannt;


#endif // Ende des defenierung.

// #ifndef, #define und #endif  sind Stichwortern und es ermöglicht uns "Header Guards" zu erschaffen.
// "Header Guards" heißt, dass wir vermeiden, viele defenitionen von den selben "Header" zu erstellen.