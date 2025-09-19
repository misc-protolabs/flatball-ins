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
import webbrowser
import os
import argparse

# --- Logging ---
def log(message):
    timestamp = time.strftime("[%Y-%m-%d %H:%M:%S]")
    print(f"{timestamp} {message}", flush=True)

# --- Argument Parsing ---
parser = argparse.ArgumentParser(description="Launch the Flatball 3D Ultimate Scene.")
parser.add_argument('--port', type=str, help='The COM port to use for telemetry data.')
args = parser.parse_args()

# --- Launch Services ---
webbrowser.open("http://localhost:8000");

# Launch the custom server, passing the current script's PID to it.
http = subprocess.Popen(["python", "-u", "./public/py/server.py", str(os.getpid())],
                        stdout=sys.stdout, stderr=sys.stderr)

# Prepare the command for the telemetry service
telemetry_cmd = ["python", "-u", "./public/py/ins-telemetry.py"]
if args.port:
    telemetry_cmd.extend(["--port", args.port])

telemetry = subprocess.Popen(telemetry_cmd,
                             stdout=sys.stdout, stderr=sys.stderr)

log("[INFO] Both services launched. Press Ctrl+C to shut down.")
try:
    while True:
        time.sleep(1)
except KeyboardInterrupt:
    log("[INFO] Shutting down...")
    http.terminate()
    telemetry.terminate()
