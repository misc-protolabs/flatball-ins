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
