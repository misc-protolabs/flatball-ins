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

// tst_rd_mag3dof.cpp - SPI bandwidth stress test for MMC5983MA
#include <Arduino.h>
#include <SPI.h>
#include <SparkFun_MMC5983MA_Arduino_Library.h> //Click here to get the library: http://librarymanager/All#SparkFun_MMC5983MA
#include "vfb.h"
#include "esp_sleep.h"

TaskHandle_t hndl_100hz;
void task_100hz(void *pvParameters);
void tst_loop(void);

constexpr uint32_t TEST_DURATION_MS = 5000;

SFE_MMC5983MA mag3dof;
SPISettings magSettings(SPI_FREQ, MSBFIRST, SPI_MODE0);

uint32_t k_mag_x_offst = 131072;
uint32_t k_mag_y_offst = 131072;
uint32_t k_mag_z_offst = 131072;

float k_mag_cal_b[3] = {-2.9743, 16.0530, -4.9251};
float k_mag_cal_A[3][3] = {
    {1.0190, 0.0037, 0.0070},
    {0.0037, 0.9851, 0.0161},
    {0.0070, 0.0161, 0.9965}};

uint32_t startTime;
uint32_t loopCount = 0;
uint32_t readCount = 0;
uint32_t readFailCount = 0;
float elapsedUs = 0.0;

void setup()
{
  Serial.begin(115200);
  Serial.print("#test-uuid :: ");
  Serial.println((uint32_t)(esp_random()), HEX);
  Serial.print("#test-time :: ");
  Serial.println(__DATE__ " " __TIME__);

  pinMode(EN_3V3_SW, OUTPUT);
  digitalWrite(EN_3V3_SW, HIGH);
  pinMode(BOOT_BTN, INPUT);
  pinMode(STAT_LED, OUTPUT);
  pinMode(IMU_CS, OUTPUT);
  digitalWrite(IMU_CS, HIGH);
  pinMode(IMU_INT1, INPUT);
  pinMode(MAG_CS, OUTPUT);
  digitalWrite(MAG_CS, HIGH);
  pinMode(MAG_INT, INPUT);
  delay(100);

  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI, SPI_CS_MMC5983MA);
  SPI.setFrequency(SPI_FREQ);

  if (!mag3dof.begin(MAG_CS, magSettings, SPI))
  {
    Serial.println(F("misc::MMC5983MA::Mag did not begin. Freezing..."));
  }

  Serial.println("misc::MMC5983MA::configuration...");

  mag3dof.softReset();

  Serial.println("misc::MMC5983MA::Setting filter bandwith to 800 Hz for continuous operation...");
  mag3dof.setFilterBandwidth(100);
  Serial.print("misc::MMC5983MA::Reading back filter bandwith: ");
  Serial.println(mag3dof.getFilterBandwith());

  // NOTE: 50hz timesout ~1/2 as often as 100hz, so for now, this is the "sampling frequency"
  Serial.println("misc::MMC5983MA::Setting continuous measurement frequency to 50 Hz...");
  mag3dof.setContinuousModeFrequency(100);
  Serial.print("misc::MMC5983MA::Reading back continuous measurement frequency: ");
  Serial.println(mag3dof.getContinuousModeFrequency());

  Serial.println("misc::MMC5983MA::Disabling auto set/reset...");
  // mag3dof.enableAutomaticSetReset();
  mag3dof.disableAutomaticSetReset();
  Serial.print("misc::MMC5983MA::Reading back automatic set/reset: ");
  Serial.println(mag3dof.isAutomaticSetResetEnabled() ? "enabled" : "disabled");

  Serial.println("misc::MMC5983MA::Enabling continuous mode...");
  mag3dof.enableContinuousMode();
  // mag3dof.disableContinuousMode();
  Serial.print("misc::MMC5983MA::Reading back continuous mode: ");
  Serial.println(mag3dof.isContinuousModeEnabled() ? "enabled" : "disabled");

  Serial.println("misc::MMC5983MA::Starting tst_rd_mag3dof stress test...");
  delay(500);

  Serial.println("#test-begin");
  Serial.println("t,x,y,z");

  xTaskCreate(task_100hz, "100Hz", 1024 * 16, NULL, 3, &hndl_100hz);
}

