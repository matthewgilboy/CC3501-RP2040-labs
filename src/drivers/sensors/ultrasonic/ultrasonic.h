#ifndef ULTRASONIC_H                // Include guard to prevent multiple inclusions of this header file
#define ULTRASONIC_H

#include "pico/stdlib.h"            // Include the standard library for Raspberry Pi Pico

// GPIO pin definitions for HC-SR04 ultrasonic sensor
#define TRIG_PIN 2                  // GPIO pin for the trigger signal (TRIG)
#define ECHO_PIN 3                  // GPIO pin for the echo signal (ECHO)

// Function prototypes for ultrasonic sensor operations
void init_ultrasonic_sensor();      // Initializes GPIO pins for the ultrasonic sensor
float get_distance_cm();             // Returns the distance measured by the ultrasonic sensor in centimeters
bool is_object_within_distance(float threshold_cm); // Checks if an object is within the specified distance threshold

#endif // ULTRASONIC_H
