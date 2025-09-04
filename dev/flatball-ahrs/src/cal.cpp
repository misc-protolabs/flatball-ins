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

#include "cal.h"

// SN:yyyymmdd-nn magnetic calibrations
const char SN_str[32] = "SN:20241106-03";

// AHRS alogrithm settings
const FusionAhrsSettings k_ahrs_settings = {
    .convention = FusionConventionNed,
    .gain = 5.0f,                   // High trust in gyro due to rapid spin
    .gyroscopeRange = 4000.0f,      // Matches actual IMU config
    .accelerationRejection = 20.0f, // Reject wild accel during flight
    .magneticRejection = 30.0f,     // Strong rejection to avoid yaw drift
    .recoveryTriggerPeriod = 50,    // Fast recovery after rejection
};

// accelerometer calibration
const FusionMatrix k_ahrs_accelerometerMisalignment = {
    1,0,0,0,1,0,0,0,1};
//    0.9964, 0.0007, -0.0010,
//    0.0007, 1.0040, -0.0014,
//    -0.0010, -0.0014, 0.9997};
const FusionVector k_ahrs_accelerometerSensitivity = {1.0f, 1.0f, 1.0f};
const FusionVector k_ahrs_accelerometerOffset = {
    0, 0, 0};
//    -0.0165, -0.0007, -0.0279};

// gyroscope calibration
const FusionMatrix k_ahrs_gyroscopeMisalignment = {
    1,0,0,0,1,0,0,0,1};
//    1.3375, 0, 0,
//    0, 1.1711, 0,
//    0, 0, 0.6385};
const FusionVector k_ahrs_gyroscopeSensitivity = {1.0f, 1.0f, 1.0f};
const FusionVector k_ahrs_gyroscopeOffset = {
    0, 0, 0};
//    2.8813, -5.2525, -87.5852};

// magnetometer calibration
const FusionMatrix k_ahrs_softIronMatrix = {
    1,0,0,0,1,0,0,0,1};
//    1.0097, 0.0040, 0.0012,
//    0.0040, 0.9928, -0.0111,
//    0.0012, -0.0111, 0.9978};
const FusionVector k_ahrs_hardIronOffset = {
    0, 0, 0};
//    -4.4365, 14.9180, 23.1223};

const unsigned int k_ahrs_hz = 100;

const short k_sleep_dly = 30;
const float k_gz_filt_thr = 1.5;
const float k_acc_filt_thr = 1.1;
const float k_dmag_thr = 50.0;
