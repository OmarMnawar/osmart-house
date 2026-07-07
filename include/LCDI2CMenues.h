#ifndef LCD_I2C_MENUES_H // ifndef heißt: "if not defined" auf Deutsch: "Falls nicht defeniert."
#define LCD_I2C_MENUES_H // Hier defenieren wir es.

// Aufrufen der benutzer defneierte Bibliothek.
#include "LCDI2C.hpp"
#include "RGBLED.hpp"

// byte var als Menü.
byte _hauptmenue;
byte _musikMenue;
byte _RGBLED_Menue;

// Zu speicer der alten Temperatur.
int8_t _aktuelleCT; // ein variable wert, der enthält ein wert von -127 bis 128.

// Ein boolean, um zu sehen, ob rgb led infromation gezeigt wurde oder nicht.
bool _RGBLED_InfromationGezeigt;


bool _werkseinstellungenzurueckgesetzt;
bool _rotFarbeGewaehlt;
bool _gruenFarbeGewaehlt;
bool _blauFarbeGewaehlt;


// Hier erstellen wir die umlaute und Eszett.
const byte AE[8] = { B01010,B00000,B01110,B00001,B01111,B10001,B01111,B00000 }; // AE steht für "ä"
const byte UE[8] = { B01010,B00000,B10001,B10001,B10001,B10011,B01101,B00000 }; // UE steht für "ü"
const byte OE[8] = { B01010,B00000,B01110,B10001,B10001,B10001,B01110,B00000 }; // OE steht für "ö"
const byte ESZETT[8] = { B00000,B00000,B01110,B10001,B11110,B10001,B11110,B10000 }; // ESZETT steht für "ß"

// Hier wird eine Methode implementiert, die kontrolliert, was passiert, wenn wir die Taste "Plus" und "Minus" drücken.
void FernbedienungLesenImMenue(byte &menue)
{
  switch (AktuelleFB_Taste)
  {
    case Plus:
    menue--;
    break;

    case Minus:
    menue++;
    break;
  }

}

// Hier erstellen wir den Hauptmenü.
void Hauptmenue()
{
  // Hier erstellen wir den Zeiger.
  /*Nummer 1*/ const byte MENUE_ZEIGER[8] = { B11000,B11100,B11110,B11111,B11111,B11110,B11100,B11000 };
  /*        */ LC_I2C_Lib.createChar(0,MENUE_ZEIGER);

  // Hier erstellen wir die Säule, die gefüllt ist.
  /*Nummer 2*/ const byte MENUE_SAEULE[8] = { B11111,B11111,B11111,B11111,B11111,B11111,B11111,B11111 };
  /*        */ LC_I2C_Lib.createChar(1,MENUE_SAEULE);

  // Hier erstellen wir die Säule, die oben geöffnet ist.
  /*Nummer 3*/ const byte MENUE_SAEULE2[8] = { B10001,B10001,B10001,B10001,B11111,B11111,B11111,B11111 };
  /*        */ LC_I2C_Lib.createChar(2,MENUE_SAEULE2);

  // Hier erstellen wir die Säule, die unten geöffnet ist.
  /*Nummer 4*/ const byte MENUE_SAEULE3[8] = { B11111,B11111,B11111,B11111,B10001,B10001,B10001,B10001 };
  /*        */ LC_I2C_Lib.createChar(3,MENUE_SAEULE3);

  // Von Nummer 2 bis Nummer 3: diese dienen nur zu zeigen, wo wir in der Menü sind. sie können es sehen auf der rechte seite des Menü.
  
  LC_I2C_Lib.createChar(4,AE); // hier speicher unsere custom Char in nummer 4.
  LC_I2C_Lib.createChar(5,UE); // hier speicher unsere custom Char in nummer 4.
  LC_I2C_Lib.createChar(6,OE); // hier speicher unsere custom Char in nummer 6.
  LC_I2C_Lib.createChar(7,ESZETT); // hier speicher unsere custom Char in nummer 7.


  FernbedienungLesenImMenue(_hauptmenue);

  switch (_hauptmenue)
  {
    case 0:
    _hauptmenue = 1;
    break;

    case 1:
    LC_I2C_Lib.setCursor(0,0);
    LC_I2C_Lib.write(0);
    LC_I2C_Lib.print("Temperatur");
    LC_I2C_Lib.setCursor(0,1);
    LC_I2C_Lib.print(" Musik");
    LC_I2C_Lib.setCursor(15,0);
    LC_I2C_Lib.write(2);
    LC_I2C_Lib.setCursor(15,1);
    LC_I2C_Lib.write(1);
    break;

    case 2:
    LC_I2C_Lib.setCursor(0,0);
    LC_I2C_Lib.print(" Temperatur");
    LC_I2C_Lib.setCursor(0,1);
    LC_I2C_Lib.write(0);
    LC_I2C_Lib.print("Musik");
    LC_I2C_Lib.setCursor(15,0);
    LC_I2C_Lib.write(2);
    LC_I2C_Lib.setCursor(15,1);
    LC_I2C_Lib.write(1);
    break;

    // Hier wird die LCD von Text gereignt und die richtige menü gezeigt.
    case 3:
    if (AktuelleFB_Taste == Plus)
    {
      LC_I2C_Lib.clear();
      _hauptmenue = 2;
    }
    else if (AktuelleFB_Taste == Minus)
    {
      LC_I2C_Lib.clear();
      _hauptmenue = 4;
    }
    break;

    case 4:
    LC_I2C_Lib.setCursor(0,0);
    LC_I2C_Lib.write(0);
    LC_I2C_Lib.print("RGB LED");
    LC_I2C_Lib.setCursor(0,1);
    LC_I2C_Lib.print(" Ausschalten");
    LC_I2C_Lib.setCursor(15,0);
    LC_I2C_Lib.write(3);
    LC_I2C_Lib.setCursor(15,1);
    LC_I2C_Lib.write(2);
    break;

    case 5:
    LC_I2C_Lib.setCursor(0,0);
    LC_I2C_Lib.print(" RGB LED");
    LC_I2C_Lib.setCursor(0,1);
    LC_I2C_Lib.write(0);
    LC_I2C_Lib.print("Ausschalten");
    LC_I2C_Lib.setCursor(15,0);
    LC_I2C_Lib.write(1);
    LC_I2C_Lib.setCursor(15,1);
    LC_I2C_Lib.write(3);
    break;

    case 6:
    _hauptmenue = 5;
    break;

    default:
    _hauptmenue = 1;
    break;
  }
}

