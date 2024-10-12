#include "ultrasonic.h"            // Include the ultrasonic sensor header
#include "pico/stdlib.h"          // Include the standard library for Raspberry Pi Pico
#include <stdio.h>                // Include standard I/O library for potential output

// Define GPIO pin numbers for the ultrasonic sensor
#define TRIG 8                    // GPIO pin for triggering the ultrasonic sensor
#define ECHO 9                    // GPIO pin for receiving the echo signal

// Function to initialize GPIO pins for the ultrasonic sensor
void init_ultrasonic_sensor() {
    // Initialize the TRIG pin
    gpio_init(TRIG);              
    gpio_set_dir(TRIG, GPIO_OUT); // Set TRIG pin as output
    gpio_put(TRIG, 0);             // Ensure TRIG is low initially

    // Initialize the ECHO pin
    gpio_init(ECHO);
    gpio_set_dir(ECHO, GPIO_IN);  // Set ECHO pin as input
}

// Function to get distance from the ultrasonic sensor in cm
float get_distance_cm() {
    // Send a 10 microsecond pulse to the TRIG pin to initiate the measurement
    gpio_put(TRIG, 1);             // Set TRIG high
    sleep_us(10);                  // Wait for 10 microseconds
    gpio_put(TRIG, 0);             // Set TRIG low

    // Wait for the ECHO pin to go high, indicating the start of the echo
    while (gpio_get(ECHO) == 0) {
        tight_loop_contents();      // Busy-wait until ECHO goes high
    }

    // Record the time when the echo is received
    absolute_time_t start_time = get_absolute_time(); // Get the current time
    while (gpio_get(ECHO) == 1) {
        tight_loop_contents();      // Busy-wait until ECHO goes low
    }
    absolute_time_t end_time = get_absolute_time();   // Get the current time when ECHO goes low

    // Calculate the time duration of the echo pulse in microseconds
    int64_t pulse_duration = absolute_time_diff_us(start_time, end_time);

    // Calculate the distance in centimeters (speed of sound is approximately 343 m/s)
    float distance_cm = pulse_duration / 58.0;       // Convert time to distance

    return distance_cm;                               // Return the calculated distance
}

// Function to check if an object is within the given distance (e.g., 10 cm)
bool is_object_within_distance(float threshold_cm) {
    float distance = get_distance_cm();                // Get the current distance from the sensor
    return (distance > 0 && distance <= threshold_cm); // Return true if distance is within threshold
}
