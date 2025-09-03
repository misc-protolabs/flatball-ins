#include "Arduino.h"
#include "Fusion.h"
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include "app.h"
#include "vfb.h"
#include "cal.h"

#include "imu.h"
#include "mag.h"
// #include "src/lipo.h"
#include "twister.h"
#include "sd_log.h"
#include "srvr.h"
#include "util.h"

float gx, gy, gz;
float ax, ay, az;
float mx, my, mz;
float qw, qx, qy, qz;
float pitch, roll, yaw;
// float gravx, gravy, gravz;
float fe_ax, fe_ay, fe_az;
float fl_ax, fl_ay, fl_az;
float patm, degC;
float patm_filt, degC_filt;
float acc_filt, gz_filt;
float v_batt, soc, soc_rate_of_change, soc_filt;
int fdev_st, fdev_flight_st;
int fdev_st_z1, fdev_flight_st_z1;
unsigned long tstamp;
float dt_fusion;

FusionOffset offset;
FusionAhrs ahrs;
FusionAhrsInternalStates ahrs_st;
FusionAhrsFlags ahrs_flags;

// fusion internals
float ahrs_accel_err;
float ahrs_accel_recovery_trig;
// bool ahrs_accel_ignored;
float ahrs_mag_err;
float ahrs_mag_recovery_trig;
// bool ahrs_mag_ignored;
char ahrs_flgs;

bool app_init(void)
{
  FusionOffsetInitialise(&offset, k_ahrs_hz);
  FusionAhrsInitialise(&ahrs);
  FusionAhrsSetSettings(&ahrs, &k_ahrs_settings);

  ax = 0;
  ay = 0;
  az = 0;
  gx = 0;
  gy = 0;
  gz = 0;
  mx = 0;
  my = 0;
  mz = 0;
  qw = 0;
  qx = 0;
  qy = 0;
  qz = 0;
  // gravx = 0; gravy = 0; gravz = 0;
  patm = 0;
  degC = 0;
  v_batt = 0;
  soc = 0;
  soc_rate_of_change = 0;
  acc_filt = 0.0;
  gz_filt = 0.0;
  patm_filt = 0.0;
  fdev_st = 0;
  fdev_flight_st = IDLE;
  fdev_st_z1 = 0;
  fdev_flight_st_z1 = IDLE;

  // initializeTwister();

  return 1;
}

const float launchThreshold = 1000;
const float inFlightThreshold = 500;
const float flightCompleteThreshold = 250;
const float idleThreshold = 50;
void fdevStateMachine()
{
  float abs_gz = fabs(gz);
  switch (fdev_flight_st)
  {
  case IDLE:
    // Check for conditions to transition to LAUNCH
    if (abs_gz > launchThreshold)
    {
      fdev_flight_st = LAUNCH;
    }
    break;

  case LAUNCH:
    // Check for conditions to transition to IN_FLIGHT if the previous state was LAUNCH
    if (abs_gz > inFlightThreshold && fdev_flight_st_z1 == LAUNCH)
    {
      fdev_flight_st = IN_FLIGHT;
      fdev_st = 2;
    }
    break;

  case IN_FLIGHT:
    // Check for conditions to transition to FLIGHT_COMPLETE
    if (abs_gz < flightCompleteThreshold)
    {
      fdev_flight_st = FLIGHT_COMPLETE;
    }
    break;

  case FLIGHT_COMPLETE:
    // Check for conditions to transition to IDLE
    if (abs_gz < idleThreshold)
    {
      fdev_flight_st = IDLE;
      fdev_st = 0;
    }
    break;
  }

  /*
    //detectTwists();
    // fdev_st {0 --> idle, 1 --> srvr, 2 --> logging}
    if( fdev_st == 0) {
      if (fdev_st_z1 == 2) {
        sd_log_close();
      }
      if( fdev_st_z1 == 1) {
        srvr_stop();
      }
      if( twist_2x) {
        fdev_st = 2;
        twist_2x = 0;
      } else if( twist_3x) {
        fdev_st = 1;
        twist_3x = 0;
      }
    }
    if (fdev_st == 1) {
      if (fdev_st_z1 == 2) {
        sd_log_close();
      }
      if (fdev_st_z1 != 1) {
        srvr_init();
      }
      if( twist_2x) {
        fdev_st = 0;
        twist_2x = 0;
      } else if( twist_3x) {
        fdev_st = 0;
        twist_3x = 0;
      }
    } else if (fdev_st == 2) {
      if( fdev_st_z1 == 1) {
        srvr_stop();
      }
      if (fdev_st_z1 != 2) {
        unsigned int n_log = sd_log_new();
      }
      if( twist_2x) {
        fdev_st = 0;
        twist_2x = 0;
      } else if( twist_3x) {
        fdev_st = 0;
        twist_3x = 0;
      }
    }

    fdev_st_z1 = fdev_st;
  */
  fdev_flight_st_z1 = fdev_flight_st; // Update previous state
}

