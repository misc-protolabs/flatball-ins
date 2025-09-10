import asyncio
import serial
import json
import websockets
import time

# === CONFIG ===
SERIAL_PORT = 'COM3'
BAUD_RATE = 115200
WEBSOCKET_PORT = 8080

# === SETUP SERIAL ===
try:
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=2)
    print(f"✅ Serial connected on {SERIAL_PORT} at {BAUD_RATE} baud")
except Exception as e:
    print(f"💥 Failed to open serial port {SERIAL_PORT}: {e}")
    exit(1)

# === WEBSOCKET HANDLER ===
async def telemetry_handler(websocket):
    print(f"🌐 WebSocket client connected from {websocket.remote_address}")
    while True:
        raw = ser.readline()
        if not raw:
            continue

        try:
            line = raw.decode('utf-8', errors='replace').strip()
            #print(f"[Serial] {line}")
            data = json.loads(line)
            await websocket.send(json.dumps(data))
        except Exception as e:
            print(f"💥 Error: {e}")

# === START SERVER ===
async def main():
    print(f"🚀 Starting WebSocket server on ws://localhost:{WEBSOCKET_PORT}")
    server = await websockets.serve(telemetry_handler, "localhost", WEBSOCKET_PORT)
    print("✅ WebSocket server is running")
    await asyncio.Future()  # run forever

if __name__ == "__main__":
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        print("🛑 Server interrupted by user")
