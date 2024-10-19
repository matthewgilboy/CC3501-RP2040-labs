#ifndef STEPPER_H
#define STEPPER_H

void InitialisePins();

void setMicroStep(int steps);

int StepMotor(bool dir, int numberofsteps);

void RunMotor(bool step);

#endif