#include "drivers/driving/servo/servo.h"  // Include the header file for servo control
#include "pico/stdlib.h"                   // Include standard library for Raspberry Pi Pico
#include "GPIO_PINS.h"

#define SRVSIG 13                          // Define the GPIO pin number for the servo signal
static uint slice_num;                     // Variable to store the PWM slice number
static uint32_t pwm_wrap;                  // Variable to store the PWM wrap value

// Function to initialize the SG90 servo
void init_SG90_servo(){
    // Set the GPIO pin function to PWM
    gpio_set_function(SRVSIG, GPIO_FUNC_PWM);

    // Get the PWM slice number corresponding to the SRVSIG pin
    //slice_num = pwm_gpio_to_slice_num(SRVSIG);

    // Calculate the wrap value for the PWM based on the desired frequency
    pwm_wrap = (125000000 / PWM_FREQUENCY) - 1; // 125000000 is the clock frequency; adjust for PWM_FREQUENCY
    // Set the PWM wrap value for the specified slice
    //pwm_set_wrap(slice_num, pwm_wrap);
}

// Function to set the position of the SG90 servo based on pulse width
void set_SG90_servo_pos(uint16_t pulse_width){
    // Calculate the compare value for the PWM channel based on the desired pulse width
    uint32_t pwm_compare = (pulse_width * pwm_wrap) / (1000000 / PWM_FREQUENCY);

    // Set the PWM channel level for the servo to the calculated compare value
    //pwm_set_chan_level(slice_num, PWM_CHAN_A, pwm_compare);
}