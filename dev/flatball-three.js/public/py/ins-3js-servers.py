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

import subprocess
import time
import sys

http = subprocess.Popen(["python", "-m", "http.server"])
telemetry = subprocess.Popen(["python", "./public/py/ins-telemetry.py"])

print("🚀 Both services launched. Press Ctrl+C to shut down.")
try:
    while True:
        time.sleep(1)
except KeyboardInterrupt:
    print("🛑 Shutting down...")
    http.terminate()
    telemetry.terminate()
