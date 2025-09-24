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

#include "Arduino.h"
#include "SPI.h"
#include "Wire.h"
#include "driver/rtc_io.h"
// #include "Freenove_WS2812_Lib_for_ESP32.h"
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h> // Include header for esp_wifi_set_max_tx_power

#include "vfb.h"
#include "comms.h"
#include "app.h"
#include "cal.h"

#include "imu.h"
#include "mag.h"
#include "lipo.h"
#include "baro.h"
#include "util.h"
#include "sd_log.h"
#include "srvr.h"

struct_internals internals;
bool boot_btn;
bool stat_led;
// Freenove_ESP32_WS2812 strip = Freenove_ESP32_WS2812(1, RGB_LED, 0, TYPE_RGB);

// Function declarations
void vfb_print_wakeup_reason();
void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);

bool vfb_init()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("...esp32...\n");
  Serial.println();

  Serial.println();
  Serial.println("...misc...\n");
  Serial.println();

  Serial.println("misc::vfb initialization...");
  vfb_print_wakeup_reason();

  Serial.println("misc::esp32 configuration...");

  // espnow_init();

  Serial.println("misc::pin configuration...");
  pinMode(EN_3V3_SW, OUTPUT);
  digitalWrite(EN_3V3_SW, HIGH);
  pinMode(BOOT_BTN, INPUT_PULLUP);
  pinMode(STAT_LED, OUTPUT);
  pinMode(IMU_CS, OUTPUT);
  digitalWrite(IMU_CS, HIGH);
  pinMode(IMU_INT1, INPUT);
  pinMode(MAG_CS, OUTPUT);
  digitalWrite(MAG_CS, HIGH);
  pinMode(MAG_INT, INPUT);
  delay(100);

  // Serial.println("RBG-led configuration...");
  // strip.begin();
  // strip.setBrightness(255);

  Serial.println("misc::vfb::peripherhal configuration...");

  Serial.println("misc::vfb::SDMMC configuration...");
  sd_log_init();

  Serial.println("misc::vfb::SPI configuration...");
  SPI.setFrequency(SPI_FREQ);
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI, IMU_CS);
  imu_init();
  mag_init();

  Serial.println("misc::vfb::I2C configuration...");
  Wire.begin();
  Wire.setClock(I2C_FREQ);

  lipo_init();
  baro_init();

  Serial.printf("misc::vfb::esp32::Free PSRAM: %d bytes\n", ESP.getFreePsram());
  Serial.printf("misc::vfb::esp32::Chip revision: %d\n", ESP.getChipRevision());
  Serial.printf("misc::vfb::esp32::Chip model: %s\n", ESP.getChipModel());
  Serial.printf("misc::vfb::esp32::Free heap: %d bytes\n", ESP.getFreeHeap());
  Serial.printf("misc::vfb::esp32::Free stack: %d bytes\n", uxTaskGetStackHighWaterMark(NULL));
  Serial.println("misc::vfb::esp32 configured.");

  digitalWrite(STAT_LED, LOW);

  Serial.println("misc::vfb::initialized.");
  delay(100);
  return true;
}

void vfb_step_100Hz()
{
  imu_step(&ax_raw, &ay_raw, &az_raw, &gx_raw, &gy_raw, &gz_raw);
  // get_imu_temp(&degC); // FIXME: move to inside imu_step()

  mag_step(&mx_raw, &my_raw, &mz_raw);
  // baro_step( &patm, &degC); // CPU utilization is too high, move to 10Hz

  app_step_100Hz();
}

void vfb_step_100Hz_core1(void)
{
  if (fdev_st == 2)
  {
    stat_led = !stat_led;
  }
  digitalWrite(STAT_LED, stat_led);
  // uint8_t r, g, b;
  // uint8_t brightness = 128;
  // r = (fdev_st == 0) * brightness;
  // g = (fdev_st == 1) * brightness;
  // b = (fdev_st == 2) * brightness;
  // strip.setLedColorData(0, g, r, b);
  // strip.show();

  comms_step();
  sd_log_write(); //(0); //(fdev_flight_st == LAUNCH);
  srvr_step();
}

void vfb_step_10Hz()
{
  v_batt = lipo_v();
  // soc = lipo_soc(); // unnecessary - voltage is sufficient
  baro_step(&patm, &degC);
}

void vfb_step_10Hz_core1()
{
  if (!sd_log.srvr_active)
  {
    comms_step_10Hz();
  }
  if (fdev_st == 1)
  {
    stat_led = !stat_led;
  }
}

void vfb_step_1Hz()
{
  // baro_step( &patm, &degC);
}

