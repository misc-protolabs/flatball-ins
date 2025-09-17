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

#ifndef ADAFRUIT_GENERICDEVICE_H
#define ADAFRUIT_GENERICDEVICE_H

#include <Arduino.h>

typedef bool (*busio_genericdevice_read_t)(void *obj, uint8_t *buffer,
                                           size_t len);
typedef bool (*busio_genericdevice_write_t)(void *obj, const uint8_t *buffer,
                                            size_t len);
typedef bool (*busio_genericdevice_readreg_t)(void *obj, uint8_t *addr_buf,
                                              uint8_t addrsiz, uint8_t *data,
                                              uint16_t datalen);
typedef bool (*busio_genericdevice_writereg_t)(void *obj, uint8_t *addr_buf,
                                               uint8_t addrsiz,
                                               const uint8_t *data,
                                               uint16_t datalen);

/*!
 * @brief Class for communicating with a device via generic read/write functions
 */
class Adafruit_GenericDevice {
public:
  Adafruit_GenericDevice(
      void *obj, busio_genericdevice_read_t read_func,
      busio_genericdevice_write_t write_func,
      busio_genericdevice_readreg_t readreg_func = nullptr,
      busio_genericdevice_writereg_t writereg_func = nullptr);

  bool begin(void);
  void end(void);

  bool read(uint8_t *buffer, size_t len);
  bool write(const uint8_t *buffer, size_t len);
  bool readRegister(uint8_t *addr_buf, uint8_t addrsiz, uint8_t *buf,
                    uint16_t bufsiz);
  bool writeRegister(uint8_t *addr_buf, uint8_t addrsiz, const uint8_t *buf,
                     uint16_t bufsiz);

protected:
  /*! @brief Function pointer for reading raw data from the device */
  busio_genericdevice_read_t _read_func;
  /*! @brief Function pointer for writing raw data to the device */
  busio_genericdevice_write_t _write_func;
  /*! @brief Function pointer for reading a 'register' from the device */
  busio_genericdevice_readreg_t _readreg_func;
  /*! @brief Function pointer for writing a 'register' to the device */
  busio_genericdevice_writereg_t _writereg_func;

  bool _begun; ///< whether we have initialized yet (in case the function needs
               ///< to do something)

private:
  void *_obj; ///< Pointer to object instance
};

#endif // ADAFRUIT_GENERICDEVICE_H
