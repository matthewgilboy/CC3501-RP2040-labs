#include "magneticswitch.h"          // Include the magnetic switch header file

#define MAGSIG 12                    // Define the GPIO pin for the magnetic switch (sensor)

// Initialize the magnetic switch (sensor) on the specified GPIO pin
void init_Mag_Switch(uint32_t Mag_GPIO_Pin) {
    gpio_init(Mag_GPIO_Pin);         // Initialize the GPIO pin
    gpio_set_dir(Mag_GPIO_Pin, GPIO_IN); // Set the pin direction as input
    gpio_pull_up(Mag_GPIO_Pin);      // Enable the internal pull-up resistor
}

// Check if the magnetic switch is active (returns true if active)
bool Mag_Switch_Active_True(uint32_t Mag_GPIO_Pin) {
    return !gpio_get(Mag_GPIO_Pin);  // Return true if the switch is active (when the pin reads low)
}
