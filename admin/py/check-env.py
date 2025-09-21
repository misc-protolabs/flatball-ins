import json
import subprocess
import sys
import os

def check_toolchain():
    """Reads toolchain.json and checks for the presence of each tool."""
    script_dir = os.path.dirname(os.path.abspath(__file__))
    json_path = os.path.join(script_dir, '..', 'json', 'toolchain.json')
    
    try:
        with open(json_path, 'r') as f:
            tools = json.load(f)
    except FileNotFoundError:
        print(f"Error: {json_path} not found.")
        sys.exit(1)

    print("Checking development toolchain...")
    all_tools_found = True

    for tool in tools:
        name = tool['name']
        command = tool['command']
        try:
            # Use shell=True for simplicity, as commands are defined internally.
            result = subprocess.run(command, shell=True, check=True, capture_output=True, text=True)
            # Try to get a clean, single-line version
            version_info = result.stdout.strip().split('\n')[0]
            print(f"  [✓] Found {name}: {version_info}")
        except (subprocess.CalledProcessError, FileNotFoundError):
            print(f"  [✗] {name} not found. Please install it and ensure it's in your system's PATH.")
            all_tools_found = False

    print("\n--------------------------------------------------")
    if all_tools_found:
        print("Success! All required tools are installed.")
        sys.exit(0)
    else:
        print("Some tools are missing. Please see CONTRIBUTING.md for setup instructions.")
        sys.exit(1)

if __name__ == "__main__":
    check_toolchain()
