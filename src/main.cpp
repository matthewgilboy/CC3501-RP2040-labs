#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "WS2812.pio.h" // This header file gets produced during compilation from the WS2812.pio file
#include "drivers/logging/logging.h"
#include <iostream>
#include "QualityCheck.h"

int main() {
    // Sample values from sensors
    float weightSensorValue = 155.0;
    int detectedShape = 1;  // Assume 1 represents a certain shape
    int detectedRed = 255, detectedGreen = 100, detectedBlue = 50;
    
    // Acceptable weight range
    float minWeight = 150.0;
    float maxWeight = 160.0;

    // Acceptable shape and color values
    int acceptableShape = 1;
    int acceptableRed = 255, acceptableGreen = 100, acceptableBlue = 50;

    // Perform the weight check with a range
    if (checkWeight(weightSensorValue, minWeight, maxWeight)) {
        std::cout << "Weight is acceptable." << std::endl;
    } else {
        std::cout << "Weight is unacceptable." << std::endl;
    }

    // Perform the shape check
    if (checkShape(detectedShape, acceptableShape)) {
        std::cout << "Shape is acceptable." << std::endl;
    } else {
        std::cout << "Shape is unacceptable." << std::endl;
    }

    // Perform the color check
    if (checkColor(detectedRed, detectedGreen, detectedBlue, 
                   acceptableRed, acceptableGreen, acceptableBlue)) {
        std::cout << "Color is acceptable." << std::endl;
    } else {
        std::cout << "Color is unacceptable." << std::endl;
    }

    return 0;
}
