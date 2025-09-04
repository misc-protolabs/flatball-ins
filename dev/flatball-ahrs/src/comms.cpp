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

#include "comms.h"
#include "Arduino.h"
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h> // Include header for esp_wifi_set_max_tx_power

#include "app.h"
#include "vfb.h"
#include "profiler.h"
#include "cal.h"

#include "lipo.h"
#include "baro.h"
#include "util.h"

// Global variables
bool espnow_ok = false;
uint8_t remoteMACAddress[6] = {0xe8, 0x6b, 0xea, 0xfd, 0x73, 0x44}; // misc::WIFI MAC Address: E8:6B:EA:FD:73:44
uint8_t baseMACAddress[6] = {0xA0, 0xB7, 0x65, 0x5E, 0x4E, 0x90};

struct_tx_message tx_data;
struct_rx_message rx_data;
int8_t txPower = 78; // Maximum power
uint32_t tx_cnt;
uint32_t rx_cnt;
bool newDataRx = true;

void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  tx_cnt++;
}

// void onDataRecv(const esp_now_recv_info *esp_now_info, const uint8_t *incomingData, int len) {
void onDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
{
  // Handle incoming data
  memcpy(&rx_data, incomingData, sizeof(rx_data));
  newDataRx = true;
  rx_cnt++;
}

bool espnow_init(void)
{
  espnow_ok = false;

  Serial.print("misc::ESP-NOW init...\n");
  // ESP-NOW Initialization
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  if (esp_now_init() != ESP_OK)
  {
    return espnow_ok;
  }

  // Set ESP-NOW transmit power level
  esp_wifi_set_max_tx_power(txPower);

  // Register the send callback
  esp_now_register_send_cb(onDataSent);

  // Register the receive callback
  esp_now_register_recv_cb(onDataRecv);

  // Print MAC address
  delay(250);
  Serial.print("misc::WIFI MAC Address: ");
  Serial.println(WiFi.macAddress());
  // uint8_t mac[6];
  // WiFi.macAddress(mac);
  // Serial.printf("MAC address: %02X:%02X:%02X:%02X:%02X:%02X\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

  // Add peer
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, baseMACAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    return espnow_ok;
  }

  tx_cnt = 0;
  rx_cnt = 0;

  Serial.print("misc::ESP-NOW initialized.\n");
  espnow_ok = true;
  return espnow_ok;
}

void parse_rx_data(void)
{
  uint8_t calculated_checksum = 0;
  calculated_checksum ^= rx_data.command_id;
  for (int i = 0; i < sizeof(rx_data.command_data); i++)
  {
    calculated_checksum ^= rx_data.command_data[i];
  }

  if (calculated_checksum == rx_data.checksum)
  {
    if (rx_data.command_id == 0x7f)
    {
      fdev_st = (int)(rx_data.command_data[0]);
      Serial.println("fdev_st overridden...");
    }
  }
  else
  {
    Serial.println("Checksum mismatch, command ignored");
  }
}

void pack_tx_data()
{
  float dt = 0.01; // 100Hz
  tx_data.cnt = tx_cnt;
  tx_data.deltaTime = dt;
  tx_data.v_batt = v_batt;
  tx_data.patm = patm;
  tx_data.degC = degC;
  tx_data.ax = ax;
  tx_data.ay = ay;
  tx_data.az = az;
  tx_data.gx = gx;
  tx_data.gy = gy;
  tx_data.gz = gz;
  tx_data.mx = mx;
  tx_data.my = my;
  tx_data.mz = mz;
  tx_data.boot_btn = boot_btn;
}

void comms_step(void)
{
  // Send data via ESP-NOW
  if (espnow_ok)
  {
    // Pack data into the structure
    pack_tx_data();

    // Calculate the size of the packed data
    size_t dataSize = sizeof(tx_data);

    esp_err_t result = esp_now_send(baseMACAddress, (uint8_t *)&tx_data, dataSize);
    if (result != ESP_OK)
    {
      // Handle the error
    }
  }
}

