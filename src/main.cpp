#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include <iostream>
#include <math.h>
#include "hardware/irq.h"
#include "WS2812.pio.h"
#include "drivers/logging/logging.h"
#include "drivers/sensors/accelerometer/accelerometer.h"
#include "hardware/i2c.h"
#include "drivers/driving/stepper/stepper.h"

using namespace std;

#define LED_PIN 14
#define TrigPin 8
#define EchoPin 9
#define DISTANCE_THRESHOLD 8 // Distance threshold in centimeters
#define SRVSIG 13
#define LEFT_HIGH_TIME_MS 2
#define RIGHT_HIGH_TIME_MS 1
#define PERIOD_MS 20
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY UART_PARITY_NONE
#define UART_TX_PIN 25 // UART PIN FOR BLUETOOTH
#define UART_RX_PIN 24 // UART PIN FOR BLUETOOTH
#define UART_ID uart1 // UART ID FOR BLUETOOTH
#define TILT_THRESHOLD 15.0

volatile uint32_t rise_time = 0;
volatile uint32_t fall_time = 0;
volatile bool trig_pulse_complete = false;
volatile bool input_ready = false;
char buffer[100] = {};
int ind = 0;
char good_bad;

void stepperMoveLeft() {
    // Set SRVSIG to high (3.3V)
    gpio_put(SRVSIG, 1);
    sleep_ms(LEFT_HIGH_TIME_MS); // Keep it high for 2ms
    // Set SRVSIG to low (0V)
    gpio_put(SRVSIG, 0);
    sleep_ms(PERIOD_MS - LEFT_HIGH_TIME_MS); // Keep it low for the remainder (18ms)
}

void stepperMoveRight() {
    // Set SRVSIG to high (3.3V)
    gpio_put(SRVSIG, 1);
    sleep_ms(RIGHT_HIGH_TIME_MS); // Keep it high for 1ms
    // Set SRVSIG to low (0V)
    gpio_put(SRVSIG, 0);
    sleep_ms(PERIOD_MS - RIGHT_HIGH_TIME_MS); // Keep it low for the remainder (19ms)
}

// Function for RX interrupt handler
void on_uart_rx(void) {
    while (uart_is_readable(UART_ID)) {
        char ch = uart_getc(UART_ID);
        if (ind < sizeof(buffer) - 1) { // Ensure buffer doesn't overflow
            printf("UART RX: %c\n", ch);
            buffer[ind++] = ch; // Store character in buffer
            good_bad = ch;
            input_ready = true;
        }
    }
}

// Function to initialize UART communication
void initialize_uart() {
    // Set up UART with a basic baud rate
    uart_init(UART_ID, BAUD_RATE);

    // Set the TX and RX pins
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // Set UART flow control CTS/RTS off
    uart_set_hw_flow(UART_ID, false, false);

    // Set data format
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);

    // Turn off FIFO
    uart_set_fifo_enabled(UART_ID, false);

    // Set up RX interrupt handler
    int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;
    irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
    irq_set_enabled(UART_IRQ, true);

    // Enable UART RX interrupts
    uart_set_irq_enables(UART_ID, true, false);
}

// Function to reset the buffer
void reset_buffer() {
    for (size_t i = 0; i < 100; i++) {
        buffer[i] = '\000';
    }
    input_ready = false;
    ind = 0;
}

// Function to handle UART movement commands
void goodBadDetection() {
    if (sscanf((char *) buffer, "%c", &good_bad) == 1) {
        switch (good_bad) {
            case '1': // Move left (Bad part)
                for (int i = 0; i < 5; ++i) {
                    stepperMoveLeft();
                }
                sleep_ms(500);
                for (int i = 0; i < 5; ++i) {
                    stepperMoveRight();
                }
                break;
            case '2': // Move right after bad part is removed
                for (int i = 0; i < 5; ++i) {
                    stepperMoveRight();
                }
                break;
            default:
                printf("ERROR: Invalid key.\r\n");
        }
    }
}

// GPIO callback for handling pulse timing
void gpio_callback(uint gpio, uint32_t events) {
    if (gpio == EchoPin) {
        if (events & GPIO_IRQ_EDGE_RISE) {
            rise_time = time_us_32();
        }
        if (events & GPIO_IRQ_EDGE_FALL) {
            fall_time = time_us_32();
            trig_pulse_complete = true;
        }
    }
}

// Trigger pulse for ultrasonic sensor
void measure_trig_pulse_duration() {
    gpio_init(TrigPin);
    gpio_set_dir(TrigPin, GPIO_OUT);
    gpio_put(TrigPin, 0);
    sleep_ms(2);
    gpio_put(TrigPin, 1);
    sleep_us(10);
    gpio_put(TrigPin, 0);

    gpio_init(EchoPin);
    gpio_set_dir(EchoPin, GPIO_IN);
    gpio_set_irq_enabled_with_callback(EchoPin, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    if (!trig_pulse_complete) {
        tight_loop_contents();
    }
}

int main() {
    stdio_init_all();
    gpio_init(SRVSIG);
    gpio_set_dir(SRVSIG, GPIO_OUT);
    initialize_uart();
    LIS3DH_init();

    uint pio_program_offset = pio_add_program(pio0, &ws2812_program);
    ws2812_program_init(pio0, 0, pio_program_offset, LED_PIN, 800000, false);

    uint32_t led_data[1] = {0};
    InitialisePins();
    setMicroStep(5);
    sleep_ms(10);

    bool motorTiltCondition = false;
    bool motorUltraCondition = false;

    for (;;) {
        LIS3DH_updateReadings();
        RunMotor();

        // Calculate distance after pulse
        uint32_t time_diff_us = fall_time - rise_time;
        float distance_cm = (time_diff_us / 2.0) * 0.0343;

        // Get tilt values
        float x_tilt = getXTilt();
        float y_tilt = getYTilt();

        if (fabs(x_tilt) > TILT_THRESHOLD || fabs(y_tilt) > TILT_THRESHOLD) {
            led_data[0] = (0 << 24) | (255 << 16) | (0 << 8); // Green
            motorTiltCondition = true;
        } else {
            led_data[0] = 0;
            measure_trig_pulse_duration();
            motorTiltCondition = false;
        }

        // Control LED based on distance
        if (distance_cm < DISTANCE_THRESHOLD) {
            led_data[0] = (255 << 24) | (0 << 16) | (0 << 8); // Red
            motorUltraCondition = true;
            uart_puts(UART_ID, "1");
        } else {
            motorUltraCondition = false;
            uart_puts(UART_ID, "0");
        }

        if (motorTiltCondition || motorUltraCondition) {
            StopMotor();
        } else {
            StartMotor();
        }

        pio_sm_put_blocking(pio0, 0, led_data[0]);

        if (input_ready) {
            input_ready = false;
            goodBadDetection();
            reset_buffer();
        }
    }
}
