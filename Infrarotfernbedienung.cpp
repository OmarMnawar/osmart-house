// Aufrufen Arduino Bibliothek.
#include <IRremote.hpp>

// Aufrufen alle benutzer defenierte Bibliotheken.
#include "Infrarotfernbedienung.hpp"
#include "Zeitmesser.h"

unsigned int _fbTasteWert; // usigned int hält werte von 0 bis circa. 64000 <-- dieses Wert kann ungenau sein.

// Hier bezeichnen wir den Compiler, dass Enum,die von den Datei "Infrarotfernbedienung.hpp" sich befienden, hier defeniert wird.
FB_Tasten AktuelleFB_Taste;

void Infrarotfernbedienung::InitialisiereInfrarotfernbedienung()
{	
	IrReceiver.begin(FERNBEDIENUNG_PIN);
}

// Hier ist eine Methode, um die werte der Infrarotfernbedienung zu updaten.
void Infrarotfernbedienung::UpdateInfrarotfernbedienung()
{
	if(IrReceiver.decode())
	{
		_fbTasteWert = IrReceiver.decodedIRData.command;

		switch(_fbTasteWert)
		{
			case 0xA2:
			AktuelleFB_Taste = AnAus;
			StartZeitMessen();
			break;
			
			case 0xE2:
			AktuelleFB_Taste = Menue;
			StartZeitMessen();
			break;			
			
			case 0x2:
			AktuelleFB_Taste = Plus;
			StartZeitMessen();
			break;
			
			case 0xC2:
			AktuelleFB_Taste = Zurueck;
			StartZeitMessen();
			break;
			
			
			case 0xA8:
			AktuelleFB_Taste = BestaeSpielPaus;
			StartZeitMessen();
			break;
			
			
			case 0x68:
			AktuelleFB_Taste = Nummer0;
			StartZeitMessen();
			break;
			
			case 0x98:
			AktuelleFB_Taste = Minus;
			StartZeitMessen();
			break; 


			case 0x30:
			AktuelleFB_Taste = Nummer1;
			StartZeitMessen();
			break;

			case 0x18:
			AktuelleFB_Taste = Nummer2;
			StartZeitMessen();
			break;

			case 0x7A:
			AktuelleFB_Taste = Nummer3;
			StartZeitMessen();
			break;

      			
      case 0x10:
			AktuelleFB_Taste = Nummer4;
			StartZeitMessen();
			break;

		}
		Serial.println(_fbTasteWert,HEX);
		IrReceiver.resume();
	}
	
	if(AktuelleFB_Taste != KeineTaste && ZeitAbgelaufenMitZiel(10))
	{
		AktuelleFB_Taste = KeineTaste;
	}

}
