#include "drivers/driving/stepper/stepper.h"  // Include the stepper driver header
#include "pico/time.h"                        // Include the time library for sleep functions
#include "pico/stdlib.h"                      // Include standard library for Raspberry Pi Pico
#include "hardware/pwm.h"   // Include PWM library
#include <math.h>

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

#define nRESET 28

int waittime = 1000;        // Wait time between steps (in microseconds)
int xPos = 0;               // Position of the motor

// Function to initialize the stepper motor driver
void init_stepper(){
    gpio_init(STEP_1);                       // Initialize the STEP pin
    gpio_set_dir(STEP_1, GPIO_OUT);         // Set the STEP pin as an output

    gpio_init(DIR_1);                        // Initialize the DIR pin
    gpio_set_dir(DIR_1, GPIO_OUT);          // Set the DIR pin as an output

    gpio_init(FAULT_1);

    gpio_init(DECAY);
    gpio_set_dir(DECAY, GPIO_OUT);

    gpio_init(nRESET);
    gpio_set_dir(nRESET, GPIO_IN);

    gpio_init(nENBL);
    gpio_set_dir(nENBL, GPIO_OUT); 

    gpio_init(HOME_1);

    gpio_init(nMODE0);
    gpio_set_dir(nMODE0,GPIO_OUT);
    gpio_init(nMODE1);
    gpio_set_dir(nMODE1, GPIO_OUT);
    gpio_init(nMODE2);
    gpio_set_dir(nMODE2,GPIO_OUT);

    gpio_init(SLEEP_1);
    gpio_put(SLEEP_1, true);                    // Wake up the stepper driver by setting SLEEP high
    gpio_set_dir(SLEEP_1,GPIO_OUT);

    gpio_pull_down(DECAY);
    gpio_pull_up(nRESET);

    gpio_put(nENBL, false);
    gpio_put(DECAY, false);
    gpio_put(nMODE0, true);
    gpio_put(nMODE1, true);
    gpio_put(nMODE2, true);
    gpio_put(STEP_1, false);

}

// Function to move the stepper motor a specified number of steps in a given direction
void setMicroSteps(int steps){
    bool m0;
    bool m1;
    bool m2;

    if (steps < 7 && steps > 0){
    steps -= 1;

    m0 = fmod(steps/1,2);
    m1 = fmod(steps/2,2);
    m2 = fmod(steps/4,2);
    } else {
        m0,m1,m2 = 0;
    }
    gpio_put(nMODE0,m0);
    gpio_put(nMODE1,m1);
    gpio_put(nMODE2,m2);
}

int moveStepper(bool run, bool dir){
    gpio_put(DIR_1, dir);
if (run == true) {
        if (dir == true){
            gpio_put(STEP_1,true);
        } else {
            gpio_put(STEP_1,false);
        }
    }

}