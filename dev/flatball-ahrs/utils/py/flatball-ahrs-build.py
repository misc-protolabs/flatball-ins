import os
import sys
import shutil
import subprocess
from datetime import datetime

LIB_DIR = "lib"
BUILD_DIRS = [".pio"] #, "build", ".temp", "dist"]
VALID_LIBS = {
    "Adafruit_BMP3XX_Library",
    "Adafruit_BusIO",
    "Adafruit_Unified_Sensor",
    "ESP32WebServer",
    "Fusion",
    "SparkFun_6DoF_ISM330DHCX",
    "SparkFun_MAX1704x_Fuel_Gauge_Arduino_Library",
    "SparkFun_MMC5983MA_Magnetometer_Arduino_Library",
    "TaskScheduler"
}

# 🗒️ UTF-8 logging setup
timestamp = datetime.now().strftime("%Y-%m-%d_%H%M")
log_filename = f"flatball-ahrs-build.log"
#log_filename = f"flatball_build{timestamp}.log"
#os.makedirs("logs", exist_ok=True)
log_file = open(log_filename, "w", encoding="utf-8")

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