void app_step_100Hz(void)
{
  float dt = 0.01;                        // 100Hz
  FusionVector gyro_raw = {gy, gx, -gz};  // NED
  FusionVector accel_raw = {ay, ax, -az}; // NED
  FusionVector mag_raw = {my, mx, mz};    // NED

  FusionVector gyroscope = FusionCalibrationInertial(
      gyro_raw,
      k_ahrs_gyroscopeMisalignment,
      k_ahrs_gyroscopeSensitivity,
      k_ahrs_gyroscopeOffset);

  FusionVector accelerometer = FusionCalibrationInertial(
      accel_raw,
      k_ahrs_accelerometerMisalignment,
      k_ahrs_accelerometerSensitivity,
      k_ahrs_accelerometerOffset);

  FusionVector magnetometer = mag_raw;
  if (mag_present)
  {
    magnetometer = FusionCalibrationMagnetic(
        mag_raw,
        k_ahrs_softIronMatrix,
        k_ahrs_hardIronOffset);
  }

  // Update gyroscope offset correction algorithm
  gyroscope = FusionOffsetUpdate(&offset, gyroscope);

  // Update gyroscope AHRS algorithm
  if (!mag_present)
  {
    FusionAhrsUpdateNoMagnetometer(&ahrs, gyroscope, accelerometer, dt);
  }
  else
  {
    FusionAhrsUpdate(&ahrs, gyroscope, accelerometer, magnetometer, dt);
  }

  // algorithm outputs
  const FusionQuaternion quat = FusionAhrsGetQuaternion(&ahrs);
  const FusionVector earth = FusionAhrsGetEarthAcceleration(&ahrs);
  const FusionVector lin_acc = FusionAhrsGetLinearAcceleration(&ahrs);
  const FusionVector grav_vec = FusionAhrsGetGravity(&ahrs);
  const FusionEuler euler = FusionQuaternionToEuler(quat);

  ahrs_st = FusionAhrsGetInternalStates(&ahrs);
  ahrs_accel_err = ahrs_st.accelerationError;
  ahrs_accel_recovery_trig = ahrs_st.accelerationRecoveryTrigger;
  // ahrs_accel_ignored = ahrs_st.accelerometerIgnored;
  ahrs_mag_err = ahrs_st.magneticError;
  ahrs_mag_recovery_trig = ahrs_st.magneticRecoveryTrigger;
  // ahrs_mag_ignored = ahrs_st.magnetometerIgnored;

  ahrs_flags = FusionAhrsFlags();
  ahrs_flgs |= ahrs_flags.initialising;
  ahrs_flgs |= ahrs_flags.angularRateRecovery << 1;
  ahrs_flgs |= ahrs_flags.accelerationRecovery << 2;
  ahrs_flgs |= ahrs_flags.magneticRecovery << 3;
  ahrs_flgs |= ahrs_st.accelerometerIgnored << 4;
  ahrs_flgs |= ahrs_st.magnetometerIgnored << 5;

  qw = quat.element.w;
  qx = quat.element.x;
  qy = quat.element.y;
  qz = quat.element.z;

  // gravx = grav_vec.x;
  // gravy = grav_vec.y;
  // gravz = grav_vec.z;

  roll = euler.angle.roll;
  pitch = euler.angle.pitch;
  yaw = euler.angle.yaw;

  fe_ax = earth.axis.x;
  fe_ay = earth.axis.y;
  fe_az = earth.axis.z;

  fl_ax = lin_acc.axis.x;
  fl_ay = lin_acc.axis.y;
  fl_az = lin_acc.axis.z;

  // patm_filt = filt_1ord( patm, patm_filt_z1, 0.10, 0.01);

  // fdevStateMachine();
  // patm_filt_z1 = patm_filt;
}

void app_step_10Hz(void)
{
  float dt = 0.01;
  static float acc_filt_z1, gz_filt_z1;
  static float degC_filt_z1;

  float acc = sqrt(ax * ax + ay * ay + az * az);
  acc_filt = filt_1ord(acc, acc_filt_z1, 1.0, dt);
  gz_filt = filt_1ord(gz, gz_filt_z1, 1.0, dt);
  degC_filt = filt_1ord(degC, degC_filt_z1, 1.0, dt);

  acc_filt_z1 = acc_filt;
  gz_filt_z1 = gz_filt;
  degC_filt_z1 = degC_filt;
}

void app_step_1Hz(void)
{
  static float soc_filt_z1;

  soc_filt = filt_1ord(soc, soc_filt_z1, 2.0, 1.0);
  // soc_rate_of_change = (soc_filt - soc_filt_z1) * 3600; // SOC/hour

  soc_filt_z1 = soc_filt;
}
