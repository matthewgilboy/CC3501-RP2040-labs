#ifndef CASE_H
#define CASE_H

#include <stdio.h>

// Define GPIO pin number for the emergency switch
#define EMGNCY_SW 2

// Initializes the emergency switch GPIO pin
void switch_init();

// Returns the current state of the switch (0, 1, or 2)
int get_switch_state();

#endif  // CASE_H
