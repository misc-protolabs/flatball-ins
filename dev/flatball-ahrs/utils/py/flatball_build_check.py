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
import subprocess
from datetime import datetime

LIB_DIR = "lib"
SRC_DIR = "src"
LOG_FILE = f"build_log_{datetime.now().strftime('%Y%m%d_%H%M%S')}.txt"

# Step 1: Sanity check on lib folder
def scan_lib_folder():
    print("\n📦 Scanning local libraries...\n")
    for lib in sorted(os.listdir(LIB_DIR)):
        lib_path = os.path.join(LIB_DIR, lib)
        src_path = os.path.join(lib_path, "src")
        json_path = os.path.join(lib_path, "library.json")

        print(f"🔍 {lib}")
        if os.path.isdir(src_path):
            headers = [f for f in os.listdir(src_path) if f.endswith(".h")]
            print(f"  ✅ Found src/ with {len(headers)} headers")
        else:
            print("  ❌ Missing src/ folder")

        if os.path.exists(json_path):
            print("  📝 library.json present")
        else:
            print("  ⚠️ No library.json")

# Step 2: Clean project
def clean_project():
    print("\n🧹 Cleaning previous build artifacts...\n")
    subprocess.run(["pio", "run", "-t", "clean"])

# Step 3: Build with verbose output, save to log
def build_verbose():
    print(f"\n🚧 Running verbose build... Output will be saved to {LOG_FILE}\n")
    with open(LOG_FILE, "w") as log:
        subprocess.run(["pio", "run", "-v"], stdout=log, stderr=subprocess.STDOUT)
    print(f"\n✅ Build complete. Check {LOG_FILE} for full output.\n")

if __name__ == "__main__":
    print("🚀 Flatball IMU Build + Sanity Check\n")
    scan_lib_folder()
    clean_project()
    build_verbose()