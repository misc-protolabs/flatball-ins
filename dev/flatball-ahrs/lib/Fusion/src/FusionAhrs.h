// Copyright 2025 Michael V. Schaefer
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at:
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @file FusionAhrs.h
 * @author Seb Madgwick
 * @brief AHRS algorithm to combine gyroscope, accelerometer, and magnetometer
 * measurements into a single measurement of orientation relative to the Earth.
 */

#ifndef FUSION_AHRS_H
#define FUSION_AHRS_H

//------------------------------------------------------------------------------
// Includes

#include "FusionConvention.h"
#include "FusionMath.h"
#include <stdbool.h>

//------------------------------------------------------------------------------
// Definitions

/**
 * @brief AHRS algorithm settings.
 */
typedef struct {
    FusionConvention convention;
    float gain;
    float gyroscopeRange;
    float accelerationRejection;
    float magneticRejection;
    unsigned int recoveryTriggerPeriod;
} FusionAhrsSettings;

/**
 * @brief AHRS algorithm structure. Structure members are used internally and
 * must not be accessed by the application.
 */
typedef struct {
    FusionAhrsSettings settings;
    FusionQuaternion quaternion;
    FusionVector accelerometer;
    bool initialising;
    float rampedGain;
    float rampedGainStep;
    bool angularRateRecovery;
    FusionVector halfAccelerometerFeedback;
    FusionVector halfMagnetometerFeedback;
    bool accelerometerIgnored;
    int accelerationRecoveryTrigger;
    int accelerationRecoveryTimeout;
    bool magnetometerIgnored;
    int magneticRecoveryTrigger;
    int magneticRecoveryTimeout;
} FusionAhrs;

/**
 * @brief AHRS algorithm internal states.
 */
typedef struct {
    float accelerationError;
    bool accelerometerIgnored;
    float accelerationRecoveryTrigger;
    float magneticError;
    bool magnetometerIgnored;
    float magneticRecoveryTrigger;
} FusionAhrsInternalStates;

/**
 * @brief AHRS algorithm flags.
 */
typedef struct {
    bool initialising;
    bool angularRateRecovery;
    bool accelerationRecovery;
    bool magneticRecovery;
} FusionAhrsFlags;

//------------------------------------------------------------------------------
// Function declarations

void FusionAhrsInitialise(FusionAhrs *const ahrs);

void FusionAhrsReset(FusionAhrs *const ahrs);

void FusionAhrsSetSettings(FusionAhrs *const ahrs, const FusionAhrsSettings *const settings);

void FusionAhrsUpdate(FusionAhrs *const ahrs, const FusionVector gyroscope, const FusionVector accelerometer, const FusionVector magnetometer, const float deltaTime);

void FusionAhrsUpdateNoMagnetometer(FusionAhrs *const ahrs, const FusionVector gyroscope, const FusionVector accelerometer, const float deltaTime);

void FusionAhrsUpdateExternalHeading(FusionAhrs *const ahrs, const FusionVector gyroscope, const FusionVector accelerometer, const float heading, const float deltaTime);

FusionQuaternion FusionAhrsGetQuaternion(const FusionAhrs *const ahrs);

void FusionAhrsSetQuaternion(FusionAhrs *const ahrs, const FusionQuaternion quaternion);

FusionVector FusionAhrsGetGravity(const FusionAhrs *const ahrs);

FusionVector FusionAhrsGetLinearAcceleration(const FusionAhrs *const ahrs);

FusionVector FusionAhrsGetEarthAcceleration(const FusionAhrs *const ahrs);

FusionAhrsInternalStates FusionAhrsGetInternalStates(const FusionAhrs *const ahrs);

FusionAhrsFlags FusionAhrsGetFlags(const FusionAhrs *const ahrs);

void FusionAhrsSetHeading(FusionAhrs *const ahrs, const float heading);

#endif

//------------------------------------------------------------------------------
// End of file
