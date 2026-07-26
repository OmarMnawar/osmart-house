#ifndef Sensor_H
#define Sensor_H

namespace sensor {

    enum class states {
        movement_detected,
        no_movement_detected
    };


    void init();
    void update_state();


    extern states current_state;
}

#endif