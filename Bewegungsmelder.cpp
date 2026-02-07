// Aufrufen benutzer defenierter Bibliotheke.
#include "Bewegungsmelder.hpp"

bool BewegungsErkannt;

// Hier defenieren wir was den BEWEGUNGSMELDER_PIN gilt als.
void Bewegungsmelder::InitialisiereBewegungsmelder()
{
	pinMode(BEWEGUNGSMELDER_PIN, INPUT);
}

// Methode, um den bool wert auf die gelesen wert von den gegeben pin zu speicher.
void Bewegungsmelder::UpdateBewegungsmelder()
{
	BewegungsErkannt = digitalRead(BEWEGUNGSMELDER_PIN);
}