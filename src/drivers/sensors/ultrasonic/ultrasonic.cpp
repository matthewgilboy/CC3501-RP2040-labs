#include "ultrasonic.h"            // Include the ultrasonic sensor header
#include "pico/stdlib.h"           // Include the standard library for Raspberry Pi Pico
#include <stdio.h>                 // Include standard I/O library for potential output
using namespace std;

#define LED_PIN 14                 // Define the pin for controlling the WS2812 LED
#define TrigPin 8                  // Define the pin for the ultrasonic sensor's trigger
#define EchoPin 9                  // Define the pin for the ultrasonic sensor's echo
#define DISTANCE_THRESHOLD 8       // Define a distance threshold in centimeters

// Global variables for tracking pulse timings
volatile uint32_t rise_time = 0;   // Variable to store the time of the rising edge
volatile uint32_t fall_time = 0;   // Variable to store the time of the falling edge
volatile bool trig_pulse_complete = false;  // Flag to check if the ultrasonic pulse is complete

// GPIO interrupt handler function for echo pin
void gpio_callback(uint gpio, uint32_t events)
{
    if (gpio == EchoPin) {  // Check if the interrupt is on the echo pin
        if (events & GPIO_IRQ_EDGE_RISE) {
            rise_time = time_us_32();  // Capture the time when the pulse rises
        }
        if (events & GPIO_IRQ_EDGE_FALL) {
            fall_time = time_us_32();  // Capture the time when the pulse falls
            trig_pulse_complete = true;  // Set the flag indicating the pulse is complete
        }
    }
}

// Function to trigger the ultrasonic sensor and measure the pulse duration
void measure_trig_pulse_duration()
{
    gpio_init(TrigPin);                  // Initialize the trigger pin
    gpio_set_dir(TrigPin, GPIO_OUT);     // Set the trigger pin as output
    gpio_put(TrigPin, 0);                // Ensure the trigger pin is low initially
    sleep_ms(2);                         // Wait to ensure stable state

    gpio_put(TrigPin, 1);                // Send a 10-microsecond pulse to trigger the ultrasonic sensor
    sleep_us(10);                        // Wait for 10 microseconds
    gpio_put(TrigPin, 0);                // Set the trigger pin back to low

    gpio_init(EchoPin);                  // Initialize the echo pin
    gpio_set_dir(EchoPin, GPIO_IN);      // Set the echo pin as input
    // Enable interrupts for both rising and falling edges on the echo pin
    gpio_set_irq_enabled_with_callback(EchoPin, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    // Wait for the pulse to complete
    while (!trig_pulse_complete) {
        tight_loop_contents();           // Continue waiting for the pulse to complete
    }

    trig_pulse_complete = false;         // Reset the flag for the next measurement
}

int main()
{
    stdio_init_all();  // Initialize all the standard I/O for debugging

    // Initialize the WS2812 LED program
    // uint pio_program_offset = pio_add_program(pio0, &ws2812_program);
    // ws2812_program_init(pio0, 0, pio_program_offset, LED_PIN, 800000, false); // Configure WS2812 LED settings
    // uint32_t led_data[1];  // Create an array to hold LED color data

    for (;;) {
        measure_trig_pulse_duration();  // Trigger the ultrasonic sensor and measure the pulse
        sleep_ms(100);  // Delay before the next measurement

        // Calculate distance based on pulse duration
        uint32_t time_diff_us = fall_time - rise_time;  // Time difference between rising and falling edge
        float distance_cm = (time_diff_us / 2.0) * 0.0343;  // Convert time to distance in cm

        // Control the LED based on the distance measured
        if (distance_cm < DISTANCE_THRESHOLD) {
            // Turn on the LED with green color if the distance is below the threshold
            //led_data[0] = (0 << 24) | (255 << 16) | (0 << 8);  // Green color
        } else {
            // Turn off the LED if the distance is above the threshold
            //led_data[0] = 0;  // No color (LED off)
        }

        // Send the LED data to the WS2812 LED
        //pio_sm_put_blocking(pio0, 0, led_data[0]);

        sleep_ms(500);  // Wait before checking the distance again
    }

    return 0;
}
