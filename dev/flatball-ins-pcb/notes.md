<!--
<!-- Copyright 2025 Michael V. Schaefer
<!-- 
<!-- Licensed under the Apache License, Version 2.0 (the "License");
<!-- you may not use this file except in compliance with the License.
<!-- You may obtain a copy of the License at:
<!-- 
<!--     http://www.apache.org/licenses/LICENSE-2.0
<!-- 
<!-- Unless required by applicable law or agreed to in writing, software
<!-- distributed under the License is distributed on an "AS IS" BASIS,
<!-- WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
<!-- See the License for the specific language governing permissions and
<!-- limitations under the License.
-->

# Flatball-INS PCB: Sensor + LED Integration Summary

## Magnetometer Selection

### Candidates Considered
- **LIS3MDL (ST)**
  - Status: Obsolete / discontinued
  - SPI support: Yes (mode 0)
  - Not recommended for new designs

- **IIS2MDC / LIS2MDL (ST)**
  - Status: Active, recommended replacement for LIS3MDL
  - Interface: I²C and SPI (mode 0 supported)
  - Max ODR: 150 Hz
  - Power: ~100–500 µA depending on ODR
  - Cost: ~$2.50–$3.50 in volume
  - Availability: Strong (Digi-Key, Mouser, Amazon, Adafruit)

- **MLX90393 (Melexis)**
  - Interface: I²C and SPI (mode 3 only)
  - Max ODR: ~1 kHz
  - Power: ~1–3 mA
  - Resolution: ~0.15 µT
  - Caveat: SPI mode mismatch (mode 3 vs mode 0 for other sensors)

- **BMM150 (Bosch)**
  - Interface: I²C and SPI (mode 0 supported)
  - Max ODR: ~100 Hz
  - Resolution: ~0.3 µT
  - Status: Active, but no higher-spec successors in Bosch lineup

- **MMC5983MA (Memsic)**
  - Interface: I²C only
  - Resolution: ~40 nT (very high)
  - Range: ±8 gauss (narrow)
  - Power: ~20 µA
  - Status: Active, but limited sourcing

- **RM3100 (PNI)**
  - Interface: I²C and SPI (mode 0 supported)
  - ODR: >100 Hz
  - Power: ~1–2 mA
  - Accuracy: Exceptional (magneto-inductive, aerospace-grade)
  - Cost: Higher, niche sourcing

### Decision
- **Chosen part:** **IIS2MDC**  
  - Meets requirements: SPI mode 0, ≥100 Hz ODR, low power, good accuracy, strong availability, low cost.
- **Dropped part:** **MMC5983MA**  
  - Excellent resolution but I²C-only, narrow range, and sourcing friction.

---

## Barometric Pressure / Temperature Sensor

### Candidates Considered
- **BMP581 (Bosch)**
  - Interface: I²C / SPI
  - Max ODR: up to 1 kHz
  - Resolution: ~6 cm altitude
  - Power: ~700 µA @ 100 Hz
  - Cost: ~$6–7 each in singles, ~$3.50–4.00 in volume
  - Status: Bosch’s newest baro, strong availability

- **BMP390 (Bosch)**
  - Interface: I²C / SPI
  - Max ODR: 200 Hz
  - Resolution: ~25 cm altitude
  - Power: ~3.2 µA @ 1 Hz
  - Cost: ~$2.50–3.50 in volume
  - Status: Active, widely used

- **LPS22HB / LPS25HB (ST)**
  - Interface: I²C / SPI
  - Max ODR: 75 Hz
  - Resolution: ~8 cm altitude
  - Power: ~12 µA @ 1 Hz
  - Status: Active, ultra-low power

- **MS5611 / MS5637 (TE/MEAS)**
  - Interface: I²C / SPI
  - Max ODR: ~50–100 Hz
  - Resolution: ~10 cm altitude
  - Power: ~1 mA
  - Status: Older, still available

### Decision
- **Chosen part:** **BMP581**  
  - Matches high ODR requirement (up to 1 kHz) for correlation with 120 fps video and IMU fusion.  
  - Modern, supported, and future-proof.  
  - Acceptable cost in volume (~$3.50–4.00).  
- **Alternative:** BMP390 if lower ODR (≤200 Hz) is acceptable and cost/power are prioritized.

### PCB Placement Notes
- Place near PCB edge with vent hole; mark with **“VENT”** silkscreen.
- Keep away from hot LEDs/regulators to avoid thermal drift.
- Optional PTFE membrane over vent for dust/moisture protection.

---

## LED Integration for Video Analysis

### Requirements
- Camera: iPhone 12 Pro, 120 fps slo-mo
- Trace duration: ~2 seconds (~240 frames)
- Goals: Stable exposure, visible time markers, reproducible analysis

### LED Count and Placement
- **4 white fill LEDs** at 90° intervals around 65 mm PCB rim
- **1 marker LED** (blue or IR) near 0° reference, slightly inward
- **Silkscreen features:**
  - 0° arrow and fiducial at marker LED
  - Tick marks every 30° around rim
  - Labels A–D for fill LEDs
  - Center crosshair + 7 mm square for alignment
  - Optional radial contrast bars for calibration

### Electrical / Driver Notes
- **Fill LEDs:** Constant-current driver, PWM ≥ 2 kHz, brightness 30–60%
- **Marker LED:** Separate MOSFET driver, 0.5–1.0 ms strobe pulses
- **Grounding:** Star/split returns; keep LED loops away from magnetometer
- **Thermal:** Copper pour + vias under LEDs

### Firmware / Config Parameters
- **mode:** off | continuous | strobe | hybrid
- **brightness:** 0–1.0
- **pwm_freq_hz:** ≥2000
- **strobe_width_ms:** 0.5–1.0
- **strobe_times_ms:** [0, 500, 1000, 1500, 2000]
- **start_code_ms:** [0, 20, 40] (unique start burst)
- **end_code_ms:** [1960, 1980] (unique end burst)

### Recommended Operating Mode
- **Hybrid:**  
  - Continuous fill from 4 rim LEDs for exposure stability  
  - Marker LED strobes at fixed times with start/end codes for alignment  

---

## Key Takeaways
- **Magnetometer:** IIS2MDC is the pragmatic, long-term choice (SPI mode 0, 150 Hz, low power, low cost).  
- **Baro/Temp:** BMP581 is the best fit for high ODR and future-proofing; BMP390 is a lower-cost fallback.  
- **LEDs:** 4 + 1 configuration on 65 mm circular PCB provides uniform fill and robust video markers.  
- **Firmware:** Config-driven, reproducible, and fully loggable for auditability.  
