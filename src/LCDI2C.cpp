// Aufrufen alle benutzer defenierte Bibliotheken.
#include "LCDI2C.hpp"
#include "Infrarotfernbedienung.hpp"
#include "Bewegungsmelder.hpp"
#include "LCDTexte.h"
#include "LCDI2CMenues.h"
#include "Lautsprecher.h"

// Bytes, um den LCD zu defenieren.
const byte LAENGE = 16;
const byte BREITE = 2;
const byte ADRESSE = 0x27;

// Bools, um zu bestimmen, wenn etwas passiert schon oder nicht.
bool _anMelodyGespielt;
bool _ausMelodyGespielt;
bool _gruesseTextGezeigt;
bool SchonGedrueckt;



// Hier bezeichnen wir den Compiler, dass die Enums,die von den Datei "LCDI2C.hpp" sich befienden, hier defeniert werden.
LCDStatus AktuelleLCD_Status;
LCDAktion AktuelleLCD_Aktion;

// Hier bezeichnen wir den Compiler, dass die Zuweisung von der "LiquidCrystal Biblothek",
// die von den Datei "LCDI2C.hpp" sich befienden, hier defeniert werden.
LiquidCrystal_I2C LC_I2C_Lib(ADRESSE,LAENGE,BREITE);

// Hier ist eine Methode, um den lcd zu initialisieren.
void LCDI2C::InitialisiereLCD()
{  
  LC_I2C_Lib.init();
}

// Hier ist eine Methode, um den An- und Aus-Zustand des LCDs durch den Bewegungsmelder oder die Fernbedienung zu definieren.
void AnAusLogic()
{
  // Hier wenn die taste gedrückt ist. machen wir ein bool true. so kann das LCD An so lange wie wir wollen.
  if(BewegungsErkannt || AktuelleFB_Taste == AnAus)
  {
    AktuelleLCD_Status = An;
  }

}

// Hier ist eine Methode, um den Status des LCDS zwischen An und Aus zu handeln.
void Status()
{
  switch (AktuelleLCD_Status)
  {
    case Aus:
    AktuelleLCD_Aktion = KeineAktion;
    LC_I2C_Lib.clear();
    LC_I2C_Lib.noBacklight();   

    // Hier prüfen wir, ob an melody schon gespielt oder nicht.
    if(_anMelodyGespielt)
    {
      //LCDAusMelody();
      _ausMelodyGespielt = true;
    }
    _anMelodyGespielt = false;
    _gruesseTextGezeigt = false;
    break;
    
    case An:
    LC_I2C_Lib.backlight();
    
    // Hier prüfen wir, ob an melody nicht gespielt ist.
    if(!_anMelodyGespielt)
    {
      //LCDAnMelody();
      _anMelodyGespielt = true;
    }
    _ausMelodyGespielt = false;
    
    if(!_gruesseTextGezeigt)
    {
      AktuelleLCD_Aktion = ZeigtGruesseText;
    }
    break;

    // Falls keiner stimmt. nimmt den enum automatisch einen wert.
    default:
    AktuelleLCD_Status = Aus;
    break;
    
  }
}

// Hier erstellen wir eine Methode, die handelt, was passiert.
void Aktion()
{
  switch (AktuelleLCD_Aktion)
  {
    case ZeigtGruesseText:
    GruesseText();
    _gruesseTextGezeigt = true;
    AktuelleLCD_Aktion = ZeigtLeerlaufText;
    break;
    
    case ZeigtLeerlaufText:
    LeerlaufText();

    // zu Hauptmenue gehen.
    if(AktuelleFB_Taste == Menue)
    {
      LC_I2C_Lib.clear();
      AktuelleLCD_Aktion = ImHauptmenue;
    }
    break;
    
    case ImHauptmenue:
    Hauptmenue();
    AuswaehlBestaetigen();
    
    // Gehen wir zurück zu den leerlauf Zustand.
    if(AktuelleFB_Taste == Zurueck)
    {
      LC_I2C_Lib.clear();
      AktuelleLCD_Aktion = ZeigtLeerlaufText;
    }
    break;
    
    case ImTemperaturMenue:
    TemperaturMenue();
    
    // zu Hauptmenue gehen.
    if (AktuelleFB_Taste == Zurueck)
    {
      LC_I2C_Lib.clear();
      AktuelleLCD_Aktion = ImHauptmenue;
    }
    break;

    case ImMusikMenue:
    MusikMenue();
    
    // zu Hauptmenue gehen.
    if (AktuelleFB_Taste == Zurueck)
    {
      LC_I2C_Lib.clear();
      AktuelleLCD_Aktion = ImHauptmenue;
    }
    break;
    
    case ImRGBLEDMenue:
    RGBLEDMenue();

    if(AktuelleFB_Taste == Zurueck)
    {
      LC_I2C_Lib.clear();
      AktuelleLCD_Aktion = ImHauptmenue;
    }
    break;

    // Hier ist ein bisschen anders. Hier schlaten wir den lcd aus. wenn Aus Schlaten Im Hauptmenü gedruckt wird.
    case AusSchalten:
    StartZeitMessen();
    
    while(!ZeitAbgelaufenMitZiel(4000))
    {
      LC_I2C_Lib.setCursor(4,0);
      LC_I2C_Lib.print("LCD Wird");
      LC_I2C_Lib.setCursor(1,1);
      LC_I2C_Lib.print("ausgeschaltet.");
    }
    AktuelleLCD_Status = Aus;
    FarbeEinstellen(0,0,0);
    break;
    
    // Falls keiner stimmt. nimmt den enum automatisch einen wert.
    default:
    AktuelleLCD_Aktion = KeineAktion;
    break;
  }

}


// Hier ist eine Methode, die alle oben Methoden aufruft.
// Diese macht es möglich, alle oben Methode in einer Methode zu rufen.
void LCDI2C::LCDLogic()
{
  AnAusLogic();
  Status();
  Aktion();
}