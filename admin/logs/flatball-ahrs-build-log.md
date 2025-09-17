[2025-09-17 09:46:25] 🚀 Starting Flatball Full Build Process
[2025-09-17 09:46:25] 📦 Scanning libraries...
[2025-09-17 09:46:25] ✅ Adafruit_BMP3XX_Library — library.json generated.
[2025-09-17 09:46:25] ✅ Adafruit_BusIO — library.json generated.
[2025-09-17 09:46:25] ✅ Adafruit_Unified_Sensor — library.json generated.
[2025-09-17 09:46:25] ✅ ArduinoJson — library.json generated.
[2025-09-17 09:46:25] ✅ ESP32WebServer — library.json generated.
[2025-09-17 09:46:25] ✅ Fusion — library.json generated.
[2025-09-17 09:46:25] ✅ SparkFun_6DoF_ISM330DHCX — library.json generated.
[2025-09-17 09:46:25] ✅ SparkFun_MAX1704x_Fuel_Gauge_Arduino_Library — library.json generated.
[2025-09-17 09:46:25] ✅ SparkFun_MMC5983MA_Magnetometer_Arduino_Library — library.json generated.
[2025-09-17 09:46:25] ✅ TaskScheduler — library.json generated.
[2025-09-17 09:46:25] 🧹 Cleaning build artifacts...
[2025-09-17 09:46:25] 🗑️ Removed .pio
[2025-09-17 09:46:25] 🧼 Invoking PlatformIO clean...
[2025-09-17 09:46:34] ✅ PlatformIO clean completed.
[2025-09-17 09:46:34] 🔬 Performing AHRS sanity checks...
[2025-09-17 09:46:34] ✅ AHRS configuration verified.
[2025-09-17 09:46:34] ⚙️ Starting PlatformIO firmware build...
[2025-09-17 09:48:12] Processing esp32dev (platform: espressif32; board: esp32dev; framework: arduino)
--------------------------------------------------------------------------------
Verbose mode can be enabled via `-v, --verbose` option
CONFIGURATION: https://docs.platformio.org/page/boards/espressif32/esp32dev.html
PLATFORM: Espressif 32 (6.5.0) > Espressif ESP32 Dev Module
HARDWARE: ESP32 240MHz, 320KB RAM, 4MB Flash
DEBUG: Current (cmsis-dap) External (cmsis-dap, esp-bridge, esp-prog, iot-bus-jtag, jlink, minimodule, olimex-arm-usb-ocd, olimex-arm-usb-ocd-h, olimex-arm-usb-tiny-h, olimex-jtag-tiny, tumpa)
PACKAGES: 
 - framework-arduinoespressif32 @ 3.20014.231204 (2.0.14) 
 - tool-esptoolpy @ 1.40501.0 (4.5.1) 
 - toolchain-xtensa-esp32 @ 8.4.0+2021r2-patch5
LDF: Library Dependency Finder -> https://bit.ly/configure-pio-ldf
LDF Modes: Finder ~ chain+, Compatibility ~ off
Found 44 compatible libraries
Scanning dependencies...
Dependency Graph
|-- Fusion @ 0.0.1
|-- Wire @ 2.0.0
|-- Adafruit_Unified_Sensor @ 0.0.1
|-- Adafruit_BMP3XX_Library @ 0.0.1
|-- Adafruit_BusIO @ 0.0.1
|-- SPI @ 2.0.0
|-- WiFi @ 2.0.0
|-- ArduinoJson @ 0.0.1
|-- SparkFun_6DoF_ISM330DHCX @ 0.0.1
|-- SparkFun_MAX1704x_Fuel_Gauge_Arduino_Library @ 0.0.1
|-- SparkFun_MMC5983MA_Magnetometer_Arduino_Library @ 0.0.1
|-- FS @ 2.0.0
|-- SD_MMC @ 2.0.0
|-- ESP32WebServer @ 0.0.1
|-- ESPmDNS @ 2.0.0
Building in release mode
Compiling .pio\build\esp32dev\src\app.cpp.o
Compiling .pio\build\esp32dev\src\baro.cpp.o
Compiling .pio\build\esp32dev\src\cal.cpp.o
Compiling .pio\build\esp32dev\src\comms.cpp.o
Compiling .pio\build\esp32dev\src\imu.cpp.o
Compiling .pio\build\esp32dev\src\lipo.cpp.o
Compiling .pio\build\esp32dev\src\mag.cpp.o
Compiling .pio\build\esp32dev\src\main.cpp.o
Compiling .pio\build\esp32dev\src\profiler.cpp.o
Compiling .pio\build\esp32dev\src\sd_log.cpp.o
Compiling .pio\build\esp32dev\src\srvr.cpp.o
Compiling .pio\build\esp32dev\src\twister.cpp.o
Compiling .pio\build\esp32dev\src\util.cpp.o
Compiling .pio\build\esp32dev\src\vfb.cpp.o
Building .pio\build\esp32dev\bootloader.bin
Generating partitions .pio\build\esp32dev\partitions.bin
Compiling .pio\build\esp32dev\libdf7\Fusion\FusionAhrs.c.o
Compiling .pio\build\esp32dev\libdf7\Fusion\FusionCompass.c.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
esptool.py v4.5.1
Creating esp32 image...
Merged 1 ELF section
Successfully created esp32 image.
Compiling .pio\build\esp32dev\libdf7\Fusion\FusionOffset.c.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
src/comms.cpp: In function 'void comms_step_10Hz()':
src/comms.cpp:239:5: warning: 'template<unsigned int N> class ArduinoJson::V742PB22::StaticJsonDocument' is deprecated: use JsonDocument instead [-Wdeprecated-declarations]
     StaticJsonDocument<128> doc;
     ^~~~~~~~~~~~~~~~~~
