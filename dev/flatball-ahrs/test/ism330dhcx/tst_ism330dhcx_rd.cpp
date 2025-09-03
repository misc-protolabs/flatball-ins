// tst_rd_mag3dof.cpp - SPI bandwidth stress test for MMC5983MA
#include <Arduino.h>
#include <SPI.h>
#include <SparkFun_ISM330DHCX.h> //Click here to get the library: http://librarymanager/All#SparkFun_6DoF_ISM330DHCX
#include "esp_sleep.h"

#include "vfb.h"

TaskHandle_t hndl_100hz;
void task_100hz(void *pvParameters);
void tst_loop(void);
constexpr uint32_t TEST_DURATION_MS = 5000;

uint32_t startTime;
uint32_t loopCount = 0;
uint32_t readCount = 0;
uint32_t readFailCount = 0;
float elapsedUs = 0.0;

#define MAX_SAMPLES 500
float buf_durUs[MAX_SAMPLES];
uint16_t buf_idx = 0;
uint16_t histo[10]; // 10 buckets: 8–17 ms

SparkFun_ISM330DHCX_SPI imu6dof;
float ax, ay, az, gx, gy, gz;
static volatile bool IMUInt1Seen = false; // Flag to indicate if an IMU interrupt has been seen
float buf_ax[MAX_SAMPLES];
float buf_ay[MAX_SAMPLES];
float buf_az[MAX_SAMPLES];
float buf_gx[MAX_SAMPLES];
float buf_gy[MAX_SAMPLES];
float buf_gz[MAX_SAMPLES];

void IMUInt1ISR() // Interrupt Service Routine
{
  IMUInt1Seen = true;
}

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

  Serial.println("misc::vfb::peripherhal configuration...");

  Serial.println("misc::vfb::SPI configuration...");
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI, SPI_CS_ISM330DHCX);
  SPI.setFrequency(SPI_FREQ);

  if (!imu6dof.begin(IMU_CS))
  {
    Serial.println(F("misc::ISMDH330DHCX::IMU did not begin. Freezing..."));
    while (1)
      ;
  }

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

  Serial.println("misc::ISMDH330DHCX::SPI.");

  Serial.println("#test-begin");
  Serial.println("idx,dt,ax,ay,az,gx,gy,gz");

  xTaskCreate(task_100hz, "100Hz", 1024 * 16, NULL, 3, &hndl_100hz);
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

void tst_loop(void)
{
  bool rd_ok;
  uint32_t t0, t1;
  sfe_ism_data_t accel_raw;
  sfe_ism_data_t gyro_raw;

  loopCount++;

  t0 = micros();
  if (IMUInt1Seen) // Have we seen an interrupt?
  {
    IMUInt1Seen = false;          // Clear the flag
    imu6dof.getAccel(&accel_raw); // mg
    imu6dof.getGyro(&gyro_raw);   // mdps

    ax = (accel_raw.xData * 0.001); // g
    ay = (accel_raw.yData * 0.001); // g
    az = (accel_raw.zData * 0.001); // 1g =~ 9.806 m/s^2

    gx = (gyro_raw.xData * 0.001); // deg/s
    gy = (gyro_raw.yData * 0.001); // deg/s
    gz = (gyro_raw.zData * 0.001); // deg/s

    rd_ok = true;
  }
  t1 = micros();

  float durationUs = (t1 - t0);

  if (rd_ok)
  {
    if (buf_idx < MAX_SAMPLES)
    {
	  buf_ax[buf_idx] = ax;
	  buf_ay[buf_idx] = ay;
	  buf_az[buf_idx] = az;
	  buf_gx[buf_idx] = gx;
	  buf_gy[buf_idx] = gy;
	  buf_gz[buf_idx] = gz;
      buf_durUs[buf_idx++] = durationUs;
    }

    Serial.printf("0x%08x,%8.5e,%8.5f,%8.5f,%8.5f,%8.5f,%8.5f,%8.5f\n",
                  loopCount, durationUs * 1e-6, ax, ay, az, gx, gy, gz);

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

  stats = calculateStats(buf_ax, buf_idx);
  Serial.printf("#stats: %8s - n %8i, mean %8.5f (g), stddev %8.5f (g)\n",
                "ax", buf_idx, stats.mean, stats.stddev);

  stats = calculateStats(buf_ay, buf_idx);
  Serial.printf("#stats: %8s - n %8i, mean %8.5f (g), stddev %8.5f (g)\n",
                "ay", buf_idx, stats.mean, stats.stddev);

  stats = calculateStats(buf_az, buf_idx);
  Serial.printf("#stats: %8s - n %8i, mean %8.5f (g), stddev %8.5f (g)\n",
                "az", buf_idx, stats.mean, stats.stddev);

  stats = calculateStats(buf_gx, buf_idx);
  Serial.printf("#stats: %8s - n %8i, mean %8.5f (deg/s), stddev %8.5f (deg/s)\n",
                "gx", buf_idx, stats.mean, stats.stddev);

  stats = calculateStats(buf_gy, buf_idx);
  Serial.printf("#stats: %8s - n %8i, mean %8.5f (deg/s), stddev %8.5f (deg/s)\n",
                "gy", buf_idx, stats.mean, stats.stddev);

  stats = calculateStats(buf_gz, buf_idx);
  Serial.printf("#stats: %8s - n %8i, mean %8.5f (deg/s), stddev %8.5f (deg/s)\n",
                "gz", buf_idx, stats.mean, stats.stddev);

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
