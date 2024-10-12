#ifndef SERVO_H
#define SERVO_H

#include "hardware/pwm.h"

#define SRVSIG 13          // GPIO13 for the servo signal
#define MIN_PULSE_WIDTH 1000  // Minimum pulse width (1 ms, 0° position)
#define MAX_PULSE_WIDTH 2000  // Maximum pulse width (2 ms, 180° position)
#define PWM_FREQUENCY 50      // Standard servo frequency (50Hz)

void init_SG90_servo();
void set_SG90_servo_pos(uint16_t pulsewidth);

#endif