In file included from lib/extern/ArduinoJson/src/ArduinoJson.hpp:65,
                 from lib/extern/ArduinoJson/src/ArduinoJson.h:9,
                 from src/comms.cpp:20:
lib/extern/ArduinoJson/src/ArduinoJson/compatibility.hpp:63:58: note: declared here
 class ARDUINOJSON_DEPRECATED("use JsonDocument instead") StaticJsonDocument
                                                          ^~~~~~~~~~~~~~~~~~
Compiling .pio\build\esp32dev\libc17\Wire\Wire.cpp.o
Compiling .pio\build\esp32dev\lib1ff\Adafruit_Unified_Sensor\Adafruit_Sensor.cpp.o
Archiving .pio\build\esp32dev\libdf7\libFusion.a
src/srvr.cpp: In function 'void srvr_init()':
src/srvr.cpp:103:22: warning: NULL used in arithmetic [-Wpointer-arith]
   if (buf_ssid[0] != NULL && buf_pass[0] != NULL)
                      ^~~~
src/srvr.cpp:103:45: warning: NULL used in arithmetic [-Wpointer-arith]
   if (buf_ssid[0] != NULL && buf_pass[0] != NULL)
                                             ^~~~
Compiling .pio\build\esp32dev\lib01d\SPI\SPI.cpp.o
Compiling .pio\build\esp32dev\lib331\Adafruit_BusIO\Adafruit_BusIO_Register.cpp.o
Compiling .pio\build\esp32dev\lib331\Adafruit_BusIO\Adafruit_GenericDevice.cpp.o
Compiling .pio\build\esp32dev\lib331\Adafruit_BusIO\Adafruit_I2CDevice.cpp.o
Compiling .pio\build\esp32dev\lib331\Adafruit_BusIO\Adafruit_SPIDevice.cpp.o
Compiling .pio\build\esp32dev\lib895\Adafruit_BMP3XX_Library\Adafruit_BMP3XX.cpp.o
Archiving .pio\build\esp32dev\libc17\libWire.a
Archiving .pio\build\esp32dev\lib1ff\libAdafruit_Unified_Sensor.a
Compiling .pio\build\esp32dev\lib895\Adafruit_BMP3XX_Library\bmp3.c.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pio\build\esp32dev\lib29e\WiFi\WiFi.cpp.o
Archiving .pio\build\esp32dev\lib01d\libSPI.a
Compiling .pio\build\esp32dev\lib29e\WiFi\WiFiAP.cpp.o
Compiling .pio\build\esp32dev\lib29e\WiFi\WiFiClient.cpp.o
Compiling .pio\build\esp32dev\lib29e\WiFi\WiFiGeneric.cpp.o
Compiling .pio\build\esp32dev\lib29e\WiFi\WiFiMulti.cpp.o
Compiling .pio\build\esp32dev\lib29e\WiFi\WiFiSTA.cpp.o
Archiving .pio\build\esp32dev\lib331\libAdafruit_BusIO.a
Compiling .pio\build\esp32dev\lib29e\WiFi\WiFiScan.cpp.o
lib/extern/Adafruit_BMP3XX_Library/Adafruit_BMP3XX.cpp: In member function 'bool Adafruit_BMP3XX::performConfig(uint8_t)':
lib/extern/Adafruit_BMP3XX_Library/Adafruit_BMP3XX.cpp:370:1: warning: control reaches end of non-void function [-Wreturn-type]
 }
 ^
