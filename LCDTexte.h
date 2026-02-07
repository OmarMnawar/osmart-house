// Aufrufen der benutzer defenierte Bibliothek.
#include "LCDI2C.hpp"

// Hier ist eine Methode, die Grüße Text zeigt, wenn die LCD Angeschaltet wird.
void GruesseText()
{
  const byte WORT_END_POSITION = 16;
  const unsigned long ZEIT1 = 170;
  const unsigned long ZEIT2 = 320;

  const String TEXT = "Willkommen.";
  const char BUCHSTABEN[11] = { 'W','i','l','l','k','o','m','m','e','n','.' };
  const byte ARRAY_GROSS = sizeof(BUCHSTABEN) / sizeof(BUCHSTABEN[0]);

  for(byte buchstabe = 0; buchstabe < ARRAY_GROSS; buchstabe++)
  {
    StartZeitMessen();
    while (!ZeitAbgelaufenMitZiel(ZEIT1)) 
    {
      LC_I2C_Lib.setCursor(buchstabe,0);
      LC_I2C_Lib.print(BUCHSTABEN[buchstabe]);
    }
  }

  for(byte wortStartPosition = 0; wortStartPosition <= WORT_END_POSITION; wortStartPosition++)
  {
    StartZeitMessen();
    while(!ZeitAbgelaufenMitZiel(ZEIT2)) 
    {
      LC_I2C_Lib.setCursor(wortStartPosition,0);
      LC_I2C_Lib.print(TEXT);

    }
    LC_I2C_Lib.clear();
  }

}

// Hier ist eine Methode, die ein Text zeigt, wenn der benutzer nicht im Menü ist oder den Fernbedienung benutzt.
void LeerlaufText()
{
  const String ERSTE_WORT = "Benutze der";
  const String ZWEITE_WORT = "Fernbedienung.";
  const unsigned long ZEIT = 320;

  StartZeitMessen();
  while (!ZeitAbgelaufenMitZiel(ZEIT))
  {
    LC_I2C_Lib.setCursor(2,0);
    LC_I2C_Lib.print(ERSTE_WORT);
    LC_I2C_Lib.setCursor(1,1);
    LC_I2C_Lib.print(ZWEITE_WORT);
  }
  StartZeitMessen();
  while (!ZeitAbgelaufenMitZiel(ZEIT))
  {
    LC_I2C_Lib.clear();
  }
}