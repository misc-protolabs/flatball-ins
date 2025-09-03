#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"
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

Adafruit_BMP3XX baro;
float p, t;
float buf_p[MAX_SAMPLES];
float buf_t[MAX_SAMPLES];

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
	Wire.begin( I2C_SDA, I2C_SCL, I2C_FREQ); // SDA, SCL, frequency
	//Serial.println("misc::vfb::I2C::f == 1e6...");
	//Wire.setClock( I2C_FREQ);

	Serial.println("misc::BMP390::configuration...");
	if (!baro.begin_I2C( 0x77, &Wire, I2C_FREQ)) {   // hardware I2C mode, can pass in address & alt Wire
	//if (! baro.begin_SPI(baro_CS)) {  // hardware SPI mode  
	//if (! baro.begin_SPI(baro_CS, baro_SCK, baro_MISO, baro_MOSI)) {  // software SPI mode
	Serial.println("misc::BMP390::Could not find a valid baro3 sensor, check wiring!");
	//while (1);
	} else {
	// Set up oversampling and filter initialization
	baro.setPressureOversampling( BMP3_OVERSAMPLING_16X);
	baro.setTemperatureOversampling( BMP3_OVERSAMPLING_2X);
	baro.setIIRFilterCoeff( BMP3_IIR_FILTER_COEFF_7);
	baro.setOutputDataRate( BMP3_ODR_100_HZ);
		baro.performConfig( BMP3_MODE_NORMAL);
	}
	Serial.println( "misc::BMP390::I2C.");

	Serial.println("#test-begin");
	Serial.println("idx,dt,p,t");

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
  // NOTE: readPandT --> this is a custom function to optimize i2c bandwidth - mvs
	rd_ok = baro.readPandT( &p, &t);
  t1 = micros();

  float durationUs = (t1 - t0);

  if (rd_ok) {
	if( buf_idx < MAX_SAMPLES) {
		buf_p[buf_idx] = p;
		buf_t[buf_idx] = t;
		buf_durUs[buf_idx++] = durationUs;
	}

	Serial.printf( "0x%08x,%8.5e,%8.5f,%8.5f\n", loopCount, durationUs*1e-6, p, t);

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

	stats = calculateStats(buf_p, buf_idx);
	Serial.printf( "#stats: %8s - n %8i, mean %8.5f (Pa), stddev %8.5f (Pa)\n",
		"p", buf_idx, stats.mean, stats.stddev);

	stats = calculateStats(buf_t, buf_idx);
	Serial.printf( "#stats: %8s - n %8i, mean %8.5f (C), stddev %8.5f (C)\n",
		"t", buf_idx, stats.mean, stats.stddev);

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