bool mag_step(float *mx, float *my, float *mz)
{
  static float mx_z1, my_z1, mz_z1;
  static uint32_t rd_fail_cnt = 0;
  uint32_t mag_raw_x, mag_raw_y, mag_raw_z;
  float mx_uT, my_uT, mz_uT;
  bool rd_ok = false;

#if MAG_USE_ISR == 1
  if (mag3dof_isr_edge == true)
  {
    mag3dof_isr_edge = false;         // Clear our interrupt flag
    mag3dof.clearMeasDoneInterrupt(); // Clear the MMC5983 interrupt

    // Read all three channels simultaneously
    // Note: we are calling readFieldsXYZ to read the fields, not getMeasurementXYZ
    // The measurement is already complete, we do not need to start a new one
    rd_ok = mag3dof.readFieldsXYZ(&mag_raw_x, &mag_raw_y, &mag_raw_z);
  }
#else
  rd_ok = mag3dof.getMeasurementXYZ(&mag_raw_x, &mag_raw_y, &mag_raw_z);
#endif
  if (!rd_ok)
  {
    rd_fail_cnt++;
  }

  // The magnetic field values are 18-bit unsigned. The zero (mid) point is 2^17 (131072).
  // Normalize each field to +/- 800.0 uT
  mx_uT = ((float)(mag_raw_x) - (float)(k_mag_x_offst)) * (800.0 / 131072.0); // uT
  my_uT = ((float)(mag_raw_y) - (float)(k_mag_y_offst)) * (800.0 / 131072.0); // uT
  mz_uT = ((float)(mag_raw_z) - (float)(k_mag_z_offst)) * (800.0 / 131072.0); // 100 uT == 1 G

  // reject samples > 200 uT as this is likely erroneous - during calibration
  if (abs(mx_uT) >= 200.0)
  {
    mx_uT = mx_z1;
  }
  if (abs(my_uT) >= 200.0)
  {
    my_uT = my_z1;
  }
  if (abs(mz_uT) >= 200.0)
  {
    mz_uT = mz_z1;
  }
  // reject samples > 200 uT as this is likely erroneous - during calibration

  // average k and k-1 samples
  *mx = (mx_uT + mx_z1) / 2.0;
  *my = (my_uT + my_z1) / 2.0;
  *mz = (mz_uT + mz_z1) / 2.0;

  // update states
  mx_z1 = mx_uT;
  my_z1 = my_uT;
  mz_z1 = mz_uT;

  return rd_ok;
}

void task_100hz(void *pvParameters)
{
  static unsigned long timestamp_z1;

  startTime = micros();

  for (;;)
  {
    unsigned long timestamp = micros();
    tst_loop();
    timestamp_z1 = timestamp;
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

#define MAX_SAMPLES 500
float buf_durUs[MAX_SAMPLES];
uint16_t buf_idx = 0;
uint16_t histo[10]; // 10 buckets: 8–17 ms

float buf_mx[MAX_SAMPLES];
float buf_my[MAX_SAMPLES];
float buf_mz[MAX_SAMPLES];

void tst_loop(void)
{
  bool rd_ok;
  float x, y, z;
  uint32_t t0, t1;

  loopCount++;

  t0 = micros();
  rd_ok = mag_step(&x, &y, &z);
  t1 = micros();

  float durationUs = (t1 - t0);

  if (rd_ok)
  {
    if (buf_idx < MAX_SAMPLES)
    {
	  buf_mx[buf_idx] = x;
	  buf_my[buf_idx] = y;
	  buf_mz[buf_idx] = z;
      buf_durUs[buf_idx++] = durationUs;
    }

    Serial.printf("0x%08x,%8.5e,%8.5f,%8.5f,%8.5f\n", loopCount, durationUs * 1e-6, x, y, z);

    readCount++;
  }
  else
  {
    readFailCount++;
  }
}

struct StatsResult
{
  float mean;
  float stddev;
};

StatsResult calculateStats(float *data, size_t count)
{
  float sum = 0.0f;
  for (size_t i = 0; i < count; i++)
  {
    sum += data[i];
  }
  float mean = sum / count;

  float variance = 0.0f;
  for (size_t i = 0; i < count; i++)
  {
    float diff = data[i] - mean;
    variance += diff * diff;
  }

  StatsResult result;
  result.mean = mean;
  result.stddev = sqrt(variance / count);
  return result;
}

void tst_end(void)
{
  StatsResult stats;

  stats = calculateStats(buf_mx, buf_idx);
  Serial.printf("#stats: %8s - n %8i, mean %8.5f (uT), stddev %8.5f (uT)\n",
                "mx", buf_idx, stats.mean, stats.stddev);

  stats = calculateStats(buf_my, buf_idx);
  Serial.printf("#stats: %8s - n %8i, mean %8.5f (uT), stddev %8.5f (uT)\n",
                "my", buf_idx, stats.mean, stats.stddev);

  stats = calculateStats(buf_mz, buf_idx);
  Serial.printf("#stats: %8s - n %8i, mean %8.5f (uT), stddev %8.5f (uT)\n",
                "mz", buf_idx, stats.mean, stats.stddev);

  stats = calculateStats(buf_durUs, buf_idx);
  Serial.printf("#stats: %8s - n %8i, mean %8.5f (us), stddev %8.5f (us)\n",
                "dt", buf_idx, stats.mean, stats.stddev);

  Serial.flush();
  Serial.println("#test-end");
}

void loop(void)
{
  static uint32_t startMsTime = millis();
  uint32_t now = millis();
  if ((now - startMsTime) >= TEST_DURATION_MS)
  {
    vTaskSuspend(hndl_100hz);
    tst_end();
  }
}
