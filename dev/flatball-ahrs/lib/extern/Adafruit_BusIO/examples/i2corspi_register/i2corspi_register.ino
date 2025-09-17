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

#include <Adafruit_BusIO_Register.h>

// Define which interface to use by setting the unused interface to NULL!

#define SPIDEVICE_CS 10
Adafruit_SPIDevice *spi_dev = NULL; // new Adafruit_SPIDevice(SPIDEVICE_CS);

#define I2C_ADDRESS 0x5D
Adafruit_I2CDevice *i2c_dev = new Adafruit_I2CDevice(I2C_ADDRESS);

void setup() {
  while (!Serial) {
    delay(10);
  }
  Serial.begin(115200);
  Serial.println("I2C or SPI device register test");

  if (spi_dev && !spi_dev->begin()) {
    Serial.println("Could not initialize SPI device");
  }

  if (i2c_dev) {
    if (i2c_dev->begin()) {
      Serial.print("Device found on I2C address 0x");
      Serial.println(i2c_dev->address(), HEX);
    } else {
      Serial.print("Did not find I2C device at 0x");
      Serial.println(i2c_dev->address(), HEX);
    }
  }

  Adafruit_BusIO_Register id_reg =
      Adafruit_BusIO_Register(i2c_dev, spi_dev, ADDRBIT8_HIGH_TOREAD, 0x0F);
  uint8_t id = 0;
  id_reg.read(&id);
  Serial.print("ID register = 0x");
  Serial.println(id, HEX);
}

void loop() {}
