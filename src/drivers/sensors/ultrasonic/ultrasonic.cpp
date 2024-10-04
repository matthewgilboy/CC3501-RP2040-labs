#include "ultrasonic.h"
#include "pico/stdlib.h"
#include <stdio.h>

// Function to initialize GPIO pins for the ultrasonic sensor
void init_ultrasonic_sensor() {
    gpio_init(TRIG_PIN);
    gpio_set_dir(TRIG_PIN, GPIO_OUT);
    gpio_put(TRIG_PIN, 0);  // Ensure trigger is low initially

    gpio_init(ECHO_PIN);
    gpio_set_dir(ECHO_PIN, GPIO_IN);
}

// Function to get distance from the ultrasonic sensor in cm
float get_distance_cm() {
    // Send a 10us pulse to TRIG_PIN
    gpio_put(TRIG_PIN, 1);
    sleep_us(10);
    gpio_put(TRIG_PIN, 0);

    // Wait for the echo to start
    while (gpio_get(ECHO_PIN) == 0) {
        tight_loop_contents();  // Wait for ECHO to go high
    }

    // Measure the time that ECHO is high
    absolute_time_t start_time = get_absolute_time();
    while (gpio_get(ECHO_PIN) == 1) {
        tight_loop_contents();  // Wait for ECHO to go low
    }
    absolute_time_t end_time = get_absolute_time();

    // Calculate the time difference in microseconds
    int64_t pulse_duration = absolute_time_diff_us(start_time, end_time);

    // Calculate the distance in cm (speed of sound is 343 m/s)
    float distance_cm = pulse_duration / 58.0;

    return distance_cm;
}

// Function to check if an object is within the given distance (e.g., 10 cm)
bool is_object_within_distance(float threshold_cm) {
    float distance = get_distance_cm();
    return (distance > 0 && distance <= threshold_cm);
}
