import json
import subprocess
import argparse
from datetime import datetime
import os

LOG_DIR = os.path.join("admin", "logs")
os.makedirs(LOG_DIR, exist_ok=True)
LOG_FILE = os.path.join(LOG_DIR, "sync-github-config.log")
REPO = "misc-protolabs/flatball-ins"  # Replace with your actual repo

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
    with open(".github/labels.json") as f:
        labels = json.load(f)["labels"]
    for label in labels:
        name = label["name"]
        color = label["color"]
        desc = label.get("description", "")
        log(f"Syncing label: {name}")
        cmd_create = f'gh label create "{name}" --color "{color}" --description "{desc}" --repo {REPO}'
        cmd_edit = f'gh label edit "{name}" --color "{color}" --description "{desc}" --repo {REPO}'
        run(f"{cmd_create} || {cmd_edit}", dry_run)

def sync_milestones(dry_run):
    with open(".github/milestones.json") as f:
        milestones = json.load(f)["milestones"]
    for m in milestones:
        title = m["title"]
        desc = m.get("description", "")
        due = m.get("due_on", "")
        state = m.get("state", "open")
        log(f"Syncing milestone: {title}")
        cmd = (
            f'gh api repos/{REPO}/milestones --method POST '
            f'-f title="{title}" -f description="{desc}" -f due_on="{due}" -f state="{state}"'
        )
        run(cmd, dry_run)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Sync GitHub labels and milestones")
    parser.add_argument("--dry-run", action="store_true", help="Preview actions without executing")
    args = parser.parse_args()

    log("=== Sync Started ===")
    sync_labels(args.dry_run)
    sync_milestones(args.dry_run)
    log("=== Sync Complete ===")