void vfb_step_1Hz_core1()
{
  static bool boot_btn_z1 = 0;
  // static float boot_btn_tmr = 0.0;
  // static bool boot_btn_x_z1 = false;
  // static bool boot_btn_y_z1 = false;

  boot_btn = !digitalRead(BOOT_BTN);
  // boot_btn = edge_dbnc_rise(boot_btn, 0.25, &boot_btn_tmr, &boot_btn_x_z1, &boot_btn_y_z1);

  // fdev_st {0 --> idle, 1 --> srvr, 2 --> logging}
  if (boot_btn && !boot_btn_z1)
  {
    if (fdev_st == 0)
    {
      fdev_st = 2;
    }
    else if (fdev_st == 1)
    {
      fdev_st = 2;
    }
    else if (fdev_st == 2)
    {
      fdev_st = 1;
    }
  }
  comms_step_1Hz();

  static float acc_filt_z1, gz_filt_z1;

  float acc = sqrt(ax * ax + ay * ay + az * az);
  float acc_filt = filt_1ord(acc, acc_filt_z1, 1.5, 1.0);
  float gz_filt = filt_1ord(gz, gz_filt_z1, 1.5, 1.0);

  acc_filt_z1 = acc_filt;
  gz_filt_z1 = gz_filt;

  if (fdev_st == 0)
  {
    stat_led = !stat_led;
  }

  if (fdev_st == 0) // idle state
  {
    if (fdev_st_z1 != 0)
    {
      if (fdev_st_z1 == 2)
      {
        sd_log_close();
      }
      if (fdev_st_z1 == 1)
      {
        srvr_stop();
      }
    }
  }
  else if (fdev_st == 1) // server state
  {
    if (fdev_st_z1 != 1)
    {
      if (fdev_st_z1 == 2)
      {
        sd_log_close();
      }
      srvr_init();
    }
  }
  else if (fdev_st == 2) // logging state
  {
    if (fdev_st_z1 != 2)
    {
      if (fdev_st_z1 == 1)
      {
        srvr_stop();
      }
      unsigned int n_log = sd_log_new();
    }
  }

  if (fdev_st == 0)
  {
    static short sleep_dly = k_sleep_dly;
    bool acc_ok = acc_filt <= k_acc_filt_thr;
    bool gyro_ok = abs(gz_filt) <= k_gz_filt_thr;
    bool srvr_ok = !sd_log.srvr_active;
    if (acc_ok && gyro_ok && srvr_ok)
    {
      if (sleep_dly-- <= 0)
      {
        sleep_dly = k_sleep_dly;
        vfb_deep_sleep();
      }
    }
    else
    {
      sleep_dly = k_sleep_dly;
    }
  }

  fdev_st_z1 = fdev_st;
  boot_btn_z1 = boot_btn;
}

void vfb_print_wakeup_reason()
{
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
  switch (wakeup_reason)
  {
  case ESP_SLEEP_WAKEUP_EXT0:
    Serial.println("misc::vfb::Wakeup caused by external signal using RTC_IO");
    break;
  case ESP_SLEEP_WAKEUP_EXT1:
    Serial.println("misc::vfb::Wakeup caused by external signal using RTC_CNTL");
    break;
  case ESP_SLEEP_WAKEUP_TIMER:
    Serial.println("misc::vfb::Wakeup caused by timer");
    break;
  case ESP_SLEEP_WAKEUP_TOUCHPAD:
    Serial.println("misc::vfb::Wakeup caused by touchpad");
    break;
  case ESP_SLEEP_WAKEUP_ULP:
    Serial.println("misc::vfb::Wakeup caused by ULP program");
    break;
  default:
    Serial.printf("misc::vfb::Wakeup was not caused by deep sleep: %d\n", wakeup_reason);
    break;
  }
}

void vfb_deep_sleep()
{
  Serial.println();
  Serial.println("...misc...\n");
  Serial.println();

  WiFi.disconnect();
  SPI.end();
  Wire.end();

  sd_log_close();
  Serial.flush();

  Serial.println("misc::vfb::shutdown...\n");

  Serial.println("misc::vfb::sleeping...\n");

  Serial.println();
  Serial.println("...esp32...\n");
  Serial.println();
  Serial.flush();
  Serial.end();

  digitalWrite(EN_3V3_SW, LOW);
  digitalWrite(STAT_LED, LOW);
  /*
  // Put the ESP32 to sleep
  esp_sleep_enable_ext0_wakeup( (gpio_num_t)( BOOT_BTN), 0); // Wake up on boot button press
  // Configure pullup/downs via RTCIO to tie wakeup pins to inactive level during deepsleep.
  rtc_gpio_pullup_dis((gpio_num_t)(BOOT_BTN));
  rtc_gpio_pulldown_en((gpio_num_t)(BOOT_BTN));
  */
  esp_deep_sleep_start();
}