// Hier erstellen wir den Temeperatur Menü.
void TemperaturMenue()
{
  LC_I2C_Lib.setCursor(0,0);
  LC_I2C_Lib.print("Temperatur: ");
  LC_I2C_Lib.setCursor(0,1);
  LC_I2C_Lib.print(CelsiusTemperatur);
  LC_I2C_Lib.write(0xDF); // Um dieses Char "°" zu zeigen. sonst es wird falsch auf Lcd gezeigt.
  LC_I2C_Lib.print("C");

  // Hier speicher wir den alten wert der Temperatur, damit wir die LCD nur reinigen, wenn es geändert wird.
  if (CelsiusTemperatur != _aktuelleCT)
  {
    _aktuelleCT = CelsiusTemperatur;
    delay(500);

    LC_I2C_Lib.clear();
  }

}


// Hier erstellen wir die Musik Menü.
void MusikMenue()
{
  FernbedienungLesenImMenue(_musikMenue);
  
  switch (_musikMenue)
  {
    case 0:
    _musikMenue = 1;
    break;

    case 1:
    LC_I2C_Lib.setCursor(0,0);
    LC_I2C_Lib.write(0);
    LC_I2C_Lib.print("F");
    LC_I2C_Lib.write(5);
    LC_I2C_Lib.print("r Elise");
    LC_I2C_Lib.setCursor(0,1);
    LC_I2C_Lib.print(" Harry Potter");

    if (AktuelleFB_Taste == BestaeSpielPaus)
    {   
      LC_I2C_Lib.clear();
      StartZeitMessen();
      while (!ZeitAbgelaufenMitZiel(3000))
      {
        LC_I2C_Lib.setCursor(3,0);
        LC_I2C_Lib.print("Musik wird");
        LC_I2C_Lib.setCursor(0,1);
        LC_I2C_Lib.print("gleich gespielt.");
      }
      LC_I2C_Lib.clear();
      LC_I2C_Lib.setCursor(3,0);
      LC_I2C_Lib.print("F");
      LC_I2C_Lib.write(5);
      LC_I2C_Lib.print("r Elise");
      LC_I2C_Lib.setCursor(1,1);
      LC_I2C_Lib.print("wird Gespielt.");
      FuerElise();
      LC_I2C_Lib.clear();
      StartZeitMessen();
      while (!ZeitAbgelaufenMitZiel(3000))
      {
        LC_I2C_Lib.setCursor(2,0);
        LC_I2C_Lib.print("Musik wurde");
        LC_I2C_Lib.setCursor(4,1);
        LC_I2C_Lib.print("gespielt");
      }
      LC_I2C_Lib.clear();
      _musikMenue = 1;

    }
    break;

    case 2:
    LC_I2C_Lib.setCursor(0,0);
    LC_I2C_Lib.print(" F");
    LC_I2C_Lib.write(5); 
    LC_I2C_Lib.print("r Elise");
    LC_I2C_Lib.setCursor(0,1);
    LC_I2C_Lib.write(0);
    LC_I2C_Lib.print("Harry Potter");

    if (AktuelleFB_Taste == BestaeSpielPaus)
    {   
      LC_I2C_Lib.clear();
      StartZeitMessen();
      while (!ZeitAbgelaufenMitZiel(3000))
      {
        LC_I2C_Lib.setCursor(3,0);
        LC_I2C_Lib.print("Musik wird");
        LC_I2C_Lib.setCursor(0,1);
        LC_I2C_Lib.print("gleich gespielt.");
      }
      LC_I2C_Lib.clear();
      LC_I2C_Lib.setCursor(2,0);
      LC_I2C_Lib.print("Harry Potter");
      LC_I2C_Lib.setCursor(1,1);
      LC_I2C_Lib.print("wird Gespielt.");
      HarryPotter();
      LC_I2C_Lib.clear();
      StartZeitMessen();
      while (!ZeitAbgelaufenMitZiel(3000))
      {
        LC_I2C_Lib.setCursor(2,0);
        LC_I2C_Lib.print("Musik wurde");
        LC_I2C_Lib.setCursor(4,1);
        LC_I2C_Lib.print("gespielt");
      }
      LC_I2C_Lib.clear();
      _musikMenue = 1;
    }
    break;
    
    case 3:
    _musikMenue = 2;
    break;

    default:
    _musikMenue = 1;
    break;
    
  }
}


