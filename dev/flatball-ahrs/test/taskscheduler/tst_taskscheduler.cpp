#include <Arduino.h>
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

	Serial.println("#test-begin");
	Serial.println("idx,dt");

	xTaskCreate( task_100hz, "100Hz", 1024*16, NULL, 3, &hndl_100hz);
}

void task_100hz(void *pvParameters) {
  static unsigned long timestamp_z1;

  startTime = micros();
  timestamp_z1 = startTime;

  for (;;) {
    unsigned long timestamp = micros();
	float durationUs = ( timestamp - timestamp_z1);

	loopCount++;
	if( buf_idx < MAX_SAMPLES) {
		buf_durUs[ buf_idx++] = durationUs;
	}
	Serial.printf( "0x%08x,%8.5e\n",
		loopCount, durationUs*1e-6);

    timestamp_z1 = timestamp;
    vTaskDelay(pdMS_TO_TICKS(10));
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

	StatsResult stats = calculateStats(buf_durUs, buf_idx);
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
