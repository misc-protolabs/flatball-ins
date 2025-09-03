#ifndef _VFB_H_
#define _VFB_H_
#include "Arduino.h"

// Tested with Espressif ESP32 v2.0.5 and the "ESP32 Dev Module" board definition
#define EN_3V3_SW 32 // The 3.3V_SW regulator Enable pin is connected to D32
#define BOOT_BTN 0   // DataLogger IoT – 9DoF Boot Button is connected to D0
#define STAT_LED 25  // DataLogger IoT – 9DoF STAT LED is connected to D25
#define RGB_LED 26   // DataLogger IoT – 9DoF RGB LED is connected to D26
#define IMU_CS 5
#define IMU_INT1 34
#define MAG_CS 27
#define MAG_INT 35

#define I2C_SDA 21
#define I2C_SCL 22
#define I2C_FREQ 1000000

#define SPI_FREQ 2000000 // 2 MHz SPI clock
#define SPI_MOSI 23
#define SPI_MISO 19
#define SPI_SCK 18
#define SPI_CS_ISM330DHCX 5
#define SPI_CS_MMC5983MA 27

typedef struct struct_internals
{
  uint16_t echo_flgs;
  uint16_t mag3dof_rd_fail_cnt;
  uint16_t bmp390_rd_fail_cnt;
} struct_internals;

extern bool vfb_init(void);
extern void vfb_step_100Hz(void);
extern void vfb_step_10Hz(void);
extern void vfb_step_1Hz(void);
extern void vfb_step_100Hz_core1(void);
extern void vfb_step_10Hz_core1(void);
extern void vfb_step_1Hz_core1(void);
extern void vfb_deep_sleep(void);
extern unsigned int vfb_new_log(void);

extern struct_internals internals;
extern bool boot_btn;
extern bool stat_led;
#endif
