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

import logging
import sys
import os
import asyncio
import serial
import serial.tools.list_ports
import json
import time
import websockets

# === LOGGING SETUP ===
LOG_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'logs')
LOG_FILE = os.path.join(LOG_DIR, 'telemetry-server.log')
os.makedirs(LOG_DIR, exist_ok=True)

logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s - %(message)s',
    datefmt='%Y-%m-%d %H:%M:%S',
    handlers=[logging.FileHandler(LOG_FILE, mode='w', encoding='utf-8')],
)

class LoggerWriter:
    def __init__(self, level):
        self.level = level

    def write(self, message):
        if message.strip() != '':
            self.level(message.strip())

    def flush(self):
        pass

sys.stdout = LoggerWriter(logging.info)
sys.stderr = LoggerWriter(logging.error)

# === CONFIG ===
PREFERRED_SERIAL_PORT = 'COM3' # Will try this port first if available
BAUD_RATE = 115200
VERBOSE = False
METRICS_INTERVAL = 1.0
HEARTBEAT_THRESHOLD = 2.0
WEBSOCKET_PORT = 8080

# === STATE ===
serial_messages_received = 0
serial_decode_failures = 0
last_serial_time = time.time()
mps_counter = 0
packet_queue = asyncio.Queue()

# === SERIAL SETUP ===
def find_and_open_port():
    """Scans for serial ports and tries to connect."""
    ports = serial.tools.list_ports.comports()
    print("Searching for hardware...")

    if not ports:
        print("  [!] No serial ports found.")
        return None

    # Prioritize the preferred port
    sorted_ports = sorted(ports, key=lambda p: p.device == PREFERRED_SERIAL_PORT, reverse=True)

    for port_info in sorted_ports:
        port = port_info.device
        print(f"  [*] Attempting to connect to {port}...")
        try:
            ser = serial.Serial(port, BAUD_RATE, timeout=1)
            ser.reset_input_buffer()
            # A simple handshake could be added here to verify it's the correct device
            print(f"  [+] Successfully connected on {port}")
            return ser
        except (OSError, serial.SerialException) as e:
            print(f"    [✗] Failed to connect: {e}")
            continue
    
    print("  [!] Could not connect to any serial port.")
    return None

ser = find_and_open_port()

# === SERIAL READER ===
async def serial_reader():
    global ser, serial_messages_received, serial_decode_failures, last_serial_time, mps_counter
    print("[Serial] Reader started")
    while True:
        try:
            while True:
                await asyncio.sleep(0)  # Yield to other tasks
                raw = await asyncio.to_thread(ser.readline)
                now = time.time()

                if not raw:
                    continue

                last_serial_time = now
                try:
                    line = raw.decode('utf-8', errors='replace').strip()
                    if VERBOSE:
                        print(f"[Raw Serial] '{line}'")

                    if not line or not line.startswith('{'):
                        serial_decode_failures += 1
                        continue

                    data = json.loads(line)
                    serial_messages_received += 1
                    mps_counter += 1
                    await packet_queue.put(data)

                    if VERBOSE:
                        print(f"[Serial] #{serial_messages_received} | {line}")
                except Exception as e:
                    serial_decode_failures += 1
                    print(f"JSON decode error: {e}")
        except serial.SerialException as e:
            print(f"[Serial] Hardware disconnected: {e}")
            ser.close()
            print("[Serial] Attempting to reconnect...")
            while True:
                await asyncio.sleep(5)
                new_ser = find_and_open_port()
                if new_ser:
                    ser = new_ser
                    print("[Serial] Reconnected successfully!")
                    break

# === METRICS REPORTER ===
async def metrics_reporter():
    global mps_counter, serial_messages_received, serial_decode_failures, last_serial_time
    print("[Metrics] Reporter started")
    while True:
        now = time.time()
        silence = now - last_serial_time
        mps = mps_counter
        mps_counter = 0

        print(
            f"[Metrics] MPS: {mps:<3} | Total: {serial_messages_received:<5} | "
            f"Failures: {serial_decode_failures:<3} | Silence: {silence:5.2f}s"
        )

        if silence > HEARTBEAT_THRESHOLD:
            print(f"[Metrics] No serial data for {round(silence, 2)}s")

        await asyncio.sleep(METRICS_INTERVAL)

# === HOTPLUG SCANNER ===
async def hotplug_scanner():
    global ser
    print("[Hotplug] Scanner started. Will check for new hardware every 3 seconds.")
    while True:
        await asyncio.sleep(3)
        # Check if the serial object is still None (i.e., not connected)
        if not ser:
            new_ser = find_and_open_port()
            if new_ser:
                ser = new_ser
                print("[Hotplug] Detected new hardware. Launching serial-dependent tasks.")
                # Launch the tasks that normally start when hardware is present
                asyncio.create_task(metrics_reporter())
                await asyncio.sleep(2)
                asyncio.create_task(serial_reader())
                break # Exit the hotplug scanner loop, its job is done
    print("[Hotplug] Scanner finished.")

# === WEBSOCKET HANDLER ===
async def telemetry_handler(websocket):
    print(f"[WS] Client connected: {websocket.remote_address}")
    try:
        while True:
            data = await packet_queue.get()
            await websocket.send(json.dumps(data))
    except websockets.exceptions.ConnectionClosed:
        print(f"[WS] Client disconnected: {websocket.remote_address}")

# === MAIN ===
async def main():
    print("Starting telemetry service...")

    # Start WebSocket server
    server = await websockets.serve(telemetry_handler, "localhost", WEBSOCKET_PORT)
    print(f"WebSocket server is running on ws://localhost:{WEBSOCKET_PORT}")

    if ser:
        # Launch tasks that depend on the serial connection
        print("Hardware found at startup. Launching serial-dependent tasks.")
        asyncio.create_task(metrics_reporter())
        await asyncio.sleep(2) # Give metrics reporter a head start
        asyncio.create_task(serial_reader())
    else:
        # If no hardware, launch the hotplug scanner
        print("No hardware found at startup. Launching hotplug scanner.")
        asyncio.create_task(hotplug_scanner())

    # Keep the loop alive
    await server.wait_closed()

if __name__ == "__main__":
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        print(">> Interrupted by user")
