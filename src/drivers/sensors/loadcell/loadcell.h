#ifndef LOADCELL_H       // Include guard to prevent multiple inclusions of this header file
#define LOADCELL_H

#include <stdint.h>      // Include standard integer types for fixed-width integers

// Function to initialize the load cell setup
void loadcell_init();

// Function to read the load cell value
int16_t read_load_cell();

#endif // LOADCELL_H
