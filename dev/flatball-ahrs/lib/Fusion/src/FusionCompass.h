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
 * @file FusionCompass.h
 * @author Seb Madgwick
 * @brief Tilt-compensated compass to calculate the magnetic heading using
 * accelerometer and magnetometer measurements.
 */

#ifndef FUSION_COMPASS_H
#define FUSION_COMPASS_H

//------------------------------------------------------------------------------
// Includes

#include "FusionConvention.h"
#include "FusionMath.h"

//------------------------------------------------------------------------------
// Function declarations

float FusionCompassCalculateHeading(const FusionConvention convention, const FusionVector accelerometer, const FusionVector magnetometer);

#endif

//------------------------------------------------------------------------------
// End of file
