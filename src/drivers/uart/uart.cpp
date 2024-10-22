#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"
#include <iostream>
#include <string>
#include <cstring>

#define UART_ID uart1               // Use UART1
#define BAUD_RATE 115200            // Ensure this matches the Raspberry Pi
#define UART_TX_PIN 24              // TX pin (GPIO 24)
#define UART_RX_PIN 25              // RX pin (GPIO 25)

void uart_init() {
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);  // Configure RX pin for UART
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);  // Configure TX pin for UART

    // Enable UART FIFO and other settings
    // uart_set_hw_flow(UART_ID, false, false);
    // uart_set_format(UART_ID, 8, 1, UART_PARITY_NONE); // 8 data bits, 1 stop bit, no parity
    uart_set_fifo_enabled(UART_ID, true);  // Enable FIFO
}

char uart_receive() {
    if (uart_is_readable(UART_ID)) {
        return uart_getc(UART_ID);  // Read the incoming character
    }
    return '\0';  // Return null character if no data is available
}

void uart_send(char *message) {
    while (*message) {
        uart_putc(UART_ID, *message++);  // Send a character
    }
}