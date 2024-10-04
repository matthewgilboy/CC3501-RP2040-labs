#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "pico/stdlib.h"

// GPIO pin definitions for HC-SR04
#define TRIG_PIN 2
#define ECHO_PIN 3

// Function prototypes
void init_ultrasonic_sensor();
float get_distance_cm();
bool is_object_within_distance(float threshold_cm);

#endif // ULTRASONIC_SENSOR_H
