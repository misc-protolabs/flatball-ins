import os
import shutil
import subprocess
import serial
import time

# Paths
this_dir = os.path.dirname(os.path.abspath(__file__))
project_root = os.path.abspath(os.path.join(this_dir, "..", ".."))

# Serial logging parameters
serial_port = "COM3"   # Update to match your ESP32 port
baud_rate = 115200
log_file = os.path.join(project_root, "flatball-ahrs-profiler.log")

# Step 3: Build and upload
print("🚀 Uploading test firmware via PlatformIO...")
#subprocess.run(["pio", "run", "-t", "upload"], cwd=project_root)
subprocess.run(["pio", "run", "-e", "profiler", "-t", "upload"], cwd=project_root)

# Step 5: Pipe serial output to file
test_id = None
timestamp = None

print(f"📡 Capturing serial output to {log_file}...")
with serial.Serial(serial_port, baud_rate, timeout=1) as ser, open(log_file, "w", encoding="utf-8") as log:
    while True:
        raw = ser.readline()
        line = raw.decode(errors='ignore').strip()

        if line:
            log.write(line + "\n")
            print(".", end="", flush=True)

            if line.startswith("#test-uuid:"):
                test_id = line.split(":", 1)[1].strip()
            elif line.startswith("#test-time:"):
                timestamp = line.split(":", 1)[1].strip()
            elif line.startswith("#test-end"):
                print(f"\n✅ Done! ID: {test_id} | Timestamp: {timestamp}")
                break
            elif line.startswith("misc::vfb::sleeping..."):
                print(f"\n✅ Done! ID: {test_id} | Timestamp: {timestamp}")
                break

print("🎉 Test complete.")