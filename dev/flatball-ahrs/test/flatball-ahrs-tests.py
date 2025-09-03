import os
import subprocess

TEST_ROOT = os.path.abspath(os.path.dirname(__file__))
REPORT_FILE = os.path.join(TEST_ROOT, "flatball-ahrs-tests.log")

def find_test_scripts():
    scripts = []
    for root, _, files in os.walk(TEST_ROOT):
        for fname in files:
            if fname.startswith("tst_") and fname.endswith(".py"):
                scripts.append(os.path.join(root, fname))
    return scripts

def run_test(script_path):
    print(f"🚀 Running: {script_path}")
    try:
        subprocess.run(["python", script_path], check=True, cwd=os.path.dirname(script_path))
    except subprocess.CalledProcessError as e:
        print(f"❌ Error running {script_path}: {e}")

def extract_log_data(script_path):
    folder = os.path.dirname(script_path)
    base = os.path.splitext(os.path.basename(script_path))[0]
    log_path = os.path.join(folder, base + ".log")

    if not os.path.exists(log_path):
        return [f"#log-missing: {log_path}"]

    lines = []
    with open(log_path, "r", encoding="utf-8") as log:
        for line in log:
            if line.lstrip().startswith("#"):
                lines.append(line.strip())
    return lines

def main():
    scripts = find_test_scripts()
    report_lines = []

    for script in scripts:
        run_test(script)
        log_lines = extract_log_data(script)
        report_lines.append(f"#report-from: {os.path.relpath(script, TEST_ROOT)}")
        report_lines.extend(log_lines)
        report_lines.append("")  # Spacer

    with open(REPORT_FILE, "w", encoding="utf-8") as report:
        report.write("\n".join(report_lines))

    print(f"\n📄 Collated report saved to: {REPORT_FILE}")

if __name__ == "__main__":
    main()
