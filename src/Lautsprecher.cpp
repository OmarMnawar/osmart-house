// Aufrufen alle benutzer defenierte Bibliotheke.
#include "Lautsprecher.h"
#include "Pitches.h"
#include "Zeitmesser.h"

const byte LAUTSPRECHER_PIN = 2;

#define USE_IRREMOTE_HPP_AS_PLAIN_INCLUDE // so verhindern wir einen defnition Fehelr.

// dann rufen wir die Bibliothek auf.
#include <IRremote.hpp>

// melody für den lcd wenn es gestartet wird.
void LCDAnMelody()
{
	const unsigned long ZEIT = 320;

	//IrReceiver.stop();
	tone(LAUTSPRECHER_PIN,NOTE_C4);
	delay(ZEIT);
	noTone(LAUTSPRECHER_PIN);
	
	tone(LAUTSPRECHER_PIN,NOTE_E4);
	delay(ZEIT);
	noTone(LAUTSPRECHER_PIN);
	//IrReceiver.start(8000);
}

// Melody für den lcd wenn es ausgeschaltet wird.
void LCDAusMelody()
{
	const unsigned long ZEIT = 320;
	//IrReceiver.stop();
	tone(LAUTSPRECHER_PIN,NOTE_E4);
	delay(ZEIT);
	noTone(LAUTSPRECHER_PIN);

	tone(LAUTSPRECHER_PIN,NOTE_C4);
	delay(ZEIT);
	noTone(LAUTSPRECHER_PIN);
	//IrReceiver.start(8000);

}

// Für Elise Lied.
void FuerElise()
{
	//IrReceiver.stop();
	for (int thisNote = 0; thisNote < _FEnotes * 2; thisNote = thisNote + 2)
	{
		// calculates the duration of each note
		_FEdivider = pgm_read_word_near(FUER_ELISE_MELODY + thisNote + 1);
		if (_FEdivider > 0)
		{
			// regular note, just proceed
			_FEnoteDuration = (_FEwholenote) / _FEdivider;
		} 
		else if (_FEdivider < 0)
		{
			// dotted notes are represented with negative durations!!
			_FEnoteDuration = (_FEwholenote) / abs(_FEdivider);
			_FEnoteDuration *= 1.5; // increases the duration in half for dotted notes
		}

		StartZeitMessen();
		
		// we only play the note for 90% of the duration, leaving 10% as a pause
		tone(LAUTSPRECHER_PIN, pgm_read_word_near(FUER_ELISE_MELODY + thisNote), _FEnoteDuration * 0.9);
		// Wait for the specief duration before playing the next note.

		while (!ZeitAbgelaufenMitZiel(_FEnoteDuration))
		{		
			// Hier mussen wir nicht machen nur warten.
			// in anderem worten hier benutzen wir millis();
			// statt delay. besser wenn wir viele sachen gleich zeitig
			// machen möchten.
		}
		
		// stop the waveform generation before the next note.
		noTone(LAUTSPRECHER_PIN);
		
	}
	noTone(LAUTSPRECHER_PIN);
	//IrReceiver.start(8000);

}

// Harry Potter Lied.
void HarryPotter()
{
	// iterate over the notes of the melody. 
  // Remember, the array is twice the number of notes (notes + durations)
	//IrReceiver.stop();
	
  for(int thisNote = 0; thisNote < _HPnotes * 2; thisNote = thisNote + 2)
	{

    // calculates the duration of each note
    _HPdivider = HARRY_POTTER_MELODY[thisNote + 1];
    if (_HPdivider > 0)
		{
      // regular note, just proceed
      _HPnoteDuration = (_HPwholenote) / _HPdivider;
    }
		else if (_HPdivider < 0)
		{
      // dotted notes are represented with negative durations!!
      _HPnoteDuration = (_HPwholenote) / abs(_HPdivider);
      _HPnoteDuration *= 1.5; // increases the duration in half for dotted notes
    }
		
		StartZeitMessen();
    
		// we only play the note for 90% of the duration, leaving 10% as a pause
    tone(LAUTSPRECHER_PIN, HARRY_POTTER_MELODY[thisNote], _HPnoteDuration*0.9);
		// Wait for the specief duration before playing the next note.

		while (!ZeitAbgelaufenMitZiel(_HPnoteDuration))
		{		
			// Hier mussen wir nicht machen nur warten.
			// in anderem worten hier benutzen wir millis();
			// statt delay. besser wenn wir viele sachen gleich zeitig
			// machen möchten.
		}
    
    // stop the waveform generation before the next note.
    noTone(LAUTSPRECHER_PIN);
  }
	noTone(LAUTSPRECHER_PIN);
	//IrReceiver.start(8000);
}