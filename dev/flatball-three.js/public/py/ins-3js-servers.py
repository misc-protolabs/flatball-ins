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
