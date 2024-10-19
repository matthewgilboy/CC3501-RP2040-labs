// loadcell.h

#ifndef LOADCELL_H
#define LOADCELL_H

// Include necessary libraries
#include "GPIO_PINS.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"

// Function prototypes
void load_cell_init();
void wait_ready();
long read_load_cell();
void tare_load_cell();
void set_scale(float scale);
float get_scale();
void set_offset(long offset);
long get_offset();
float get_units();

// Global variables for scale and offset
extern float SCALE;  // Default scale value
extern long OFFSET;  // Default offset value

#endif // LOADCELL_H
