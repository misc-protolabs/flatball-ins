#include "twister.h"
#include <Arduino.h>
#include <cmath>
#include <vector>

#include "comms.h"
#include "app.h"

void filterGyroData(void);
float getPeakGyro(void);

// Variables for twist detection
float twistThreshold = 750.0;  // Threshold for detecting a twist (deg/sec)
float twistTimeout = 1000;    // Maximum duration for detecting a twist (in ms)
float twistInterval = 200;   // Minimum interval between twists (in ms)
unsigned long lastTwistTime = 0;
int twistCount = 0;
bool twist_1x, twist_2x, twist_3x;

// Variables for filtering
float filtered_gx = 0, filtered_gy = 0;
float alpha_gyro = 0.05;  // Filter constant

// Variables for peak detection
float gyroNorm;
std::vector<float> gyroHistory;
int historySizeGyro = 25;  // Number of samples to keep in history

void initializeTwister(void) {
	gyroHistory.clear();
	twist_1x = 0;
	twist_2x = 0;
	twist_3x = 0;
}

float getPeakGyro() {
    float peakGyro = 0;
    for (float val : gyroHistory) {
        if (val > peakGyro) {
            peakGyro = val;
        }
    }
    return peakGyro;
}

void filterGyroData() {
    // Apply first-order filter
    filtered_gx = alpha_gyro * gx + (1 - alpha_gyro) * filtered_gx;
    filtered_gy = alpha_gyro * gy + (1 - alpha_gyro) * filtered_gy;
}

void detectTwists() {
    // Filter gyro data
    //filterGyroData();

    // Calculate the norm of the gyroscope data
    //gyroNorm = sqrt(sq(filtered_gx) + sq(filtered_gy));
    gyroNorm = sqrt(sq(gx) + sq(gy));

    // Add current gyro norm to history
    if (gyroHistory.size() >= historySizeGyro) {
        gyroHistory.erase(gyroHistory.begin());  // Remove oldest sample
    }
    gyroHistory.push_back(gyroNorm);

    unsigned long currentTime = millis();
    static bool twistDetected = false; // Add a flag to indicate if a twist was detected

	float pkGyro = getPeakGyro();
    if (pkGyro > twistThreshold) {
        if (!twistDetected && currentTime - lastTwistTime > twistInterval) {
            twistDetected = true;
			gyroHistory.clear();
            twistCount++;
            lastTwistTime = currentTime;
            Serial.printf("twist detected - %7.3f, %7.3f\n", gyroNorm, pkGyro);
        }
    } else {
        twistDetected = false;  // Reset the flag when gyro drops below the threshold
    }

    if (currentTime - lastTwistTime > twistTimeout) {
        if (twistCount == 1) {
            twist_1x = 1;
            //Serial.printf("Single twist detected - %7.3f, %7.3f\n", gyroNorm, pkGyro);
        } else if (twistCount == 2) {
            twist_2x = 1;
            //Serial.printf("Double twist detected - %7.3f, %7.3f\n", gyroNorm, pkGyro);
        } else if (twistCount == 3) {
            twist_3x = 1;
            //Serial.printf("Triple twist detected - %7.3f, %7.3f\n", gyroNorm, pkGyro);
        }
        twistCount = 0;  // Reset twist count
    }
}
