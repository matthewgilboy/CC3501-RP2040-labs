#ifndef STEPPER_H               // Include guard to prevent multiple inclusions of this header file
#define STEPPER_H

// Function prototypes for stepper motor control
void init_stepper();           // Initializes the stepper motor driver and GPIO pins
void move_stepper(int steps, int direction); // Moves the stepper motor a specified number of steps in a given direction

#endif // STEPPER_H
