#include <pico/stdlib.h>             // Include standard library for Raspberry Pi Pico
#include <hardware/i2c.h>            // Include hardware I2C library for I2C communication
#include "loadcell.h"                // Include load cell header file for function prototypes
#include "hardware/gpio.h"           // Include hardware GPIO library for GPIO functions
#include <stdio.h>                   // Include standard I/O library for printf

#define I2C_PORT i2c0                // Define I2C port to be used
#define ADC_I2C_ADDRESS 0x68         // MCP3421 default I2C address
#define HX711_SCL 18                 // I2C Clock Pin
#define HX711_SDA 19                  // I2C Data Pin

// Initialize load cell by setting up I2C communication
void loadcell_init() {
    stdio_init_all();                // Initialize serial communication for debugging

    // Initialize I2C with a speed of 100 kHz
    i2c_init(I2C_PORT, 100000); 

    // Set GPIO pins for I2C function
    gpio_set_function(HX711_SDA, GPIO_FUNC_I2C); // Set GPIO for I2C Data
    gpio_set_function(HX711_SCL, GPIO_FUNC_I2C); // Set GPIO for I2C Clock
    gpio_pull_up(HX711_SDA);            // Enable pull-up resistor on SDA pin
    gpio_pull_up(HX711_SCL);            // Enable pull-up resistor on SCL pin

    printf("Load Cell ADC Initialized\n"); // Print confirmation message
}

// Read the load cell value from the ADC
int16_t read_load_cell() {
    uint8_t data[3];                   // Buffer to hold the ADC result (3 bytes)

    // Request data from the MCP3421 ADC
    i2c_write_blocking(I2C_PORT, ADC_I2C_ADDRESS, NULL, 0, true); // Send start condition
    sleep_ms(10);                     // Delay for ADC conversion (adjust delay as necessary)
    
    // Read 3 bytes of data from the ADC
    i2c_read_blocking(I2C_PORT, ADC_I2C_ADDRESS, data, 3, false); 

    // Combine the high and low bytes into a single integer value
    int16_t result = (data[0] << 8) | data[1]; // Combine high byte (data[0]) and low byte (data[1])

    return result;                     // Return the combined result
}
