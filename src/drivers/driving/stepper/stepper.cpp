#include "GPIO_PINS.h"    // Include the GPIO pin definitions
#include "Includes.h"     // Include any additional necessary libraries or definitions

int freq = 1000;   // PWM frequency for motor steps (currently not used, but can be adapted for PWM control)
bool MotorState;   // Tracks whether the stepper motor is in a high or low state
absolute_time_t MotorTimer;   // Tracks the time when the motor last stepped

int WaitTime = 2900;   // Minimum pulse width in microseconds for the step input (sets the timing for motor stepping)

// Function to initialize GPIO pins used to control the stepper motor
void InitialisePins(){
    stdio_init_all();   // Initialize standard I/O for debugging or communication
    gpio_init(nENBL);   // Initialize enable pin
    gpio_init(nRESET);  // Initialize reset pin
    gpio_init(DECAY);   // Initialize decay mode pin

    gpio_init(nMODE0);  // Initialize microstep mode pin 0
    gpio_init(nMODE1);  // Initialize microstep mode pin 1
    gpio_init(nMODE2);  // Initialize microstep mode pin 2

    gpio_init(DIR_1);   // Initialize motor direction pin
    gpio_init(STEP_1);  // Initialize step signal pin
    gpio_init(SLEEP_1); // Initialize sleep pin
    gpio_init(FAULT_1); // Initialize fault pin (to detect issues with the motor driver)
    gpio_init(HOME_1);  // Initialize home position pin

    // Set the direction for each pin (output/input)
    gpio_set_dir(nENBL, GPIO_OUT);  // Set enable pin as output
    gpio_set_dir(nRESET, GPIO_IN);  // Set reset pin as input
    gpio_set_dir(DECAY, GPIO_OUT);  // Set decay pin as output

    gpio_set_dir(nMODE0, GPIO_OUT); // Set mode 0 pin as output
    gpio_set_dir(nMODE1, GPIO_OUT); // Set mode 1 pin as output
    gpio_set_dir(nMODE2, GPIO_OUT); // Set mode 2 pin as output

    gpio_set_dir(DIR_1, GPIO_OUT);  // Set motor direction pin as output
    gpio_set_dir(STEP_1, GPIO_OUT); // Set step signal pin as output
    gpio_set_dir(SLEEP_1, GPIO_OUT); // Set sleep pin as output

    // Initialize the initial states for each pin
    gpio_put(nENBL, false);    // Disable the motor driver by default
    gpio_pull_up(nRESET);      // Enable pull-up resistor on reset pin (active low reset)
    gpio_put(DECAY, false);    // Set decay mode to off
    gpio_pull_down(DECAY);     // Enable pull-down resistor on decay pin

    gpio_put(nMODE0, true);    // Set microstepping mode (default full step)
    gpio_put(nMODE1, true);    
    gpio_put(nMODE2, true);    

    gpio_put(SLEEP_1, true);   // Wake the motor driver by default
    gpio_put(STEP_1, false);   // Initial state of the step signal
    gpio_put(DIR_1, true);     // Default direction of the motor

    // The following commented section relates to PWM configuration for the step signal.
    // Uncomment and configure if using PWM for step control:
    // gpio_set_function(STEP_1, GPIO_FUNC_PWM);
    // pwm_set_wrap(pwm_gpio_to_slice_num(STEP_1), 255);
    // pwm_set_gpio_level(STEP_1, 0);
    // pwm_set_clkdiv(pwm_gpio_to_slice_num(STEP_1), 1.0f);
    // pwm_set_enabled(pwm_gpio_to_slice_num(STEP_1), true);
}

// Function to set the microstepping mode
// Accepts values from 1 to 6 corresponding to the microstep resolution (e.g., full step, half step, etc.)
void setMicroStep(int steps){
    // Convert the input step value (1-6) to corresponding binary values for m0, m1, m2 pins
    bool m0, m1, m2;
    
    if(steps >= 1 && steps <= 6){
        steps -= 1;   // Change input to 0-5 for binary calculation
        // Convert steps to binary representation (used for mode pins)
        m0 = fmod(steps / 1, 2);
        m1 = fmod(steps / 2, 2);
        m2 = fmod(steps / 4, 2);
    } else {
        // If invalid input, default to lowest resolution (full step)
        m0 = m1 = m2 = 0;
    }

    // Set the mode pins according to calculated binary values
    gpio_put(nMODE0, m0);
    gpio_put(nMODE1, m1);
    gpio_put(nMODE2, m2);
}

// Function to step the motor a specified number of steps in a given direction
// Returns the remaining number of steps after each pulse
int StepMotor(bool dir, int numberofsteps){
    // Get the current time
    absolute_time_t t = get_absolute_time();
    // Calculate the time difference since the last step
    int diff = absolute_time_diff_us(MotorTimer, t);

    // Check if the time difference is greater than the required wait time between steps
    if(diff > WaitTime){
        // If the step pin is currently high (MotorState is true), drive it low
        if(MotorState){
            MotorState = 0;    // Set state to low
            gpio_put(STEP_1, false);   // Drive the step pin low
            MotorTimer = get_absolute_time();   // Reset the timer
        } else {
            // If the step pin is low, set direction and drive the step pin high
            MotorState = 1;   // Set state to high
            gpio_put(DIR_1, dir);   // Set direction
            gpio_put(STEP_1, true); // Drive the step pin high
            MotorTimer = get_absolute_time();   // Reset the timer
            // Decrease the number of remaining steps
            if (numberofsteps > 0){
                return numberofsteps - 1;
            }
        }
    }
    // If time between pulses is too short, return the same number of steps
    return numberofsteps;
}

// Function to control the motor step pin (directly drive the step signal)
// If using PWM, you would control the duty cycle here instead
void RunMotor(bool step){
    // pwm_set_gpio_level(STEP1, freq);   // Uncomment for PWM control
    gpio_put(STEP_1, step);   // Directly drive the step pin high or low
}
