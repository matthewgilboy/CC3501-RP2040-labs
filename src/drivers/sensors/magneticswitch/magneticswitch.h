#ifndef MAGNETICSWITCH_H            // Include guard to prevent multiple inclusions of this header file
#define MAGNETICSWITCH_H

#include <hardware/gpio.h>           // Include GPIO hardware library for Raspberry Pi Pico
#include <pico/stdlib.h>             // Include standard library for Raspberry Pi Pico
#include <cstdint>                   // Include library for fixed-width integer types

// Function prototypes for magnetic switch operations
void init_Mag_Switch(uint32_t Mag_GPIO_Pin);         // Initializes the magnetic switch on the specified GPIO pin
bool Mag_Switch_Active_True(uint32_t Mag_GPIO_Pin);   // Checks if the magnetic switch is active (true)

#endif // MAGNETICSWITCH_H
