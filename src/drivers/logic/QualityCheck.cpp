#include "QualityCheck.h"
#include <cmath>       // Include for mathematical functions like abs
#include <cstdlib>     // Include for general-purpose functions like exit

// Function to check the weight of the part within a range
bool checkWeight(float weightSensorValue, float minWeight, float maxWeight) {
    // Return true if the weight is within the specified range
    return (weightSensorValue >= minWeight) && (weightSensorValue <= maxWeight);
}

// Function to check the shape of the part
bool checkShape(int detectedShape, int acceptableShape) {
    // Return true if the detected shape matches the acceptable shape
    return detectedShape == acceptableShape;
}

// Function to check the color of the part
bool checkColor(int detectedRed, int detectedGreen, int detectedBlue, 
                int acceptableRed, int acceptableGreen, int acceptableBlue) {
    const int colorTolerance = 10; // Allowable color tolerance (in RGB values)

    // Check if the detected color is within the acceptable range, considering tolerance
    return (abs(detectedRed - acceptableRed) <= colorTolerance) &&
           (abs(detectedGreen - acceptableGreen) <= colorTolerance) &&
           (abs(detectedBlue - acceptableBlue) <= colorTolerance);
}
