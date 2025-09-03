import re
import matplotlib.pyplot as plt

# Parse log
sensor_data = []
timestamp = 0.0
interval = 0.1  # 10Hz sampling

with open("flatball-ahrs-profiler.log", "r") as f:
    for line in f:
        matches = re.findall(r"\[\s*([\d.\-]+),\s*([\d.\-]+)\s*\]", line)
        if len(matches) == 6:
            parsed = [(float(a), float(b)) for a, b in matches]
            sensor_data.append({
                "timestamp": timestamp,
                "core0": {
                    "100Hz": parsed[0],
                    "10Hz":  parsed[1],
                    "1Hz":   parsed[2],
                },
                "core1": {
                    "100Hz": parsed[3],
                    "10Hz":  parsed[4],
                    "1Hz":   parsed[5],
                }
            })
            timestamp += interval

# Time axis
timestamps = [row["timestamp"] for row in sensor_data]
freqs = ["100Hz", "10Hz", "1Hz"]

def plot_core(core_name, output_suffix):
    plt.figure(figsize=(10, 9))
    for i, freq in enumerate(freqs):
        means = [row[core_name][freq][0] for row in sensor_data]
        stddevs = [row[core_name][freq][1] for row in sensor_data]

        plt.subplot(3, 1, i + 1)
        plt.plot(timestamps, means, label="Mean", color="tab:blue")
        plt.plot(timestamps, stddevs, label="Stddev", color="tab:orange", linestyle="--")
        plt.title(f"{core_name.upper()} – {freq}")
        plt.xlabel("Time (s)")
        plt.ylabel("µs")
        plt.ylim(0, 10000)  # 0–10 ms
        plt.grid(True)
        plt.legend()

    plt.tight_layout()
    plt.savefig(f"flatball-ahrs-profiler.{output_suffix}.png", dpi=300)
    plt.close()

# Generate and save
plot_core("core0", "core0")
plot_core("core1", "core1")