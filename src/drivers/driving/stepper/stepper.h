#ifndef STEPPER_H               // Include guard to prevent multiple inclusions of this header file
#define STEPPER_H

// Function prototypes for stepper motor control
void init_stepper();           // Initializes the stepper motor driver and GPIO pins
int moveStepper(bool run, bool dir);
void setMicroSteps(int steps);

#endif // STEPPER_H
