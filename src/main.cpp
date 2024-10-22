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

#define PARITY    UART_PARITY_NONE

#define UART_TX_PIN 25 // UART PIN FOR BLUETOOTH

#define UART_RX_PIN 24 // UART PIN FOR BLUETOOTH

#define UART_ID uart1 // UART ID FOR BLUETOOTH

#define TILT_THRESHOLD 15.0

 

volatile uint32_t rise_time = 0;

volatile uint32_t fall_time = 0;

volatile bool trig_pulse_complete = false;

 

volatile bool input_ready = false;

char buffer [100] = {};

int ind = 0;

char good_bad;

int main()

{

    stdio_init_all();

    LIS3DH_init();

    uint pio_program_offset = pio_add_program(pio0, &ws2812_program);

    ws2812_program_init(pio0, 0, pio_program_offset, LED_PIN, 800000, false);

    uint32_t led_data[1];

    stdio_init_all();

    InitialisePins();

    setMicroStep(5);

    sleep_ms(10);

    bool motorTiltCondition = 0;

    bool motorUltraCondition = 0;

 

    for (;;) {

        LIS3DH_updateReadings();

        RunMotor();

        // Calculate distance after the pulse is complete

        uint32_t time_diff_us = fall_time - rise_time; // Calculate time difference

        float distance_cm = (time_diff_us / 2.0) * 0.0343; // Distance in cm

 

        float x_tilt = getXTilt();  // Get roll angle

        float y_tilt = getYTilt();  // Get pitch angle

        if (fabs(x_tilt) > TILT_THRESHOLD || fabs(y_tilt) > TILT_THRESHOLD){

            led_data[0] = (0 << 24) | (255 << 16) | (0 << 8); // Green color

            motorTiltCondition = 1;

        }

        else{

            led_data[0] = 0;

            //measure_trig_pulse_duration();

            motorTiltCondition = 0;

            //sleep_ms(100);

        }

        

        if (motorTiltCondition == 1 || motorUltraCondition == 1){

            StopMotor();

        }

        else{

            StartMotor();

        }

        pio_sm_put_blocking(pio0, 0, led_data[0]);
    }

       
}


 

 