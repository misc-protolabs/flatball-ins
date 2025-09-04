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
import shutil
import subprocess
import serial
import time

# Paths
this_dir = os.path.dirname(os.path.abspath(__file__))
project_root = os.path.abspath(os.path.join(this_dir, "..", ".."))
src_orig = os.path.join(project_root, "src")
src_bkp = os.path.join(project_root, "src_bkp")
src_temp = src_orig  # reuse original path for flashing

# Serial logging parameters
serial_port = "COM3"   # Update to match your ESP32 port
baud_rate = 115200
log_file = os.path.join(this_dir, "tst_mmc5983ma_rd.log")

# Step 1: Backup existing src/
if os.path.exists(src_orig):
    print("🧳 Backing up original src/...")
    if os.path.exists(src_bkp):
        shutil.rmtree(src_bkp)
    shutil.move(src_orig, src_bkp)

# Step 2: Create fresh src/
print("📦 Creating fresh src/ with all test sources...")
os.makedirs(src_temp, exist_ok=True)

# Copy all .c/.cpp/.h files from test folder into src/
for fname in os.listdir(this_dir):
    if fname.endswith((".c", ".cpp", ".h")):
        src_path = os.path.join(this_dir, fname)
        dst_path = os.path.join(src_temp, fname)
        shutil.copy(src_path, dst_path)
        print(f"  ✅ Copied {fname}")

# Step 3: Build and upload
print("🚀 Uploading test firmware via PlatformIO...")
env = "esp32dev"
max_retries = 3
retry_delay = 5  # seconds

for attempt in range(1, max_retries + 1):
    print(f"🚀 Attempt {attempt} to upload firmware (env: {env})...")
    result = subprocess.run(
        ["pio", "run", "-e", env, "-t", "upload"],
        cwd=project_root,
        capture_output=True,
        text=True
    )

    if result.returncode == 0:
        print("✅ Upload succeeded.")
        break
    else:
        print("⚠️ Upload failed. Reason:")
        print(result.stderr.strip())

        if "could not open port" in result.stderr.lower() or "access is denied" in result.stderr.lower():
            print(f"🔄 Retrying in {retry_delay} seconds...")
            time.sleep(retry_delay)
        else:
            print("🛑 Non-retryable error. Aborting.")
            break


# Step 4: Restore original src/
print("♻️ Cleaning up and restoring original src/...")
shutil.rmtree(src_temp)
if os.path.exists(src_bkp):
    shutil.move(src_bkp, src_orig)
    print("✅ Original src/ restored.")

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

print("🎉 Test complete.")
