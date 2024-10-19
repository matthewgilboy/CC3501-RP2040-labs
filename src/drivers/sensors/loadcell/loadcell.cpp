// LOAD.cpp

#include "GPIO_PINS.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#define LOAD_SCK HX711_SCL
#define LOAD_DOUT HX711_SDA

// Global variables for scale and offset
float SCALE = 1.0;  // Default scale value
long OFFSET = 0;    // Default offset value

// Initialize the Load Cell by configuring GPIO pins
void load_cell_init() {
    gpio_init(LOAD_SCK);  // Initialize SCK pin
    gpio_set_dir(LOAD_SCK, GPIO_OUT);
    gpio_init(LOAD_DOUT);  // Initialize DOUT pin
    gpio_set_dir(LOAD_DOUT, GPIO_IN);
}

// Wait until the HX711 is ready for reading
void wait_ready() {
    while (gpio_get(LOAD_DOUT) == 1) {
        // DOUT is high; HX711 is not ready
        sleep_us(10);
    }
}

// Read a 24-bit value from the HX711
long read_load_cell() {
    wait_ready();  // Ensure the HX711 is ready for reading

    long count = 0;

    // Read 24 bits from DOUT, MSB first
    for (int i = 0; i < 24; i++) {
        gpio_put(LOAD_SCK, 1);  // Set SCK high
        count = count << 1;      // Shift bits to the left to make space for the new bit
        gpio_put(LOAD_SCK, 0);  // Set SCK low

        if (gpio_get(LOAD_DOUT)) {  // If DOUT is high, add 1 to count
            count++;
        }
    }

    // Pulse clock pin once more to set gain for next read
    gpio_put(LOAD_SCK, 1);
    count = count ^ 0x800000;  // Convert 24-bit two's complement to signed
    gpio_put(LOAD_SCK, 0);

    return count;
}

// Set the offset value
void set_offset(long offset) {
    OFFSET = offset;
}

// Set the current weight as tare (zero)
void tare_load_cell() {
    long tareValue = read_load_cell();
    set_offset(tareValue);
}

// Set the scale factor
void set_scale(float scale) {
    SCALE = scale;
}

// Get the scale factor
float get_scale() {
    return SCALE;
}

// Get the offset value
long get_offset() {
    return OFFSET;
}

// Get the weight in units based on scale and offset
float get_units() {
    return (read_load_cell() - OFFSET) / SCALE;
}