// Hier erstellen wir die RGBLED Menü.
void RGBLEDMenue()
{
  const unsigned long INFROMATION_ZEIT = 2500;
  const unsigned long NACHRICHTEN_ZEIT = 2000;

  if(!_RGBLED_InfromationGezeigt)
  {
    StartZeitMessen();
    while (!ZeitAbgelaufenMitZiel(INFROMATION_ZEIT)) 
    {
      LC_I2C_Lib.setCursor(4,0);
      LC_I2C_Lib.print("Mit der");
      LC_I2C_Lib.setCursor(1,1);
      LC_I2C_Lib.print("Fernbedienung");
    }
    LC_I2C_Lib.clear();

    StartZeitMessen();
    while (!ZeitAbgelaufenMitZiel(INFROMATION_ZEIT))
    {
      LC_I2C_Lib.setCursor(2,0);
      LC_I2C_Lib.print("W");
      LC_I2C_Lib.write(4);
      LC_I2C_Lib.print("hle eine");
      LC_I2C_Lib.setCursor(5,1);
      LC_I2C_Lib.print("Farbe.");
    }
    LC_I2C_Lib.clear();

    StartZeitMessen();
    while (!ZeitAbgelaufenMitZiel(INFROMATION_ZEIT))
    {
      LC_I2C_Lib.setCursor(2,0);
      LC_I2C_Lib.print("Nummer Eins:");
      LC_I2C_Lib.setCursor(5,1);
      LC_I2C_Lib.print("Rot");
    }
    LC_I2C_Lib.clear();

    StartZeitMessen();
    while (!ZeitAbgelaufenMitZiel(INFROMATION_ZEIT))
    {
      LC_I2C_Lib.setCursor(2,0);
      LC_I2C_Lib.print("Nummer Zwei:");
      LC_I2C_Lib.setCursor(5,1);
      LC_I2C_Lib.print("Gr");
      LC_I2C_Lib.write(5);
      LC_I2C_Lib.print("n");
    }
    LC_I2C_Lib.clear();

    StartZeitMessen();
    while (!ZeitAbgelaufenMitZiel(INFROMATION_ZEIT))
    {
      LC_I2C_Lib.setCursor(2,0);
      LC_I2C_Lib.print("Nummer Drei:");
      LC_I2C_Lib.setCursor(5,1);
      LC_I2C_Lib.print("Blau");
    }
    LC_I2C_Lib.clear();

    StartZeitMessen();
    while (!ZeitAbgelaufenMitZiel(INFROMATION_ZEIT))
    {
      LC_I2C_Lib.setCursor(2,0);
      LC_I2C_Lib.print("Nummer Null");
      LC_I2C_Lib.setCursor(1,1);
      LC_I2C_Lib.print("Zur");
      LC_I2C_Lib.write(5);
      LC_I2C_Lib.print("cksetzen");
    }
    LC_I2C_Lib.clear();
    _RGBLED_InfromationGezeigt = true;
  }
  else 
  {
    LC_I2C_Lib.setCursor(0,0);
    LC_I2C_Lib.print("Farbe W");
    LC_I2C_Lib.write(4);
    LC_I2C_Lib.print("hlen:");
    if (!_rotFarbeGewaehlt && AktuelleFB_Taste == Nummer1)
    {
      FarbeEinstellen(255,0,0);
      _rotFarbeGewaehlt = true;
    }
    else if (!_gruenFarbeGewaehlt && AktuelleFB_Taste == Nummer2)
    {
      FarbeEinstellen(0,255,0);
      _gruenFarbeGewaehlt = true;
    }
    else if (!_blauFarbeGewaehlt && AktuelleFB_Taste == Nummer3)
    {
      FarbeEinstellen(0,0,255);
      _blauFarbeGewaehlt = true;
    }
    else if(!_werkseinstellungenzurueckgesetzt && AktuelleFB_Taste == Nummer4)
    {
      FarbeEinstellen(255,255,255);
      _werkseinstellungenzurueckgesetzt = true;
    }
    else if(_rotFarbeGewaehlt || _gruenFarbeGewaehlt || _blauFarbeGewaehlt)
    {
      LC_I2C_Lib.clear();
      StartZeitMessen();
      while(!ZeitAbgelaufenMitZiel(NACHRICHTEN_ZEIT))
      {
        LC_I2C_Lib.setCursor(2,0);
        LC_I2C_Lib.print("Farbe wurde");
        LC_I2C_Lib.setCursor(4,1);
        LC_I2C_Lib.print("Genommen.");
      }
      LC_I2C_Lib.clear();
      _rotFarbeGewaehlt = false;
      _gruenFarbeGewaehlt = false;
      _blauFarbeGewaehlt = false;
      _werkseinstellungenzurueckgesetzt = false;
      _RGBLED_InfromationGezeigt = false;
      AktuelleLCD_Aktion = ImHauptmenue;
    }
    else if (_werkseinstellungenzurueckgesetzt)
    {
      LC_I2C_Lib.clear();
      StartZeitMessen();
      while(!ZeitAbgelaufenMitZiel(NACHRICHTEN_ZEIT))
      {
        LC_I2C_Lib.setCursor(2,0);
        LC_I2C_Lib.print("Farbe zur");
        LC_I2C_Lib.write(5);
        LC_I2C_Lib.print("ck");
        LC_I2C_Lib.setCursor(4,1);
        LC_I2C_Lib.print("Gesetzt.");
      }
      LC_I2C_Lib.clear();
      _rotFarbeGewaehlt = false;
      _gruenFarbeGewaehlt = false;
      _blauFarbeGewaehlt = false;
      _werkseinstellungenzurueckgesetzt = false;
      _RGBLED_InfromationGezeigt = false;
      AktuelleLCD_Aktion = ImHauptmenue;
    }

  }

}

