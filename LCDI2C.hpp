#ifndef LCDI2C_HPP // ifndef heißt: "if not defined" auf Deutsch: "Falls nicht defeniert."
#define LCDI2C_HPP // Hier defenieren wir es.

// Arduino Biblothek aufrufen, um auf verschieden funktionen zu zugreifen.
#include <Arduino.h> 
// aufruen unsere benutzer defneierte Biblotheken.
#include "Infrarotfernbedienung.hpp"
#include "Zeitmesser.h"
#include "Temperaturmesser.hpp"
#include "Lautsprecher.h"

// Aufrufen der LiquidCrystal_I2C Biblothek.
#include <LiquidCrystal_I2C.h>
// Aufrufen der Wire Biblothek. wird von "LiquidCrystal_I2C Biblothek" benötigt.
#include <Wire.h>


// extern ist ein Stichwort, den bezeichnet, dass diese var oder object wo anders defeniert wird.
extern LiquidCrystal_I2C LC_I2C_Lib; // Das hier ist ein zuweisung von der LiquidCrystal Bibliothek.

// Hier erstellen wir ein class, um zu ermöglichen, einen Instanc zu erstellen.
class LCDI2C
{
  public:
  void InitialisiereLCD();
  void LCDLogic();
};

// Hier erstellen wir einen Enum für LCD Status. 
enum LCDStatus
{
  Aus,
  An,    
};

// extern ist ein Stichwort, den bezeichnet, dass diese var oder object wo anders defeniert wird.
extern LCDStatus AktuelleLCD_Status; // Für LCD An Und Aus

// Hier erstellen wir einen weiteren Enum für LCD Aktionen.
enum LCDAktion
{
	KeineAktion,
	ZeigtGruesseText,
  ZeigtLeerlaufText,
  ImHauptmenue,
  ImTemperaturMenue,
  ImMusikMenue,
	ImRGBLEDMenue,
	AusSchalten
};

// extern ist ein Stichwort, den bezeichnet, dass diese var oder object wo anders defeniert wird.
extern LCDAktion AktuelleLCD_Aktion; // Für LCD Aktion z.B Zeige Grüße Text usw..

#endif // Ende des defenierung.

// #ifndef, #define und #endif  sind Stichwortern und es ermöglicht uns "Header Guards" zu erschaffen.
// "Header Guards" heißt, dass wir vermeiden, viele defenitionen von den selben "Header" zu erstellen.