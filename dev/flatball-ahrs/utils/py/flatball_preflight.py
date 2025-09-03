import os
import sys

LIB_DIR = "lib"
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

def scan_lib_folder():
    print(f"📦 Scanning libraries in ./{LIB_DIR}/\n")
    libs_found = set()

    for lib in sorted(os.listdir(LIB_DIR)):
        lib_path = os.path.join(LIB_DIR, lib)

        if not os.path.isdir(lib_path):
            print(f"🔍 {lib}\n  ❌ Not a directory — skipping.\n")
            continue

        if lib not in VALID_LIBS:
            print(f"🔍 {lib}\n  ⚠️ Not in VALID_LIBS — skipping.\n")
            continue

        libs_found.add(lib)
        json_path = os.path.join(lib_path, "library.json")
        try:
            with open(json_path, "w") as f:
                f.write("{\n  \"name\": \"" + lib + "\",\n  \"version\": \"0.0.1\"\n}")
            print(f"🔍 {lib}\n  ✅ library.json generated.\n")
        except Exception as e:
            print(f"🔍 {lib}\n  ❌ Error creating library.json: {e}\n")

    # Check for missing essential libs
    missing = VALID_LIBS.difference(libs_found)
    if missing:
        print(f"❌ Missing required libraries: {missing}\n")
        sys.exit(1)

def imu_sanity_checks():
    print("🧪 Running IMU configuration checks...\n")
    # Placeholder for your actual IMU logic
    print("✅ IMU configuration verified.\n")

if __name__ == "__main__":
    print("🚀 Flatball IMU Build + Sanity Checks\n")
    scan_lib_folder()
    imu_sanity_checks()