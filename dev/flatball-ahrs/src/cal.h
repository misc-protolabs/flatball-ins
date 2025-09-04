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

#ifndef _CAL_H_
#define _CAL_H_

#include "Fusion.h"
// Define calibration (replace with actual calibration data if available)
extern const FusionMatrix k_ahrs_gyroscopeMisalignment;
extern const FusionVector k_ahrs_gyroscopeSensitivity;
extern const FusionVector k_ahrs_gyroscopeOffset;

extern const FusionMatrix k_ahrs_accelerometerMisalignment;
extern const FusionVector k_ahrs_accelerometerSensitivity;
extern const FusionVector k_ahrs_accelerometerOffset;

extern const FusionMatrix k_ahrs_softIronMatrix;
extern const FusionVector k_ahrs_hardIronOffset;

// Set AHRS algorithm settings
extern const FusionAhrsSettings k_ahrs_settings;

extern const short k_sleep_dly;
extern const float k_gz_filt_thr;
extern const float k_acc_filt_thr;
extern const float k_dmag_thr;

extern const unsigned int k_ahrs_hz;

extern const char SN_str[32];
extern uint8_t baseMACAddress[6];
extern uint8_t remoteMACAddress[6];

#endif
