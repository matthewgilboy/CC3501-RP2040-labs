#include "ultrasonic.h"            // Include the ultrasonic sensor header
#include "pico/stdlib.h"           // Include the standard library for Raspberry Pi Pico
#include <stdio.h>                 // Include standard I/O library for potential output

#define TRIG 8                    // Define the pin for the ultrasonic sensor's trigger
#define ECHO 9                    // Define the pin for the ultrasonic sensor's echo
#define DISTANCE_THRESHOLD 8       // Define a distance threshold in centimeters

// Global variables for tracking pulse timings
//volatile uint32_t rise_time = 0;   // Variable to store the time of the rising edge
//volatile uint32_t fall_time = 0;   // Variable to store the time of the falling edge
//volatile bool trig_pulse_complete = false;  // Flag to check if the ultrasonic pulse is complete

// Function to initialize the GPIO pins
void init_ultrasonic_sensor() {
    gpio_init(TRIG);                  // Initialize the trigger pin
    gpio_set_dir(TRIG, GPIO_OUT);     // Set the trigger pin as output

    gpio_init(ECHO);                  // Initialize the echo pin
    gpio_set_dir(ECHO, GPIO_IN);      // Set the echo pin as input
    // Enable interrupts for both rising and falling edges on the echo pin
    gpio_set_irq_enabled_with_callback(ECHO, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
}

// GPIO interrupt handler function for echo pin
// void gpio_callback(uint gpio, uint32_t events) {
//     if (gpio == ECHO) {  // Check if the interrupt is on the echo pin
//         if (events & GPIO_IRQ_EDGE_RISE) {
//             rise_time = time_us_32();  // Capture the time when the pulse rises
//         }
//         if (events & GPIO_IRQ_EDGE_FALL) {
//             fall_time = time_us_32();  // Capture the time when the pulse falls
//             trig_pulse_complete = true;  // Set the flag indicating the pulse is complete
//         }
//     }
// }

// Function to trigger the ultrasonic sensor and measure the pulse duration
// void measure_trig_pulse_duration() {
//     gpio_put(TRIG, 0);                // Ensure the trigger pin is low initially
//     sleep_ms(2);                       // Wait to ensure stable state

//     gpio_put(TRIG, 1);                // Send a 10-microsecond pulse to trigger the ultrasonic sensor
//     sleep_us(10);                      // Wait for 10 microseconds
//     gpio_put(TRIG, 0);                // Set the trigger pin back to low

//     // Wait for the pulse to complete
//     while (!trig_pulse_complete) {
//         tight_loop_contents();         // Continue waiting for the pulse to complete
//     }

//     trig_pulse_complete = false;       // Reset the flag for the next measurement
// }

// Function to get the distance in centimeters
// float get_distance_cm() {
//     measure_trig_pulse_duration();  // Trigger the ultrasonic sensor and measure the pulse
//     uint32_t time_diff_us = fall_time - rise_time;  // Time difference between rising and falling edge
//     float distance_cm = (time_diff_us / 2.0) * 0.0343;  // Convert time to distance in cm
//     return distance_cm;  // Return the calculated distance
// }