Archiving .pio\build\esp32dev\lib895\libAdafruit_BMP3XX_Library.a
Compiling .pio\build\esp32dev\lib29e\WiFi\WiFiServer.cpp.o
Compiling .pio\build\esp32dev\lib29e\WiFi\WiFiUdp.cpp.o
Compiling .pio\build\esp32dev\lib731\SparkFun_6DoF_ISM330DHCX\sfe_bus.cpp.o
Compiling .pio\build\esp32dev\lib731\SparkFun_6DoF_ISM330DHCX\sfe_ism330dhcx.cpp.o
Compiling .pio\build\esp32dev\lib731\SparkFun_6DoF_ISM330DHCX\sfe_ism_shim.cpp.o
Compiling .pio\build\esp32dev\lib731\SparkFun_6DoF_ISM330DHCX\st_src\ism330dhcx_reg.c.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pio\build\esp32dev\lib3ba\SparkFun_MAX1704x_Fuel_Gauge_Arduino_Library\SparkFun_MAX1704x_Fuel_Gauge_Arduino_Library.cpp.o
Compiling .pio\build\esp32dev\lib1e0\SparkFun_MMC5983MA_Magnetometer_Arduino_Library\SparkFun_MMC5983MA_Arduino_Library.cpp.o
Compiling .pio\build\esp32dev\lib1e0\SparkFun_MMC5983MA_Magnetometer_Arduino_Library\SparkFun_MMC5983MA_IO.cpp.o
Compiling .pio\build\esp32dev\libeb8\FS\FS.cpp.o
Archiving .pio\build\esp32dev\lib29e\libWiFi.a
Compiling .pio\build\esp32dev\libeb8\FS\vfs_api.cpp.o
Compiling .pio\build\esp32dev\lib387\SD_MMC\SD_MMC.cpp.o
Compiling .pio\build\esp32dev\liba8f\ESP32WebServer\ESP32WebServer.cpp.o
Archiving .pio\build\esp32dev\lib731\libSparkFun_6DoF_ISM330DHCX.a
Compiling .pio\build\esp32dev\liba8f\ESP32WebServer\Parsing.cpp.o
Archiving .pio\build\esp32dev\lib3ba\libSparkFun_MAX1704x_Fuel_Gauge_Arduino_Library.a
Compiling .pio\build\esp32dev\libb28\ESPmDNS\ESPmDNS.cpp.o
Compiling .pio\build\esp32dev\FrameworkArduino\Esp.cpp.o
Archiving .pio\build\esp32dev\lib1e0\libSparkFun_MMC5983MA_Magnetometer_Arduino_Library.a
Compiling .pio\build\esp32dev\FrameworkArduino\FirmwareMSC.cpp.o
Compiling .pio\build\esp32dev\FrameworkArduino\FunctionalInterrupt.cpp.o
Compiling .pio\build\esp32dev\FrameworkArduino\HWCDC.cpp.o
Archiving .pio\build\esp32dev\libeb8\libFS.a
Compiling .pio\build\esp32dev\FrameworkArduino\HardwareSerial.cpp.o
Compiling .pio\build\esp32dev\FrameworkArduino\IPAddress.cpp.o
Archiving .pio\build\esp32dev\lib387\libSD_MMC.a
Compiling .pio\build\esp32dev\FrameworkArduino\IPv6Address.cpp.o
Compiling .pio\build\esp32dev\FrameworkArduino\MD5Builder.cpp.o
Archiving .pio\build\esp32dev\liba8f\libESP32WebServer.a
Compiling .pio\build\esp32dev\FrameworkArduino\Print.cpp.o
Archiving .pio\build\esp32dev\libb28\libESPmDNS.a
Compiling .pio\build\esp32dev\FrameworkArduino\Stream.cpp.o
Compiling .pio\build\esp32dev\FrameworkArduino\StreamString.cpp.o
Compiling .pio\build\esp32dev\FrameworkArduino\Tone.cpp.o
Compiling .pio\build\esp32dev\FrameworkArduino\USB.cpp.o
Compiling .pio\build\esp32dev\FrameworkArduino\USBCDC.cpp.o
Compiling .pio\build\esp32dev\FrameworkArduino\USBMSC.cpp.o
Compiling .pio\build\esp32dev\FrameworkArduino\WMath.cpp.o
Compiling .pio\build\esp32dev\FrameworkArduino\WString.cpp.o
Compiling .pio\build\esp32dev\FrameworkArduino\base64.cpp.o
Compiling .pio\build\esp32dev\FrameworkArduino\cbuf.cpp.o
Compiling .pio\build\esp32dev\FrameworkArduino\esp32-hal-adc.c.o
Compiling .pio\build\esp32dev\FrameworkArduino\esp32-hal-bt.c.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pio\build\esp32dev\FrameworkArduino\esp32-hal-cpu.c.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pio\build\esp32dev\FrameworkArduino\esp32-hal-dac.c.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pio\build\esp32dev\FrameworkArduino\esp32-hal-gpio.c.o
Compiling .pio\build\esp32dev\FrameworkArduino\esp32-hal-i2c-slave.c.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pio\build\esp32dev\FrameworkArduino\esp32-hal-i2c.c.o
Compiling .pio\build\esp32dev\FrameworkArduino\esp32-hal-ledc.c.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pio\build\esp32dev\FrameworkArduino\esp32-hal-matrix.c.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pio\build\esp32dev\FrameworkArduino\esp32-hal-misc.c.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pio\build\esp32dev\FrameworkArduino\esp32-hal-psram.c.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pio\build\esp32dev\FrameworkArduino\esp32-hal-rgb-led.c.o
Compiling .pio\build\esp32dev\FrameworkArduino\esp32-hal-rmt.c.o
Compiling .pio\build\esp32dev\FrameworkArduino\esp32-hal-sigmadelta.c.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pio\build\esp32dev\FrameworkArduino\esp32-hal-spi.c.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pio\build\esp32dev\FrameworkArduino\esp32-hal-time.c.o
Compiling .pio\build\esp32dev\FrameworkArduino\esp32-hal-timer.c.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pio\build\esp32dev\FrameworkArduino\esp32-hal-tinyusb.c.o
Compiling .pio\build\esp32dev\FrameworkArduino\esp32-hal-touch.c.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pio\build\esp32dev\FrameworkArduino\esp32-hal-uart.c.o
Compiling .pio\build\esp32dev\FrameworkArduino\firmware_msc_fat.c.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pio\build\esp32dev\FrameworkArduino\libb64\cdecode.c.o
Compiling .pio\build\esp32dev\FrameworkArduino\libb64\cencode.c.o
Compiling .pio\build\esp32dev\FrameworkArduino\main.cpp.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pio\build\esp32dev\FrameworkArduino\stdlib_noniso.c.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pio\build\esp32dev\FrameworkArduino\wiring_pulse.c.o
Compiling .pio\build\esp32dev\FrameworkArduino\wiring_shift.c.o
C:/Users/schae/.platformio/packages/framework-arduinoespressif32/cores/esp32/esp32-hal-uart.c: In function 'uartSetPins':
C:/Users/schae/.platformio/packages/framework-arduinoespressif32/cores/esp32/esp32-hal-uart.c:153:9: warning: 'return' with no value, in function returning non-void
         return;
         ^~~~~~
