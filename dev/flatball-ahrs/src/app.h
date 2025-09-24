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

#ifndef _APP_H_
#define _APP_H_

extern bool app_init(void);
extern void app_step_100Hz(void);
extern void app_step_10Hz(void);
extern void app_step_1Hz(void);

enum fdevFlightState
{
    IDLE,
    LAUNCH,
    IN_FLIGHT,
    FLIGHT_COMPLETE
};

extern float gx_raw, gy_raw, gz_raw;
extern float ax_raw, ay_raw, az_raw;
extern float mx_raw, my_raw, mz_raw;
extern float gx, gy, gz;
extern float ax, ay, az;
extern float mx, my, mz;
extern float qw, qx, qy, qz;
extern float pitch, roll, yaw;
extern float fe_ax, fe_ay, fe_az;
extern float fl_ax, fl_ay, fl_az;
extern float degC, patm;
extern float v_batt, soc, soc_filt; //, soc_rate_of_change
extern unsigned long tstamp;
extern int fdev_st, fdev_flight_st;
extern int fdev_st_z1, fdev_flight_st_z1;
extern float dt_fusion;
extern float patm_filt, degC_filt;
extern float acc_filt, gz_filt;

// fusion internals
extern float ahrs_accel_err;
extern float ahrs_accel_recovery_trig;
// extern bool ahrs_accel_ignored;
extern float ahrs_mag_err;
extern float ahrs_mag_recovery_trig;
// extern bool ahrs_mag_ignored;
extern char ahrs_flgs;

#endif