// Hier wird kontrolliert, was passiert,wenn wir den Bestätigung Taste drücken.
void AuswaehlBestaetigen()
{
  if (AktuelleFB_Taste == BestaeSpielPaus)
  {
    LC_I2C_Lib.clear();

    switch (_hauptmenue)
    {
      case 0:
      // Hier muss nicht gemacht werden, weil _hauptmenue wert 0 auf 1 automatisch zugewiesen wird.
      break;

      case 1:
      AktuelleLCD_Aktion = ImTemperaturMenue;
      break;

      case 2:
      AktuelleLCD_Aktion = ImMusikMenue;
      break;

      case 3:
      // Hier muss nichts gemacht werden, weil _hauptmenue wert 3 benutzt wird, um das Menü zu reinigen.
      break;

      case 4:
      AktuelleLCD_Aktion = ImRGBLEDMenue;
      break;

      case 5:
      AktuelleLCD_Aktion = AusSchalten;
      _hauptmenue = 1; /* wir setzen Hauptmenü manuelle auf 1, um zu verhindern, nach dem Anschalten auf dem
       verlassenen wert zurückzugehen. */
      break;

      case 6:
      // Hier muss nicht gemacht werden, weil _hauptmenue wert 6 auf 5 automatisch zugewiesen wird.
      break;

      // Hier wird das wert auf keineAktion gesetzt, falls keine von oben cases stimmt.
      default:
      AktuelleLCD_Aktion = KeineAktion;
      break;
    }
  }
}

#endif // Ende des defenierung.

// #ifndef, #define und #endif  sind Stichwortern und es ermöglicht uns "Header Guards" zu erschaffen.
// "Header Guards" heißt, dass wir vermeiden, viele defenitionen von den selben "Header" zu erstellen.