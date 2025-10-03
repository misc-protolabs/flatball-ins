import subprocess
import os
import signal
import time
import psutil
import datetime
from threading import Thread

# Shared shutdown flag path
SHUTDOWN_FLAG = os.path.abspath(os.path.join(os.path.dirname(__file__), ".shutdown.flag"))

# Timestamped logs
timestamp = datetime.datetime.now().strftime("%Y-%m-%d_%H-%M-%S")
log_dir = os.path.join(os.path.dirname(__file__), "logs")
os.makedirs(log_dir, exist_ok=True)

ollama_log = os.path.join(log_dir, f"ollama_{timestamp}.md")
npm_log = os.path.join(log_dir, f"npm_{timestamp}.md")
heartbeat_log = os.path.join(log_dir, f"heartbeat_{timestamp}.md")

npm_path = r"C:\Program Files\nodejs\npm.cmd"
npm_dir = os.path.dirname(__file__)

def stream_output(proc, logfile):
    with open(logfile, "w", encoding="utf-8", errors="replace") as f:
        while True:
            line = proc.stdout.readline()
            if not line:
                break
            decoded = line.decode(errors="ignore").rstrip()
            print(decoded)
            f.write(decoded + "\n")
            f.flush()

def launch_process(cmd, cwd, logfile):
    proc = subprocess.Popen(
        cmd,
        cwd=cwd,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        creationflags=subprocess.CREATE_NEW_PROCESS_GROUP if os.name == "nt" else 0
    )
    Thread(target=stream_output, args=(proc, logfile), daemon=True).start()
    return proc

print("Starting ollama serve...")
ollama_proc = launch_process(["ollama", "serve"], None, ollama_log)

time.sleep(2)

print("Starting npm start...")
npm_proc = launch_process([npm_path, "start"], npm_dir, npm_log)

def shutdown():
    print("\nShutting down processes...")
    for proc in [ollama_proc, npm_proc]:
        try:
            if proc.poll() is None:
                parent = psutil.Process(proc.pid)
                children = parent.children(recursive=True)
                for child in children:
                    child.terminate()
                parent.terminate()
        except Exception as e:
            print(f"Error stopping process: {e}")
    print(f"Logs saved to: {log_dir}")

try:
    print("\nWaiting for shutdown signal (Ctrl+C or .shutdown.flag)...\n")
    while True:
        if os.path.exists(SHUTDOWN_FLAG):
            print(f"Shutdown flag detected at {SHUTDOWN_FLAG}")
            shutdown()
            break

        now = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        heartbeat = f"[heartbeat] Launcher alive at {now}"
        print(heartbeat)
        with open(heartbeat_log, "a", encoding="utf-8") as hb:
            hb.write(f"{heartbeat}\n")
        time.sleep(5)
except KeyboardInterrupt:
    shutdown()