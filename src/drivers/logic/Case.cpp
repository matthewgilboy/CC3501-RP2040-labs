#include <stdio.h>
#include "drivers/logic/Case.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"

// Static variable to store the position of the switch
static volatile int switchpos = 0;

// Interrupt Service Routine (ISR) for switch events
void switch_isr(uint gpio, uint32_t events) {
    // Update switch position, cycling through values 0, 1, 2
    switchpos = (switchpos + 1) % 3;
}

// Initializes the emergency switch GPIO pin
void switch_init() {
    gpio_init(EMGNCY_SW);  // Initialize the GPIO pin for the switch
    gpio_set_dir(EMGNCY_SW, GPIO_IN);  // Set the pin direction to input
    // Enable interrupt on falling edge with callback to switch_isr
    gpio_set_irq_enabled_with_callback(EMGNCY_SW, GPIO_IRQ_EDGE_FALL, true, &switch_isr);
}

// Returns the current state of the switch
int get_switch_state(void) {
    return switchpos;  // Return the switch position (0, 1, or 2)
}