void comms_step_10Hz(void)
{
  if (Serial.available() > 0)
  {
    int b = Serial.read();
    if (b == '0')
    {
      fdev_st = 0;
      Serial.println("fdev_st --> 0");
    }
    if (b == '1')
    {
      fdev_st = 1;
      Serial.println("fdev_st --> 1");
    }
    if (b == '2')
    {
      fdev_st = 2;
      Serial.println("fdev_st --> 2");
    }
  }

  internals.echo_flgs = 0x0000;
  if (fdev_st == 0)
  {
#ifdef FLATBALL_AHRS_PROFILER
    internals.echo_flgs = 0x1001;
#else
    internals.echo_flgs = 0x0002;
#endif
  }
  else if (fdev_st == 1)
  {
    // Serial.println("Free heap: " + String(ESP.getFreeHeap()));
  }
  else if (fdev_st == 2)
  {
    static int8_t dly_cr = 64;
    Serial.printf(".");
    if (dly_cr-- <= 0)
    {
      Serial.printf("\n");
      dly_cr = 64;
    }
  }

  if (internals.echo_flgs == 0x0001)
  {
    Serial.printf("%3i,", fdev_st);
    Serial.printf("[% 6.2f, % 6.2f, % 6.2f] (g),", ax, ay, az);
    Serial.printf("[% 7.2f, % 7.2f, % 7.2f] (deg/sec),", gx, gy, gz);
    Serial.printf("[% 7.1f, % 7.1f, % 7.1f] (uT),", mx, my, mz);
    Serial.printf("%5.3f (v),", v_batt);
    Serial.printf("%8.1f (Pa), %4.1f (C),", patm, degC);
    Serial.printf("%2i,%2i,", boot_btn, stat_led);
    // Serial.printf("[%5.2f, %5.2f]%%",
    //               internals.core0_idle_rat_buf[0] * 100.0, internals.core1_idle_rat_buf[0] * 100.0);
    Serial.printf("\n");
    // Serial.printf("%3i : [%5.3f, %5.3f : %5.3f, %5.3f : %5.3f, %5.3f] :: ",
    //               fdev_st, internals.core0_100Hz_dt_buf[0], internals.core1_100Hz_dt_buf[0],
    //               // fdev_st, internals.core0_100Hz_dt, internals.core1_100Hz_dt, // --> #wtf - reporting 0.0
    //               internals.core0_10Hz_dt_buf[0], internals.core1_10Hz_dt_buf[0],
    //               internals.core0_1Hz_dt_buf[0], internals.core1_1Hz_dt_buf[0]);
    // Serial.printf(" [%5.2f, %5.2f]%% [%5.2f, %5.2f]%% ::",
    //              internals.core0_cpu_usage_buf[0], internals.core0_idle_rat_buf[0] * 100.0,
    //              internals.core1_cpu_usage_buf[0], internals.core1_idle_rat_buf[0] * 100.0);
    // Serial.printf(" ::: x%04x,x%04x", internals.mag3dof_rd_fail_cnt, internals.bmp390_rd_fail_cnt);
  }
  else if (internals.echo_flgs == 0x0002)
  {
    //Serial.printf("%3i,", fdev_st);
    //Serial.printf("[%8.3f, %8.3f, %8.3f, %8.3f],", qw, qx, qy, qz);
    Serial.printf("[% 6.2f, % 6.2f, % 6.2f],", ax, ay, az);
    Serial.printf("[% 7.2f, % 7.2f, % 7.2f],", gx, gy, gz);
    Serial.printf("[% 7.1f, % 7.1f, % 7.1f],", mx, my, mz);
    Serial.printf("[%7.2f, %7.2f, %7.2f],", pitch, roll, yaw);
    Serial.printf("[%5.2f, %5.2f, %u],", ahrs_accel_err, ahrs_accel_recovery_trig, (ahrs_flgs >> 4) & 0x01);
    Serial.printf("[%5.2f, %5.2f, %u],", ahrs_mag_err, ahrs_mag_recovery_trig, (ahrs_flgs >> 5) & 0x01);
    Serial.printf("[%1u %1u %1u %1u],", ahrs_flgs & 0x01, ahrs_flgs & 0x02, ahrs_flgs & 0x03, ahrs_flgs & 0x04);
    //Serial.printf("%5.3f (v),", v_batt);
    //Serial.printf("%8.1f (Pa), %4.1f (C),", patm, degC);
    //Serial.printf("%2i,%2i,", boot_btn, stat_led);
    // Serial.printf("[%5.2f, %5.2f]%%",
    //               internals.core0_idle_rat_buf[0] * 100.0, internals.core1_idle_rat_buf[0] * 100.0);
    Serial.printf("\n");
  }
  else if (internals.echo_flgs == 0x1001)
  {
    Profiler_printStats();
    Serial.printf("\n");
  }

  internals.echo_flgs = 0x0000;
}

void comms_step_1Hz(void)
{
  if (newDataRx)
  {
    parse_rx_data();
    // parseCommand(rx_data);
    newDataRx = false;
  }
}
