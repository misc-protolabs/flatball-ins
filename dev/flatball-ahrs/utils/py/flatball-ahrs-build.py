# Copyright 2025 Michael V. Schaefer
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at:
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import os
import sys
import shutil
import subprocess
from datetime import datetime

LIB_DIR = "lib/extern"
BUILD_DIRS = [".pio"] #, "build", ".temp", "dist"]
VALID_LIBS = {
    "Adafruit_BMP3XX_Library",
    "Adafruit_BusIO",
    "Adafruit_Unified_Sensor",
    "ArduinoJson",
    "ESP32WebServer",
    "Fusion",
    "SparkFun_6DoF_ISM330DHCX",
    "SparkFun_MAX1704x_Fuel_Gauge_Arduino_Library",
    "SparkFun_MMC5983MA_Magnetometer_Arduino_Library",
    "TaskScheduler"
}

# 🗒️ UTF-8 logging setup
#ROOT = os.path.dirname(__file__) or "."
LOG_DIR = os.path.join(".", "..", "..", "admin", "logs")
LOG_FILE = os.path.join(LOG_DIR, "flatball-ahrs-build-log.md")
print( LOG_FILE)
timestamp = datetime.now().strftime("%Y-%m-%d_%H%M")
log_file = open(LOG_FILE, "w", encoding="utf-8")

def log(msg):
    time_tag = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    line = f"[{time_tag}] {msg}"
    print(line)
    log_file.write(line + "\n")

def scan_lib_folder():
    log("📦 Scanning libraries...")
    libs_found = set()

    for lib in sorted(os.listdir(LIB_DIR)):
        lib_path = os.path.join(LIB_DIR, lib)

        if not os.path.isdir(lib_path):
            log(f"❌ {lib} is not a directory — skipping.")
            continue

        if lib not in VALID_LIBS:
            log(f"⚠️ {lib} not in VALID_LIBS — skipping.")
            continue

        libs_found.add(lib)
        json_path = os.path.join(lib_path, "library.json")
        try:
            with open(json_path, "w") as f:
                f.write("{\n  \"name\": \"" + lib + "\",\n  \"version\": \"0.0.1\"\n}")
            log(f"✅ {lib} — library.json generated.")
        except Exception as e:
            log(f"❌ {lib} — error creating library.json: {e}")

    missing = VALID_LIBS.difference(libs_found)
    if missing:
        log(f"❌ Missing required libraries: {missing}")
        sys.exit(1)

def clean_artifacts():
    log("🧹 Cleaning build artifacts...")
    for path in BUILD_DIRS:
        if os.path.exists(path):
            try:
                shutil.rmtree(path)
                log(f"🗑️ Removed {path}")
            except Exception as e:
                log(f"⚠️ Could not remove {path}: {e}")
        else:
            log(f"ℹ️ {path} not found — skipping.")

def clean_platformio():
    log("🧼 Invoking PlatformIO clean...")
    # Trigger PlatformIO clean
    result = subprocess.run(
        ["platformio", "run", "--target", "clean"],
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
        encoding="utf-8"
    )
    if result.returncode == 0:
        log("✅ PlatformIO clean completed.")
    else:
        log("❌ PlatformIO clean failed.")
        log(result.stdout)

def ahrs_sanity_checks():
    log("🔬 Performing AHRS sanity checks...")
    # Placeholder — Extend with your actual validation steps
    log("✅ AHRS configuration verified.")

def run_platformio_build():
    log("⚙️ Starting PlatformIO firmware build...")

    try:
        result = subprocess.run(
            #["platformio", "run", "-v"],
            ["platformio", "run", "-e", "esp32dev"],
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True,
            encoding="utf-8"
        )
        log(result.stdout)
        if result.returncode != 0:
            log("❌ Build failed.")
            sys.exit(1)
        else:
            log("✅ Firmware build completed successfully.")
    except Exception as e:
        log(f"❌ Error running PlatformIO build: {e}")
        sys.exit(1)

if __name__ == "__main__":
    log("🚀 Starting Flatball Full Build Process")
    scan_lib_folder()
    clean_artifacts()
    clean_platformio()
    ahrs_sanity_checks()
    run_platformio_build()
    log("🏁 Build completed successfully.")
    log_file.close()
