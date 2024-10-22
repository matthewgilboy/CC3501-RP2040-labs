#include "pico/stdlib.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "hardware/pwm.h"
#include "hardware/uart.h"
#include "hardware/irq.h"
#include <math.h>
#include <string.h>
#include <pico/time.h>

// Define pins
#define nENBL 4
#define nRESET 28
#define DECAY 21

#define MODE0 5
#define MODE1 11
#define MODE2 10

#define STEP1 15
#define DIR1 20
#define SLEEP1 23
#define FAULT1 22
#define HOME1 3

bool run = 1;
int freq = 495;  // Frequency for steps

// Function to initialize pins
void InitialisePins() {
    stdio_init_all();
    
    gpio_init(nENBL);
    gpio_init(nRESET);
    gpio_init(DECAY);
    gpio_init(MODE0);
    gpio_init(MODE1);
    gpio_init(MODE2);
    gpio_init(DIR1);
    gpio_init(STEP1);
    gpio_init(SLEEP1);
    gpio_init(FAULT1);
    gpio_init(HOME1);

    // Set directions
    gpio_set_dir(nENBL, GPIO_OUT);
    gpio_set_dir(nRESET, GPIO_IN);
    gpio_set_dir(DECAY, GPIO_OUT);
    gpio_set_dir(MODE0, GPIO_OUT);
    gpio_set_dir(MODE1, GPIO_OUT);
    gpio_set_dir(MODE2, GPIO_OUT);
    gpio_set_dir(DIR1, GPIO_OUT);
    gpio_set_dir(STEP1, GPIO_OUT);
    gpio_set_dir(SLEEP1, GPIO_OUT);

    // Set pin states
    gpio_put(nENBL, false);
    gpio_pull_up(nRESET);
    gpio_pull_down(DECAY);

    gpio_put(MODE0, true);
    gpio_put(MODE1, true);
    gpio_put(MODE2, true);
    gpio_put(SLEEP1, true);
    gpio_put(DIR1, true);
    gpio_put(STEP1, false);
}

// Function to set microstep value
void setMicroStep(int steps) {
    // Input value from 1 - 6
    bool m0, m1, m2;

    // Set each mode pin based on the input according to documentation
    if (steps >= 1 && steps <= 6) {
        steps -= 1;  // Adjust input to 0 - 5
        // Convert input to binary and split the digits
        m0 = fmod(steps / 1, 2);
        m1 = fmod(steps / 2, 2);
        m2 = fmod(steps / 4, 2);
    } else {
        m0 = m1 = m2 = 0;
    }

    // Drive mode pins
    gpio_put(MODE0, m0);
    gpio_put(MODE1, m1);
    gpio_put(MODE2, m2);
}

// Function to run the motor
void RunMotor() {
    if (run == 1) {
        gpio_put(STEP1, 1);
        sleep_ms(500 / freq);
        gpio_put(STEP1, 0);
        sleep_ms(500 / freq);
    }
}

// Function to start the motor
void StartMotor() {
    run = 1;
}

// Function to stop the motor
void StopMotor() {
    run = 0;
}
