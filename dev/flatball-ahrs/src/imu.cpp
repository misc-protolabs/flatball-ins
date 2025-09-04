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

#include <SparkFun_ISM330DHCX.h> //Click here to get the library: http://librarymanager/All#SparkFun_6DoF_ISM330DHCX

#include "imu.h"
#include "vfb.h"

#define IMU_USE_ISR 1
#define IMU_USE_I2C 0

#if IMU_USE_I2C == 1
SparkFun_ISM330DHCX imu6dof;
#else
// SPI instance class call
SparkFun_ISM330DHCX_SPI imu6dof;
#endif
static volatile bool IMUInt1Seen = false; // Flag to indicate if an IMU interrupt has been seen

void IMUInt1ISR() // Interrupt Service Routine
{
  IMUInt1Seen = true;
}

void imu_init(void)
{

  // Begin the IMU
#if IMU_USE_I2C == 1
  if (!imu6dof.begin())
  {
    // Serial.println(F("misc::ISMDH330DHCX::IMU did not begin. Freezing..."));
    while (1)
      ;
  }
#else
  if (!imu6dof.begin(IMU_CS))
  {
    Serial.println(F("misc::ISMDH330DHCX::IMU did not begin. Freezing..."));
    while (1)
      ;
  }
#endif

  Serial.println("misc::ISMDH330DHCX::configuration...");

  // Reset the device to default settings
  // This if helpful is you're doing multiple uploads testing different settings.
  imu6dof.deviceReset();

  // Wait for it to finish reseting
  while (!imu6dof.getDeviceReset())
  {
    delay(1);
  }

  // Serial.println(F("IMU has been reset."));
  // Serial.println(F("Applying settings..."));
  delay(100);

  imu6dof.setDeviceConfig();
  imu6dof.setBlockDataUpdate();

  // Set the output data rate and precision of the accelerometer
  imu6dof.setAccelDataRate(ISM_XL_ODR_104Hz);
  imu6dof.setAccelFullScale(ISM_16g);

  // Set the output data rate and precision of the gyroscope
  imu6dof.setGyroDataRate(ISM_GY_ODR_104Hz);
  imu6dof.setGyroFullScale(ISM_4000dps);

  // Turn on the accelerometer's filter and apply settings.
  imu6dof.setAccelFilterLP2();
  imu6dof.setAccelSlopeFilter(ISM_LP_ODR_DIV_100);

  // Turn on the gyroscope's filter and apply settings.
  imu6dof.setGyroFilterLP1();
  imu6dof.setGyroLP1Bandwidth(ISM_ULTRA_LIGHT);

  // Set the accelerometer's status i.e. the data ready to interrupt one.
  imu6dof.setAccelStatustoInt1();

  // This function call will modify which "events" trigger an interrupt.
  imu6dof.setIntNotification(ISM_ALL_INT_PULSED);

  imu6dof.setDataReadyMode(1); // Set data ready mode to pulsed

  imu6dof.setPinMode(false); // Set the pin mode to push-pull, active high

  // Attach the interrupt
  attachInterrupt(IMU_INT1, IMUInt1ISR, RISING);

#if IMU_USE_I2C == 1
  Serial.println("misc::ISMDH330DHCX::I2C.");
#else
  Serial.println("misc::ISMDH330DHCX::SPI.");
#endif
}

float k_acc_raw2mg = 4.88e-4 * 1e3; // 1 mg = 0.001 g, 1 LSB = 488.0 uG
float k_gyr_raw2mdps = 14e-2 * 1e3; // 1 mdps = 0.001 deg/s, 1 LSB = 140.0 uDPS
void imu_step(float *ax, float *ay, float *az, float *gx, float *gy, float *gz)
{
  static float ax_g_z1, ay_g_z1, az_g_z1;
  static float gx_dps_z1, gy_dps_z1, gz_dps_z1;
  sfe_ism_data_t accel_raw;
  sfe_ism_data_t gyro_raw;
  float ax_g, ay_g, az_g;
  float gx_dps, gy_dps, gz_dps;

  // if( imu6dof.checkStatus()) {
  if (IMUInt1Seen) // Have we seen an interrupt?
  {
    sfe_ism_raw_data_t acc_raw, gyr_raw;
    IMUInt1Seen = false;                        // Clear the flag
    imu6dof.getRaw_acc_gyr(&acc_raw, &gyr_raw); // Get the raw accelerometer and gyroscope data

    accel_raw.xData = acc_raw.xData * k_acc_raw2mg;  // mg
    accel_raw.yData = acc_raw.yData * k_acc_raw2mg;  // mg
    accel_raw.zData = acc_raw.zData * k_acc_raw2mg;  // mg
    gyro_raw.xData = gyr_raw.xData * k_gyr_raw2mdps; // mdps
    gyro_raw.yData = gyr_raw.yData * k_gyr_raw2mdps; // mdps
    gyro_raw.zData = gyr_raw.zData * k_gyr_raw2mdps; // mdps
    // imu6dof.getAccel( &accel_raw);        // mg
    // imu6dof.getGyro( &gyro_raw);          // mdps

    ax_g = (accel_raw.xData * 0.001); // g
    ay_g = (accel_raw.yData * 0.001); // g
    az_g = (accel_raw.zData * 0.001); // 1g =~ 9.806 m/s^2

    gx_dps = (gyro_raw.xData * 0.001); // deg/s
    gy_dps = (gyro_raw.yData * 0.001); // deg/s
    gz_dps = (gyro_raw.zData * 0.001); // deg/s
  }
  else
  {
    ax_g = ax_g_z1;
    ay_g = ay_g_z1;
    az_g = az_g_z1;
    gx_dps = gx_dps_z1;
    gy_dps = gy_dps_z1;
    gz_dps = gz_dps_z1;
  }

  *ax = ax_g; //(ax_g + ax_g_z1) / 2.0;
  *ay = ay_g; //(ay_g + ay_g_z1) / 2.0;
  *az = az_g; //(az_g + az_g_z1) / 2.0;

  *gx = gx_dps; //(gx_dps + gx_dps_z1) / 2.0;
  *gy = gy_dps; //(gy_dps + gy_dps_z1) / 2.0;
  *gz = gz_dps; //(gz_dps + gz_dps_z1) / 2.0;

  //*degC = (float)( imu6dof.getTemp() / 256.0f) + 25.0f; // degC

  // update states
  ax_g_z1 = ax_g;
  ay_g_z1 = ay_g;
  az_g_z1 = az_g;
  gx_dps_z1 = gx_dps;
  gy_dps_z1 = gy_dps;
  gz_dps_z1 = gz_dps;
}

void get_imu_temp(float *degC)
{
  *degC = (float)(imu6dof.getTemp() / 256.0f) + 25.0f; // degC
}