#ifndef SERVO_H                // Include guard to prevent multiple inclusions of this header file
#define SERVO_H

#include "hardware/pwm.h"      // Include the PWM hardware library for controlling PWM signals

// Define constants for the servo configuration
#define SRVSIG 13              // GPIO pin 13 for the servo signal
#define MIN_PULSE_WIDTH 1000   // Minimum pulse width (1 ms) corresponding to 0° position
#define MAX_PULSE_WIDTH 2000   // Maximum pulse width (2 ms) corresponding to 180° position
#define PWM_FREQUENCY 50       // Standard PWM frequency for servos (50 Hz)

void init_SG90_servo();        // Function prototype to initialize the SG90 servo
void set_SG90_servo_pos(uint16_t pulsewidth); // Function prototype to set the servo position based on pulse width

#endif // SERVO_H