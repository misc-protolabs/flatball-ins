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

#ifndef COMMS_H
#define COMMS_H

#include <esp_now.h>

// Structure to send data
typedef struct struct_tx_message {
    uint32_t cnt;
    float deltaTime;
    float v_batt;
    //float soc, soc_rate_of_change;
    float patm, degC;
    float ax, ay, az;
    float gx, gy, gz;
    float mx, my, mz;
    //float roll, pitch, yaw;
    //float qw, qx, qy, qz;
    //float fe_ax, fe_ay, fe_az;
    //float fl_ax, fl_ay, fl_az;
    bool boot_btn;
} struct_tx_message;

// Structure to receive data
typedef struct struct_rx_message {
    uint8_t command_id;
    uint8_t command_data[16];
    uint8_t checksum;
} struct_rx_message;

// Global variables
extern struct_tx_message tx_data;
extern struct_rx_message rx_data;
extern int8_t txPower;
extern uint32_t tx_cnt;
extern uint32_t rx_cnt;

// Function declarations
bool espnow_init(void);
//void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
//void onDataRecv(const esp_now_recv_info *esp_now_info, const uint8_t *incomingData, int len);
//void pack_tx_data(void);
void comms_step( void);
void comms_step_10Hz( void);
void comms_step_1Hz( void);

#endif // COMMS_H
