#include "drivers/driving/stepper/stepper.h"  // Include the stepper driver header
#include "pico/time.h"                        // Include the time library for sleep functions
#include "pico/stdlib.h"                      // Include standard library for Raspberry Pi Pico

// Define GPIO pin numbers for various control signals
#define HOME_1 3                              // GPIO pin for the home position switch
#define nENBL 4                               // GPIO pin for enabling the driver (active low)
#define nMODE0 5                              // GPIO pin for mode selection (mode 0)

#define nMODE2 10                             // GPIO pin for mode selection (mode 2)
#define nMODE1 11                             // GPIO pin for mode selection (mode 1)

#define LED_DIN 14                            // GPIO pin for LED data input
#define STEP_1 15                             // GPIO pin for step signal

#define DIR_1 20                              // GPIO pin for direction control
#define DECAY 21                              // GPIO pin for decay mode selection
#define FAULT_1 22                            // GPIO pin for fault detection (not used in this code)
#define SLEEP_1 23                            // GPIO pin for sleep control (active high)

// Function to initialize the stepper motor driver
void init_stepper(){
    gpio_init(STEP_1);                       // Initialize the STEP pin
    gpio_set_dir(STEP_1, GPIO_OUT);         // Set the STEP pin as an output

    gpio_init(DIR_1);                        // Initialize the DIR pin
    gpio_set_dir(DIR_1, GPIO_OUT);          // Set the DIR pin as an output

    gpio_init(SLEEP_1);                      // Initialize the SLEEP pin
    gpio_set_dir(SLEEP_1, GPIO_OUT);        // Set the SLEEP pin as an output

    gpio_put(SLEEP_1, 1);                    // Wake up the stepper driver by setting SLEEP high
}

// Function to move the stepper motor a specified number of steps in a given direction
void move_stepper(int steps, int direction){
    gpio_put(DIR_1, direction);              // Set the direction of the stepper motor

    // Loop to create the specified number of steps
    for (int i = 0; i < steps; i++){
        gpio_put(STEP_1, 1);                 // Send a step pulse (high)
        sleep_us(1000);                       // Wait for 1 ms (adjustable for speed)
        gpio_put(STEP_1, 0);                 // Send a step pulse (low)
        sleep_us(1000);                       // Wait for 1 ms (adjustable for speed)
    }
}