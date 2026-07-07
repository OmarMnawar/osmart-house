#ifndef INFRAROTFERNBEDIENUNG_HPP // ifndef heißt: "if not defined" auf Deutsch: "Falls nicht defeniert."
#define INFRAROTFERNBEDIENUNG_HPP // Hier defenieren wir es.

// Hier erstellen wir ein class, um zu ermöglichen, einen instanc zu erstellen.
class Infrarotfernbedienung
{
	private:
	const byte FERNBEDIENUNG_PIN = 11;

	public:
	void InitialisiereInfrarotfernbedienung();
	void UpdateInfrarotfernbedienung();

};

// Hier ist ein Enum, den für die Fernbedienung Tasten zu ständig ist.
enum FB_Tasten
{
	KeineTaste,
	AnAus,
	Menue,
	Test,
	Plus,
	Zurueck,
	SchnellerRuecklauf,
	BestaeSpielPaus,
	SchnellerVorlauf,
	Nummer0,
	Minus,
	Loeschen,
	Nummer1,
	Nummer2,
	Nummer3,
	Nummer4,
	Nummer5,
	Nummer6,
	Nummer7,
	Nummer8,
	Nummer9,
};

// extern ist ein Stichwort, den bezeichnet, dass diese var oder object wo anders defeniert wird.
extern FB_Tasten AktuelleFB_Taste; // Für Fernbedienung.

#endif // Ende des defenierung.

// #ifndef, #define und #endif  sind Stichwortern und es ermöglicht uns "Header Guards" zu erschaffen.
// "Header Guards" heißt, dass wir vermeiden, viele defenitionen von den selben "Header" zu erstellen.
