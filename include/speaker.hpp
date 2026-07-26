#ifndef SPEAKER_H
#define SPEAKER_H

#include <Arduino.h>

namespace speaker {

    enum class states : uint8_t {
        no_sound,
        playing_lcd_on_sound,
        playing_lcd_off_sound,
        playing_error_sound,
        playing_success_sound
    };

    void handle_sounds_playing();

    extern states current_state;
}

#endif