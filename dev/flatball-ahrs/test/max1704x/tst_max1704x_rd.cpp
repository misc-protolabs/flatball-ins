// tst_rd_mag3dof.cpp - SPI bandwidth stress test for MMC5983MA
#include <Arduino.h>
#include <SparkFun_MAX1704x_Fuel_Gauge_Arduino_Library.h> // Click here to get the library: http://librarymanager/All#SparkFun_MAX1704x_Fuel_Gauge_Arduino_Library
#include <Wire.h>

#include "esp_sleep.h"

#include "vfb.h"

TaskHandle_t hndl_100hz;
void task_100hz(void *pvParameters);
void tst_loop( void);
constexpr uint32_t TEST_DURATION_MS = 5000;

uint32_t startTime;
uint32_t loopCount = 0;
uint32_t readCount = 0;
uint32_t readFailCount = 0;
float elapsedUs = 0.0;

#define MAX_SAMPLES 500
float buf_durUs[MAX_SAMPLES];
uint16_t buf_idx = 0;
uint16_t histo[10];  // 10 buckets: 8–17 ms

SFE_MAX1704X lipo(MAX1704X_MAX17048);
float v_batt;
float buf_v[MAX_SAMPLES];

void setup() {
	Serial.begin(115200);
	Serial.print("#test-uuid :: ");
	Serial.println( (uint32_t)( esp_random()), HEX);
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

	Serial.println("misc::vfb::I2C configuration...");
	Wire.begin( 21, 22, I2C_FREQ); // SDA, SCL, frequency
	//Serial.println("misc::vfb::I2C::f == 1e6...");
	//Wire.setClock( I2C_FREQ);

  lipo.enableDebugging(); // Uncomment this line to enable helpful debug messages on Serial

  // Set up the MAX17048 LiPo fuel gauge:
  if (lipo.begin() == false) // Connect to the MAX17048 using the default wire port
  {
    Serial.println(F("misc::MAX17408::not detected. Please check wiring. Freezing."));
    while (1)
      ;
  }

  Serial.println("misc::MAX17408::configuration...");

  // Just because we can, let's reset the MAX17048
  Serial.println(F("misc::MAX17408::Resetting the MAX17048..."));
  delay(1000); // Give it time to get its act back together

  // Read and print the reset indicator
  Serial.print(F("misc::MAX17048::Reset Indicator was: "));
  bool RI = lipo.isReset(true); // Read the RI flag and clear it automatically if it is set
  Serial.println(RI); // Print the RI
  // If RI was set, check it is now clear
  if (RI)
  {
    Serial.print(F("misc::MAX17048::Reset Indicator is now: "));
    RI = lipo.isReset(); // Read the RI flag
    Serial.println(RI); // Print the RI    
  }
  lipo.setVALRTMax( (uint8_t)( 4.3 / 0.02));
  lipo.setVALRTMin( (uint8_t)( 3.2 / 0.02));

  // To quick-start or not to quick-start? That is the question!
  // Read the following and then decide if you do want to quick-start the fuel gauge.
  // "Most systems should not use quick-start because the ICs handle most startup problems transparently,
  //  such as intermittent battery-terminal connection during insertion. If battery voltage stabilizes
  //  faster than 17ms then do not use quick-start. The quick-start command restarts fuel-gauge calculations
  //  in the same manner as initial power-up of the IC. If the system power-up sequence is so noisy that the
  //  initial estimate of SOC has unacceptable error, the system microcontroller might be able to reduce the
  //  error by using quick-start."
  // If you still want to try a quick-start then uncomment the next line:
	//lipo.quickStart();

  // Read and print the device ID
  Serial.print(F("misc::MAX17048::Device ID: 0x"));
  uint8_t id = lipo.getID(); // Read the device ID
  if (id < 0x10) Serial.print(F("0")); // Print the leading zero if required
  Serial.println(id, HEX); // Print the ID as hexadecimal

  // Read and print the device version
  Serial.print(F("misc::MAX17048::Device version: 0x"));
  uint8_t ver = lipo.getVersion(); // Read the device version
  if (ver < 0x10) Serial.print(F("0")); // Print the leading zero if required
  Serial.println(ver, HEX); // Print the version as hexadecimal

  // Read and print the battery threshold
  Serial.print(F("misc::MAX17048::Battery empty threshold is currently: "));
  Serial.print(lipo.getThreshold());
  Serial.println(F("%"));

  // Read and print the high voltage threshold
  Serial.print(F("misc::MAX17048::High voltage threshold is currently: "));
  float highVoltage = ((float)lipo.getVALRTMax()) * 0.02; // 1 LSb is 20mV. Convert to Volts.
  Serial.print(highVoltage, 2);
  Serial.println(F("V"));

  // Read and print the low voltage threshold
  Serial.print(F("misc::MAX17048::Low voltage threshold is currently: "));
  float lowVoltage = ((float)lipo.getVALRTMin()) * 0.02; // 1 LSb is 20mV. Convert to Volts.
  Serial.print(lowVoltage, 2);
  Serial.println(F("V"));

  Serial.println("misc::MAX17408::I2C.");

	Serial.println("#test-begin");
	Serial.println("idx,dt,v");

	xTaskCreate( task_100hz, "100Hz", 1024*16, NULL, 3, &hndl_100hz);
}

void task_100hz(void *pvParameters) {
  static unsigned long timestamp_z1;

  startTime = micros();

  for (;;) {
    unsigned long timestamp = micros();
    tst_loop();
    timestamp_z1 = timestamp;
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void tst_loop( void) {
  bool rd_ok;
  uint32_t t0, t1;

  loopCount++;  

  t0 = micros();
  v_batt = lipo.getVoltage();
  rd_ok = true;
  t1 = micros();

  float durationUs = (t1 - t0);

  if (rd_ok) {
	if( buf_idx < MAX_SAMPLES) {
		buf_v[buf_idx] = v_batt;
		buf_durUs[buf_idx++] = durationUs;
	}

	Serial.printf( "0x%08x,%8.5e,%8.5f\n", loopCount, durationUs*1e-6, v_batt);

	readCount++;
  } else {
	readFailCount++;
  }
}

struct StatsResult {
  float mean;
  float stddev;
};

StatsResult calculateStats(float* data, size_t count) {
  float sum = 0.0f;
  for (size_t i = 0; i < count; i++) {
    sum += data[i];
  }
  float mean = sum / count;

  float variance = 0.0f;
  for (size_t i = 0; i < count; i++) {
    float diff = data[i] - mean;
    variance += diff * diff;
  }

  StatsResult result;
  result.mean = mean;
  result.stddev = sqrt(variance / count);
  return result;
}

void tst_end( void) {
	StatsResult stats;

	stats = calculateStats(buf_v, buf_idx);
	Serial.printf( "#stats: %8s - n %8i, mean %8.5f (V), stddev %8.5f (V)\n",
		"v", buf_idx, stats.mean, stats.stddev);

	stats = calculateStats(buf_durUs, buf_idx);
	Serial.printf( "#stats: %8s - n %8i, mean %8.5f (us), stddev %8.5f (us)\n",
		"dt", buf_idx, stats.mean, stats.stddev);

	Serial.flush();
	Serial.println("#test-end");
}

void loop( void) {
	static uint32_t startMsTime = millis();
	uint32_t now = millis();
	if( (now - startMsTime) >= TEST_DURATION_MS) {
		vTaskSuspend(hndl_100hz);
		tst_end();
	}
}
