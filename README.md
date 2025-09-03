# Flatball Inertial Navigation System: Advanced Frisbee Flight Dynamics & Aerodynamics Research

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![GitHub Workflow Status](https://img.shields.io/github/actions/workflow/status/misc-protolabs/flatball-ins/.github/workflows/main.yml?branch=main)](https://github.com/misc-protolabs/flatball-ins/actions?query=workflow%3A%22Firmware+Build%22)
[![Open Issues](https://img.shields.io/github/issues/misc-protolabs/flatball-ins.svg)](https://github.com/misc-protolabs/flatball-ins/issues)

Inertial navigation system for analyzing flight dynamics and aerodynamic behavior of flatball (frisbee) trajectories. Designed for research-grade IMU deployments and real-time pose estimation.

---

## ✨ Project Overview

This project aims to revolutionize frisbee flight analysis through a sophisticated, instrumented disc platform. Our goal is to provide precision data for aerodynamics research, performance analysis, and biomechanical feedback, ultimately contributing to a journal/conference article.

* **Core Goal:** Instrument frisbee flight for precision aerodynamics research, performance analysis, and biomechanical feedback.
* **Disc Platform:** A modified regulation frisbee meticulously instrumented with sensor suite, logging, wifi, on/off, battery, LEDs.
* **Sensor Suite:** Integrated 9-DoF IMU (accelerometer, gyroscope, magnetometer) and a BMP390 barometric pressure/temperature sensor, all managed by an onboard microcontroller. Optional GPS module support is planned.
* **Launch System:** A custom-designed cradle launcher adapted from baseball/softball cannon technology for consistent and controlled disc launches.
* **Primary Output:** Co-authored journal and/or conference articles based on research findings.

---

## 🛠️ Hardware Specifications

The custom-designed PCB and mechanical components are central to our data acquisition capabilities.

| Component             | Description                                    |
| :-------------------- | :--------------------------------------------- |
| **Microcontroller** | ESP32-WROOOM (Primary)                         |
| **Power** | 3.7V LiPo cell with JST connector              |
| **IMU (6-DoF)** | ISM330DHCX (Gyroscope + Accelerometer)         |
| **Magnetometer** | MMC5983MA (3-DoF)                              |
| **Ambient Sensors** | BMP390 (Barometric Pressure + Temperature)     |
| **PCB Version** | `vx.y` (Currently under development, custom design) |
| **Cradle Design** | Modular TPU with integrated damping inserts    |

Detailed PCB fabrication files, schematics, and the Bill of Materials (BOM) can be found in the [`hardware/`](./hardware) directory.

---

## 💾 Firmware Features

Our FreeRTOS-based firmware is designed for robust real-time data acquisition and management.

* **Real-time OS:** FreeRTOS scheduler operating at 100Hz (sensors), 10Hz, and 1Hz.
* **Communication:** SPI, I2C, and SD-MMC interfaces for peripheral communication and data storage.
* **Sensor Integration:**
    * MMC5983MA 3-DoF Magnetometer
    * ISM330DHCX 6-DoF Gyroscope + Accelerometer IMU
    * BMP390 Barometric Pressure and Temperature Sensor
* **Data Handling:** Real-time data logging directly to SD-MMC card.
* **Connectivity:** File management and retrieval via an onboard HTTP server; Wi-Fi and Bluetooth capabilities.
* **Power Management:** Deep sleep modes for extended battery life.
* **Calibration:** Comprehensive sensor and frisbee calibration routines for data accuracy.

Explore the source code and build instructions within the [`firmware/`](./firmware) directory.

---

## 🧪 Test Protocols & Data Handling

Our standardized test protocols ensure consistent and comparable data collection.

| Test Type         | Initial Conditions                      | Metrics Captured             |
| :---------------- | :-------------------------------------- | :--------------------------- |
| **Calibration** | Manual                                  | 9-DoF, Pressure, Temperature |
|                   |                                         |                              |
| **Forehand (CW)** | $u = 15 \, \text{m/s}$, $r = -50 \, \text{rad/s}$ | 9-DoF, Pressure, Temperature |
| **Backhand (CCW)**| $u = 15 \, \text{m/s}$, $r = +50 \, \text{rad/s}$ | 9-DoF, Pressure, Temperature |
|                   |                                         |                              |

All captured data is output in CSV format with automatic timestamping for easy analysis. Analysis scripts and tools are located in the [`analysis/`](./analysis) directory.

---

## 🚀 Quick Start Guide

Get up and running with the Flatball Aero SSL project in a few simple steps.

1.  **Clone the Repository:**
    ```bash
    git clone [https://github.com/misc-protolabs/flatball-ins.git](https://github.com/misc-protolabs/flatball-ins.git)
    cd flatball-ins
    ```
2.  **Flash Firmware:**
    ```bash
    make flash TARGET=esp32
    ```
    *(Note: Changed `nrf52840` to `esp32` based on Hardware Specs)*
3.  **Start Data Logging:**
    ```bash
    python scripts/logger.py --port /dev/ttyUSB0 # Adjust port as needed
    ```

For detailed setup instructions, development environment configuration, and troubleshooting, please refer to our [Wiki](https://github.com/misc-protolabs/flatball-ins/wiki).

---

## 🤝 Contributing

We welcome contributions to the Flatball Aero SSL project! Whether you're interested in hardware design, firmware development, data analysis, or documentation, your input is valuable.

* **Reporting Issues:** If you find a bug or have a feature request, please open an issue [here](https://github.com/misc-protolabs/flatball-ins/issues).
* **Submitting Pull Requests:** For code contributions, please follow our [contribution guidelines](CONTRIBUTING.md) (create this file!) and submit a pull request.

Check our [Project Board](https://github.com/misc-protolabs/flatball-ins/projects) for current tasks and priorities.

---

## 👥 Contributors

This project is a collaborative effort, made possible by the dedication of the following individuals and organizations:

| <a href="https://github.com/misc-protolabs"><img src="https://github.com/misc-protolabs/flatball-ins/blob/main/admin/mike-schaefer/avatar.jpg" width="100px;" alt="Mike Schaefer"/><br /><sub><b>Mike Schaefer</b></sub></a><br />💻 📐 🔧 |
| :-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------: | :---------------------------------------------------------------------------------------------------------------------------------------------------------------------------: | :-----------------------------------------------------------------------------------------------------------------------------------------------------------------------: |

---

## ⚖️ License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
