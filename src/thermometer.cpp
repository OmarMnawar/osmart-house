#include "thermometer.hpp"
#include "inputs.hpp"

namespace thermometer {

    int16_t temperature_celsius = 0;
    int16_t last_measurement = 0;
    const float BETA = 3950;
    bool temperature_updated = false;

    void update_thermometer() {
        int16_t value_reading = analogRead(pins::THERMOMETER);

        // temperature_celsius = map(value_reading, 0, 1024, -50, 150);

        // OM: This is the standard math for a NTC Temperature readings in celsius.
        temperature_celsius = 1 / (log(1 / (1023. / value_reading - 1)) / BETA + 1.0 / 298.15) - 273.15;

        if (temperature_celsius == last_measurement) {
            temperature_updated = false;
            return;
        }

        last_measurement = temperature_celsius;
        temperature_updated  = true;
    }
}