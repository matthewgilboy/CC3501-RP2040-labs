// QualityCheck.h

#ifndef QUALITYCHECK_H
#define QUALITYCHECK_H

// Declare the function to check the weight of the part within a specified range
bool checkWeight(float weightSensorValue, float minWeight, float maxWeight);

// Declare the function to check the shape of the part against an acceptable shape
bool checkShape(int detectedShape, int acceptableShape);

// Declare the function to check the color of the part
// Parameters are detected RGB values and acceptable RGB values
bool checkColor(int detectedRed, int detectedGreen, int detectedBlue, 
                int acceptableRed, int acceptableGreen, int acceptableBlue);

#endif // QUALITYCHECK_H
