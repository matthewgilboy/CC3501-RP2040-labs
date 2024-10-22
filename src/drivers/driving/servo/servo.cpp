#include "drivers/driving/servo/servo.h"  // Include the header file for servo control
#include "pico/stdlib.h"                   // Include standard library for Raspberry Pi Pico
#include "GPIO_PINS.h"

#define SRVSIG 13                          // Define the GPIO pin number for the servo signal
static uint slice_num;                     // Variable to store the PWM slice number
static uint32_t pwm_wrap;                  // Variable to store the PWM wrap value

void servoMoveLeft() {
    // Set SRVSIG to high (3.3V)
    gpio_put(SRVSIG, 1);
    sleep_ms(LEFT_HIGH_TIME_MS); // Keep it high for 2ms
    // Set SRVSIG to low (0V)
    gpio_put(SRVSIG, 0);
    sleep_ms(PERIOD_MS - LEFT_HIGH_TIME_MS); // Keep it low for the remainder (18ms)
}

void servoMoveRight() {
    // Set SRVSIG to high (3.3V)
    gpio_put(SRVSIG, 1);
    sleep_ms(RIGHT_HIGH_TIME_MS); // Keep it high for 1ms
    // Set SRVSIG to low (0V)
    gpio_put(SRVSIG, 0);
    sleep_ms(PERIOD_MS - RIGHT_HIGH_TIME_MS); // Keep it low for the remainder (19ms)
}