import json
import subprocess
import argparse
import os
from datetime import datetime, timedelta, timezone

LOG_DIR = os.path.join("..", "..", "admin", "logs-and-reports")
os.makedirs(LOG_DIR, exist_ok=True)
LOG_FILE = os.path.join(LOG_DIR, "sync-github-config-log.txt")
REPO = "misc-protolabs/flatball-ins"  # Replace with your actual repo

def default_due_date():
    future_date = datetime.now(timezone.utc) + timedelta(days=5*365)
    return future_date.strftime("%Y-%m-%dT%H:%M:%SZ")

def log(msg):
    timestamp = datetime.now().isoformat()
    os.makedirs(LOG_DIR, exist_ok=True)
    with open(LOG_FILE, "a") as f:
        f.write(f"[{timestamp}] {msg}\n")
    print(msg)

def run(cmd, dry_run):
    log(f"CMD: {cmd}")
    if dry_run:
        log("Dry run: command not executed.")
        return ""
    result = subprocess.run(cmd, shell=True, capture_output=True, text=True)
    if result.returncode != 0:
        log(f"Error: {result.stderr}")
    return result.stdout.strip()

def sync_labels(dry_run):
    repo = "misc-protolabs/flatball-ins"
    with open("../../json/labels.json") as f:
        labels = json.load(f)
    for label in labels:
        name = label["name"]
        color = label["color"]
        desc = label.get("description", "")
        log(f"Syncing label: {name}")
        cmd_create = f'gh label create "{name}" --color "{color}" --description "{desc}" --repo {REPO}'
        cmd_edit = f'gh label edit "{name}" --color "{color}" --description "{desc}" --repo {REPO}'
        run(f"{cmd_create} || {cmd_edit}", dry_run)

def sync_milestones(dry_run):
    repo = "misc-protolabs/flatball-ins"
    with open("../../json/milestones.json") as f:
        milestones = json.load(f)
    for m in milestones:
        due_on = m.get("due_on") or default_due_date()
        cmd = [
            "gh", "api", f"repos/{repo}/milestones",
            "--method", "POST",
            "-f", f"title={m['title']}",
            "-f", f"description={m['description']}",
            "-f", f"due_on={due_on}",
            "-f", "state=open"
        ]
        run(cmd, dry_run)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Sync GitHub labels and milestones")
    parser.add_argument("--dry-run", action="store_true", help="Preview actions without executing")
    args = parser.parse_args()

    log("=== Sync Started ===")
    sync_labels(args.dry_run)
    #sync_milestones(args.dry_run)
    log("=== Sync Complete ===")
