// Aufrufen benutzer defenierter Bibliotheke.
#include "Arduino.h"
#include "sensor.hpp"
#include "inputs.hpp"

namespace sensor {
	states current_state = states::no_movement_detected;

	void init() {
		pinMode(pins::SENSOR, INPUT);
	}

	void update_state() {
		if (digitalRead(pins::SENSOR)) {
			current_state = states::movement_detected;
		}
		else {
			current_state = states::no_movement_detected;
		}
	}
}