#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "drivers/logging/logging.h"
#include <iostream>
#include "drivers/sensors/magneticswitch/magneticswitch.h"
#include "drivers/sensors/accelerometer/accelerometer.h"
#include "drivers/sensors/loadcell/loadcell.h"
#include "drivers/sensors/ultrasonic/ultrasonic.h"
#include "drivers/logic/QualityCheck.h"
#include "drivers/driving/servo/servo.h"
#include "drivers/driving/stepper/stepper.h"
#include "drivers/logic/Case.h"

#define EMGNCY_SW 2
#define HOME_1 3
#define nENBL 4
#define nMODE0 5
#define ACCEL_INT1 6
#define ACCEL_INT2 7
#define TRIG 8
#define ECHO 9
#define nMODE2 10
#define nMODE1 11
#define MAGSIG 12
#define SRVSIG 13
#define LED_DIN 14
#define STEP_1 15
#define LIS3DHTR_SDA 16
#define LIS3DHTR_SCL 17
#define HX711_SDA 18
#define HX711_SCL 19
#define DIR_1 20
#define DECAY 21
#define FAULT_1 22
#define SLEEP_1 23
#define RX_BTOOTH 24
#define TX_BTOOTH 25
#define BTOOTH_CTS 26
#define BTOOTH_RTS 27
#define nRESET 28
#define RST 29

int mode = 0;
uint32_t led_data[1];  // Create an array to hold LED color data


int main(){
// Main function to initialize and test the stepper motor
    stdio_init_all();                         // Initialize standard IO
    init_stepper();                           // Initialize stepper motor driver
    
    while (true) {
        // Move stepper motor 200 steps forward (assuming 1.8 degrees per step for full revolution)
        printf("Moving forward\n");
        move_pwm(1,200);
        sleep_ms(2000);                       // Wait for 2 seconds

        // Move stepper motor 200 steps backward
        printf("Moving backward\n");
        move_pwm(0,200);
        sleep_ms(2000);                       // Wait for 2 seconds
    }
    
    return 0;
}
