// Aufrufen benutzer defenierter Bibliotheke.
#include "Arduino.h"
#include "Sensor.hpp"
#include "Inputs.hpp"
#include "States.hpp"

namespace Sensor {
	States current_state = States::NoMovementDetected;

	void init() {
		pinMode(Pins::SENSOR_PIN, INPUT);
	}

	void update_state() {
		if (digitalRead(Pins::SENSOR_PIN)) {
			current_state = States::MovementDetected;
		}
		else {
			current_state = States::NoMovementDetected;
		}
	}
}