C:/Users/schae/.platformio/packages/framework-arduinoespressif32/cores/esp32/esp32-hal-uart.c:149:6: note: declared here
 bool uartSetPins(uint8_t uart_num, int8_t rxPin, int8_t txPin, int8_t ctsPin, int8_t rtsPin)
      ^~~~~~~~~~~
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Archiving .pio\build\esp32dev\libFrameworkArduino.a
Linking .pio\build\esp32dev\firmware.elf
Retrieving maximum program size .pio\build\esp32dev\firmware.elf
Checking size .pio\build\esp32dev\firmware.elf
Advanced Memory Usage is available via "PlatformIO Home > Project Inspect"
RAM:   [==        ]  17.4% (used 57128 bytes from 327680 bytes)
Flash: [=======   ]  72.0% (used 944325 bytes from 1310720 bytes)
Building .pio\build\esp32dev\firmware.bin
esptool.py v4.5.1
Creating esp32 image...
Merged 26 ELF sections
Successfully created esp32 image.
========================= [SUCCESS] Took 96.14 seconds =========================

Environment    Status    Duration
-------------  --------  ------------
esp32dev       SUCCESS   00:01:36.140
========================= 1 succeeded in 00:01:36.140 =========================

[2025-09-17 09:48:12] ✅ Firmware build completed successfully.
[2025-09-17 09:48:12] 🏁 Build completed successfully.
