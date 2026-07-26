#include "speaker.hpp"
#include "Arduino.h"
#include "inputs.hpp"
#include "pitches.hpp"

namespace speaker {

    states current_state = states::no_sound;
    uint32_t sound_time_elapsed = 0;
    uint32_t play_time_duration = 320;

    void play_sound(uint32_t frequency);
    void play_lcd_on_sound();
    void play_lcd_off_sound();
    void play_error_sound();
    void play_sucess_sound();

    void handle_sounds_playing() {
        
        switch(current_state) {

            case states::no_sound:
                // OM: Do nothing.
            return;

            case states::playing_lcd_on_sound:
                play_sound(pitches::NOTE_E4);
            break;

            case states::playing_lcd_off_sound:
                play_sound(pitches::NOTE_C4);
            break;

            case states::playing_error_sound:
                play_sound(pitches::NOTE_D3);
            break;

            case states::playing_success_sound:
                play_sound(pitches::NOTE_A5);
            break;
            }
    }
    
    
    void play_sound(uint32_t frequency) {
        if (sound_time_elapsed == 0) {
            sound_time_elapsed = millis();
        }
    
        tone(pins::SPEAKER, frequency);
    
        if (millis() - sound_time_elapsed >= play_time_duration) {
            noTone(pins::SPEAKER);
            sound_time_elapsed = 0;
            current_state = states::no_sound;
        }
    }
}