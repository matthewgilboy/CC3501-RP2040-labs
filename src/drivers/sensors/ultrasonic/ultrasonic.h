#ifndef ULTRASONIC_H                // Include guard to prevent multiple inclusions
#define ULTRASONIC_H

#include "pico/stdlib.h"            // Include the standard library for Raspberry Pi Pico

// GPIO pin definitions for the HC-SR04 ultrasonic sensor
#define TRIG_PIN 8                 // GPIO pin for the trigger signal (TRIG)
#define ECHO_PIN 9                  // GPIO pin for the echo signal (ECHO)

// Function prototypes for ultrasonic sensor operations
void init_ultrasonic_sensor();      // Initializes GPIO pins for the ultrasonic sensor
float get_distance_cm();            // Returns the distance measured by the ultrasonic sensor in centimeters
void gpio_callback(uint gpio, uint32_t events);  // GPIO interrupt callback function
void measure_trig_pulse_duration(); // Trigger the ultrasonic sensor and measure the pulse duration
int time_diff();                    // Measures the time difference for the pulse

#endif // ULTRASONIC_H
