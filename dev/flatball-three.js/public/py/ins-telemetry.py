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

import asyncio
import serial
import json
import time
import websockets

# === CONFIG ===
SERIAL_PORT = 'COM3'
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

# === SETUP SERIAL ===
try:
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=2)
    ser.reset_input_buffer()
    print(f"✅ Serial connected on {SERIAL_PORT} at {BAUD_RATE} baud")
except Exception as e:
    print(f"💥 Failed to open serial port {SERIAL_PORT}: {e}")
    exit(1)

# === SERIAL READER ===
async def serial_reader():
    global serial_messages_received, serial_decode_failures, last_serial_time, mps_counter
    print("[Serial] Reader started")
    while True:
        await asyncio.sleep(0)  # Yield to other tasks
        raw = await asyncio.to_thread(ser.readline)
        now = time.time()

        if raw:
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
                print(f"💥 JSON decode error: {e}")

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
            f"[Metrics] 📊 MPS: {mps:<3} | Total: {serial_messages_received:<5} | "
            f"Failures: {serial_decode_failures:<3} | Silence: {silence:5.2f}s"
        )

        if silence > HEARTBEAT_THRESHOLD:
            print(f"[Metrics] ⚠️ No serial data for {round(silence, 2)}s")

        await asyncio.sleep(METRICS_INTERVAL)

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
    print("🚀 Starting telemetry debug loop")

    # Start WebSocket server
    server = await websockets.serve(telemetry_handler, "localhost", WEBSOCKET_PORT)
    print(f"✅ WebSocket server is running on ws://localhost:{WEBSOCKET_PORT}")

    # Launch tasks
    asyncio.create_task(metrics_reporter())
    await asyncio.sleep(2)
    asyncio.create_task(serial_reader())

    # Keep the loop alive
    await server.wait_closed()

if __name__ == "__main__":
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        print("🛑 Interrupted by user")
