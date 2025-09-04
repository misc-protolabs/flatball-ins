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
 * @file FusionCompass.c
 * @author Seb Madgwick
 * @brief Tilt-compensated compass to calculate the magnetic heading using
 * accelerometer and magnetometer measurements.
 */

//------------------------------------------------------------------------------
// Includes

#include "FusionAxes.h"
#include "FusionCompass.h"
#include <math.h>

//------------------------------------------------------------------------------
// Functions

/**
 * @brief Calculates the magnetic heading.
 * @param convention Earth axes convention.
 * @param accelerometer Accelerometer measurement in any calibrated units.
 * @param magnetometer Magnetometer measurement in any calibrated units.
 * @return Heading angle in degrees.
 */
float FusionCompassCalculateHeading(const FusionConvention convention, const FusionVector accelerometer, const FusionVector magnetometer) {
    switch (convention) {
        case FusionConventionNwu: {
            const FusionVector west = FusionVectorNormalise(FusionVectorCrossProduct(accelerometer, magnetometer));
            const FusionVector north = FusionVectorNormalise(FusionVectorCrossProduct(west, accelerometer));
            return FusionRadiansToDegrees(atan2f(west.axis.x, north.axis.x));
        }
        case FusionConventionEnu: {
            const FusionVector west = FusionVectorNormalise(FusionVectorCrossProduct(accelerometer, magnetometer));
            const FusionVector north = FusionVectorNormalise(FusionVectorCrossProduct(west, accelerometer));
            const FusionVector east = FusionVectorMultiplyScalar(west, -1.0f);
            return FusionRadiansToDegrees(atan2f(north.axis.x, east.axis.x));
        }
        case FusionConventionNed: {
            const FusionVector up = FusionVectorMultiplyScalar(accelerometer, -1.0f);
            const FusionVector west = FusionVectorNormalise(FusionVectorCrossProduct(up, magnetometer));
            const FusionVector north = FusionVectorNormalise(FusionVectorCrossProduct(west, up));
            return FusionRadiansToDegrees(atan2f(west.axis.x, north.axis.x));
        }
    }
    return 0; // avoid compiler warning
}

//------------------------------------------------------------------------------
// End of file
