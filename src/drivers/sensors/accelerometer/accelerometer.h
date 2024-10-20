#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <string>
#include <stdint.h>  // For uint8_t, int16_t types

// Struct to store accelerometer values
struct Vector3D {
    int16_t x;  // X-axis acceleration value
    int16_t y;  // Y-axis acceleration value
    int16_t z;  // Z-axis acceleration value
};

// Declare global variable `values` as extern to use in other files
extern Vector3D values;  // Declare the accelerometer values struct as extern

// LIS3DH accelerometer functions

// Initializes the LIS3DH accelerometer
void LIS3DH_init();

// Deinitializes the LIS3DH accelerometer
void LIS3DH_deinit();

// Writes a value to a specified register in the accelerometer
int LIS3DH_writeRegister(uint8_t reg, uint8_t value);

// Reads a value from a specified register in the accelerometer
int LIS3DH_readRegister(uint8_t reg, uint8_t *value);

// Reads raw acceleration values from the accelerometer and updates the `values` struct
int LIS3DH_readAccelValues();

// Returns a formatted string of the current accelerometer readings (X, Y, Z)
std::string LIS3DH_getCurrentReadings();

// Updates internal readings from the accelerometer and checks tilt limits
int LIS3DH_updateReadings();

// Calculates the X tilt (roll) in degrees based on accelerometer data
float getXTilt();

// Calculates the Y tilt (pitch) in degrees based on accelerometer data
float getYTilt();

// Returns the conversion ratio for accelerometer readings (used for scaling)
float getAccConversionRatio();

#endif  // ACCELEROMETER_H
