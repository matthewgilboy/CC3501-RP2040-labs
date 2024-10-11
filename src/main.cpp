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

int main() {
    stdio_init_all();
    init_Mag_Switch(MAGSIG);
    
    while (true){
        if (Mag_Switch_Active_True(MAGSIG)){
            printf("%d", 1);
        } else {
            printf("%d", 0);
        }
        sleep_ms(100);
    }

return 0;
}
//     // Sample values from sensors
//     float weightSensorValue = 155.0;
//     int detectedShape = 1;  // Assume 1 represents a certain shape
//     int detectedRed = 255, detectedGreen = 100, detectedBlue = 50;
    
//     // Acceptable weight range
//     float minWeight = 150.0;
//     float maxWeight = 160.0;

//     // Acceptable shape and color values
//     int acceptableShape = 1;
//     int acceptableRed = 255, acceptableGreen = 100, acceptableBlue = 50;

//     // Perform the weight check with a range
//     if (checkWeight(weightSensorValue, minWeight, maxWeight)) {
//         std::cout << "Weight is acceptable." << std::endl;
//     } else {
//         std::cout << "Weight is unacceptable." << std::endl;
//     }

//     // Perform the shape check
//     if (checkShape(detectedShape, acceptableShape)) {
//         std::cout << "Shape is acceptable." << std::endl;
//     } else {
//         std::cout << "Shape is unacceptable." << std::endl;
//     }

//     // Perform the color check
//     if (checkColor(detectedRed, detectedGreen, detectedBlue, 
//                    acceptableRed, acceptableGreen, acceptableBlue)) {
//         std::cout << "Color is acceptable." << std::endl;
//     } else {
//         std::cout << "Color is unacceptable." << std::endl;
//     }

//     return 0;
// }
