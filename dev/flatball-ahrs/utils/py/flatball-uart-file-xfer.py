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

import serial
import argparse
import os

def send_command(ser, cmd):
    ser.write((cmd + '\n').encode())
    ser.flush()
    print(f"📤 Sent command: {cmd}")
    output = []
    while True:
        line = ser.readline().decode(errors='ignore').strip()
        if not line:
            break
        output.append(line)
        if line == '>':  # ESP32 prompt
            break
    return output

def download_csvs(ser, folder, source):
    os.makedirs(folder, exist_ok=True)
    ser.write((f"download {source}\n").encode())
    ser.flush()
    current_file = None
    buffer = []

    while True:
        line = ser.readline().decode(errors='ignore')
        if not line:
            continue
        if line.startswith("📄 "):  # New CSV start
            if current_file and buffer:
                with open(current_file, 'w', encoding='utf-8') as f:
                    f.writelines(buffer)
                print(f"✅ Saved {current_file}")
                buffer.clear()
            filename = line.strip().split("📄 ")[1]
            filename = os.path.basename(filename)
            current_file = os.path.join(folder, filename)
        elif "--- EOF ---" in line:
            if current_file and buffer:
                with open(current_file, 'w', encoding='utf-8') as f:
                    f.writelines(buffer)
                print(f"✅ Saved {current_file}")
            buffer.clear()
            current_file = None
        else:
            buffer.append(line)

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--port', required=True, help='Serial port (e.g., COM3 or /dev/ttyUSB0)')
    parser.add_argument('--baud', type=int, default=115200)
    parser.add_argument('--cmd', choices=[
        'list logs', 'list flatball-logs',
        'download logs', 'download flatball-logs',
        'delete logs', 'delete flatball-logs'
    ])
    parser.add_argument('--save', help='Local folder to save CSVs (used only for download commands)')
    args = parser.parse_args()

    with serial.Serial(args.port, args.baud, timeout=2) as ser:
        ser.reset_input_buffer()
        if args.cmd:
            if args.cmd.startswith('download') and args.save:
                source = args.cmd.split()[1]
                download_csvs(ser, args.save, source)
            else:
                output = send_command(ser, args.cmd)
                for line in output:
                    print(line)
        else:
            print("🕹️ Enter interactive mode. Type a command:")
            while True:
                cmd = input("> ")
                if cmd in ['exit', 'quit']:
                    break
                output = send_command(ser, cmd)
                for line in output:
                    print(line)

if __name__ == "__main__":
    main()
