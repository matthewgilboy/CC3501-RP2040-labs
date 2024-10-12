#include "accelerometer.h"
#include <stdio.h>
#include <math.h>  // Include math library for atan2 function
#include "hardware/i2c.h"
#include "pico/stdlib.h"

// I2C configuration and conversion constants
#define I2C_INSTANCE i2c0  // I2C instance to be used
#define LIS3DHTR_SDA 16     // GPIO pin for I2C SDA
#define LIS3DHTR_SCL 17     // GPIO pin for I2C SCL
#define I2C_ADDRESS 0x19    // I2C address of the LIS3DH accelerometer
#define START_ADDR 0x28     // Starting register address for reading accelerometer data
#define ACC_CONVERSION_RATIO (32768 / 2)  // Conversion ratio for ±2g full scale

// Struct to store accelerometer values
Vector3D values;  // Define the global variable declared as extern in the header

// Tilt threshold in degrees
#define TILT_THRESHOLD 15.0  // Maximum allowed tilt angle before triggering an alert

// Flag to indicate if tilt exceeds limit
bool tilt_exceeded = false;

// Function to initialize the LIS3DH accelerometer
void LIS3DH_init() {
    // Initialize I2C communication with a baud rate of 100 kHz
    i2c_init(I2C_INSTANCE, 100 * 1000);
    gpio_set_function(LIS3DHTR_SDA, GPIO_FUNC_I2C);  // Set SDA pin as I2C function
    gpio_set_function(LIS3DHTR_SCL, GPIO_FUNC_I2C);  // Set SCL pin as I2C function
    gpio_pull_up(LIS3DHTR_SDA);  // Enable pull-up resistor on SDA
    gpio_pull_up(LIS3DHTR_SCL);  // Enable pull-up resistor on SCL
    printf("I2C Initialized on SDA: %d, SCL: %d\n", LIS3DHTR_SDA, LIS3DHTR_SCL);

    // Configure the accelerometer
    LIS3DH_writeRegister(0x20, 0x57);  // CTRL_REG1: Set to 100Hz and enable all axes
    LIS3DH_writeRegister(0x23, 0x00);  // CTRL_REG4: Configure to ±2g (00 in FS bits)
}

// Getter function for the accelerometer conversion ratio
float getAccConversionRatio() {
    return ACC_CONVERSION_RATIO;  // Return the conversion ratio for acceleration
}

// Write a value to a specific accelerometer register
int LIS3DH_writeRegister(uint8_t reg, uint8_t value) {
    uint8_t data[2] = {reg, value};  // Prepare data to write
    // Attempt to write data to the specified register
    if (i2c_write_blocking(I2C_INSTANCE, I2C_ADDRESS, data, 2, false) != 2) {
        printf("Failed to write to register 0x%X\n", reg);  // Log error if write fails
        return -1;  // Return error code
    }
    return 0;  // Return success
}

// Read a value from a specific accelerometer register
int LIS3DH_readRegister(uint8_t reg, uint8_t *value) {
    // Attempt to write register address for reading
    if (i2c_write_blocking(I2C_INSTANCE, I2C_ADDRESS, &reg, 1, true) != 1) {
        printf("Failed to write register address for read (0x%X)!\n", reg);  // Log error
        return -1;  // Return error code
    }
    // Attempt to read the value from the specified register
    if (i2c_read_blocking(I2C_INSTANCE, I2C_ADDRESS, value, 1, false) != 1) {
        printf("Failed to read register value from 0x%X!\n", reg);  // Log error
        return -1;  // Return error code
    }
    return 0;  // Return success
}

// Read raw acceleration values from the accelerometer
int LIS3DH_readAccelValues() {
    uint8_t reg = START_ADDR | 0x80;  // Set MSB for multi-byte read
    uint8_t raw_data[6];  // Array to hold raw data from accelerometer

    // Attempt to set register address for acceleration read
    if (i2c_write_blocking(I2C_INSTANCE, I2C_ADDRESS, &reg, 1, true) != 1) {
        printf("Failed to set register address for acceleration read!\n");
        return -1;  // Return error code
    }
    // Attempt to read acceleration data (6 bytes for x, y, z)
    if (i2c_read_blocking(I2C_INSTANCE, I2C_ADDRESS, raw_data, 6, false) != 6) {
        printf("Failed to read acceleration data!\n");
        return -1;  // Return error code
    }

    // Combine the raw data into 16-bit signed values for x, y, z
    values.x = (int16_t)(raw_data[0] | (raw_data[1] << 8));  // Combine x-axis data
    values.y = (int16_t)(raw_data[2] | (raw_data[3] << 8));  // Combine y-axis data
    values.z = (int16_t)(raw_data[4] | (raw_data[5] << 8));  // Combine z-axis data

    return 0;  // Return success
}

// Function to calculate tilt (roll) in degrees
float getXTilt() {
    // Calculate roll angle in degrees using atan2 function
    return atan2((float)values.y, (float)values.z) * (180.0 / M_PI);  // Convert from radians to degrees
}

// Function to calculate pitch (tilt) in degrees
float getYTilt() {
    // Calculate pitch angle in degrees using atan2 function
    return atan2((float)values.x, (float)values.z) * (180.0 / M_PI);  // Convert from radians to degrees
}

// Get formatted string of current accelerometer readings
std::string LIS3DH_getCurrentReadings() {
    // Convert raw acceleration values to gravitational units
    float x_frac = (float)values.x / ACC_CONVERSION_RATIO;
    float y_frac = (float)values.y / ACC_CONVERSION_RATIO;
    float z_frac = (float)values.z / ACC_CONVERSION_RATIO;

    char buf[50];  // Buffer to hold formatted string
    // Format the readings as a string with two decimal places
    snprintf(buf, sizeof(buf), "(%.2fg, %.2fg, %.2fg)", x_frac, y_frac, z_frac);
    return std::string(buf);  // Return formatted string
}

// Update accelerometer readings and check tilt limit
int LIS3DH_updateReadings() {
    // Read acceleration values from the accelerometer
    int result = LIS3DH_readAccelValues();
    if (result != 0) {
        return result;  // Return error if reading fails
    }

    // Check tilt and set flag if exceeded
    float x_tilt = getXTilt();  // Get roll angle
    float y_tilt = getYTilt();  // Get pitch angle
    // Check if either tilt exceeds the defined threshold
    if (fabs(x_tilt) > TILT_THRESHOLD || fabs(y_tilt) > TILT_THRESHOLD) {
        tilt_exceeded = true;  // Set flag if tilt exceeds threshold
        printf("Tilt limit exceeded! X: %.2f°, Y: %.2f°\n", x_tilt, y_tilt);  // Log tilt alert
    } else {
        tilt_exceeded = false;  // Reset the flag if within limits
    }

    return 0;  // Return success
}
