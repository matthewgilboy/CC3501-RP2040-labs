#include "QualityCheck.h"
#include <cmath>
#include <cstdlib>

// Function to check the weight of the part within a range
bool checkWeight(float weightSensorValue, float minWeight, float maxWeight) {
    return (weightSensorValue >= minWeight) && (weightSensorValue <= maxWeight);
}

// Function to check the shape of the part
bool checkShape(int detectedShape, int acceptableShape) {
    return detectedShape == acceptableShape;
}

// Function to check the color of the part
bool checkColor(int detectedRed, int detectedGreen, int detectedBlue, 
                int acceptableRed, int acceptableGreen, int acceptableBlue) {
    const int colorTolerance = 10; // Allowable color tolerance (in RGB values)
    return (abs(detectedRed - acceptableRed) <= colorTolerance) &&
           (abs(detectedGreen - acceptableGreen) <= colorTolerance) &&
           (abs(detectedBlue - acceptableBlue) <= colorTolerance);
}
