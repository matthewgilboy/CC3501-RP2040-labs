#ifndef steppermotor
#define steppermotor

// Function to initialize motor pins
void InitialisePins();

// Function to set microstep mode (full-step, half-step, etc.)
void setMicroStep(int steps);

// Function to run the motor continuously
void RunMotor();

// Function to start the motor
void StartMotor();

// Function to stop the motor
void StopMotor();

#endif
