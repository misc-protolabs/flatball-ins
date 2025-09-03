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
