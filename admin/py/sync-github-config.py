#!/usr/bin/env python3
"""
sync-github-config.py

Synchronize labels and milestones to GitHub and write a concise Markdown run log.
- Uses `gh` CLI.
- Writes markdown to admin/logs-and-reports/sync-github-config-log.md.
- Uses `gh label create --force` to idempotently ensure labels.
- Uses `gh api` with JSON stdin to create and PATCH milestones.
- Skips/patches existing milestones and logs only concise success/failure info.
"""

from __future__ import annotations
import json
import subprocess
import argparse
import os
from datetime import datetime, timedelta, timezone
from typing import Dict, Any, List, Tuple

# Config
ROOT = os.path.dirname(__file__) or "."
LOG_DIR = os.path.join(ROOT, "..", "..", "admin", "logs")
os.makedirs(LOG_DIR, exist_ok=True)
LOG_FILE = os.path.join(LOG_DIR, "sync-github-config-log.md")
REPO = "misc-protolabs/flatball-ins"
LABELS_JSON = os.path.join(ROOT, "..", "json", "labels.json")
MILESTONES_JSON = os.path.join(ROOT, "..", "json", "milestones.json")


def now_iso() -> str:
    return datetime.now(timezone.utc).astimezone().isoformat()


def default_due_date() -> str:
    future_date = datetime.now(timezone.utc) + timedelta(days=5 * 365)
    return future_date.strftime("%Y-%m-%dT%H:%M:%SZ")


def append_line(s: str = "") -> None:
    os.makedirs(LOG_DIR, exist_ok=True)
    with open(LOG_FILE, "a", encoding="utf-8") as f:
        f.write(s + "\n")


def log_run_header() -> None:
    append_line(f"# Sync run: {now_iso()}")
    append_line("")
    append_line(f"[{now_iso()}] === Sync Started ===")
    append_line("")


def log_run_footer() -> None:
    append_line(f"[{now_iso()}] === Sync Complete ===")
    append_line("")


def run_cmd(cmd: List[str], input_text: str | None = None) -> Tuple[int, str, str]:
    try:
        proc = subprocess.run(cmd, input=input_text, capture_output=True, text=True, check=False)
        return proc.returncode, proc.stdout or "", proc.stderr or ""
    except FileNotFoundError as e:
        return 127, "", f"Command not found: {e}"


def sync_labels(dry_run: bool) -> None:
    append_line("### Label sync")
    append_line("")
    if not os.path.exists(LABELS_JSON):
        append_line(f"- Labels config missing: `{LABELS_JSON}`")
        append_line("")
        return

    with open(LABELS_JSON, "r", encoding="utf-8") as f:
        labels = json.load(f)

    for label in labels:
        name = label["name"]
        color = label["color"]
        desc = label.get("description", "")
        append_line(f"- Ensuring label `{name}`")

        if dry_run:
            append_line(f"  - DRY RUN: would run `gh label create --force` for `{name}`")
            continue

        cmd = ["gh", "label", "create", name, "--color", color, "--description", desc, "--repo", REPO, "--force"]
        rc, out, err = run_cmd(cmd)
        if rc == 0:
            append_line(f"  - OK: created/updated `{name}`")
        else:
            # compact failure message
            msg = (err.strip().splitlines()[0] if err else "unknown error")
            append_line(f"  - ERROR: `{name}`: {msg}")
    append_line("")


def list_existing_milestones(dry_run: bool) -> Dict[str, Dict[str, Any]]:
    append_line("### Existing milestones")
    append_line("")
    mapping: Dict[str, Dict[str, Any]] = {}
    if dry_run:
        append_line("- DRY RUN: would list existing milestones")
        append_line("")
        return mapping

    cmd = ["gh", "api", f"repos/{REPO}/milestones", "--paginate"]
    rc, out, err = run_cmd(cmd)
    if rc != 0:
        msg = (err.strip().splitlines()[0] if err else "failed to list milestones")
        append_line(f"- ERROR listing milestones: {msg}")
        append_line("")
        return mapping

    try:
        items: List[Dict[str, Any]] = json.loads(out)
    except Exception:
        append_line("- ERROR parsing milestones JSON response")
        append_line("```")
        append_line(out.strip())
        append_line("```")
        append_line("")
        return mapping

    for item in items:
        title = item.get("title")
        number = item.get("number") or item.get("id")
        due_on = item.get("due_on")
        mapping[title] = item
        append_line(f"- **{title}** (id: `{number}`) due_on: `{due_on}`")
    append_line("")
    return mapping


def create_milestone(body: Dict[str, Any], dry_run: bool) -> bool:
    append_line(f"- Creating milestone `{body.get('title')}` due_on `{body.get('due_on')}`")
    if dry_run:
        append_line("  - DRY RUN: would POST JSON body:")
        append_line("```json")
        append_line(json.dumps(body, indent=2))
        append_line("```")
        return True

    cmd = ["gh", "api", f"repos/{REPO}/milestones", "--method", "POST", "--input", "-"]
    rc, out, err = run_cmd(cmd, input_text=json.dumps(body))
    if rc != 0:
        msg = (err.strip().splitlines()[0] if err else "create failed")
        append_line(f"  - ERROR creating milestone: {msg}")
        return False
    append_line("  - OK created")
    return True


def patch_milestone(number: int, body: Dict[str, Any], dry_run: bool) -> bool:
    append_line(f"- Patching milestone id `{number}` with changes")
    if dry_run:
        append_line("  - DRY RUN: would PATCH JSON body:")
        append_line("```json")
        append_line(json.dumps(body, indent=2))
        append_line("```")
        return True

    cmd = ["gh", "api", f"repos/{REPO}/milestones/{number}", "--method", "PATCH", "--input", "-"]
    rc, out, err = run_cmd(cmd, input_text=json.dumps(body))
    if rc != 0:
        msg = (err.strip().splitlines()[0] if err else "patch failed")
        append_line(f"  - ERROR patching milestone: {msg}")
        return False
    append_line("  - OK patched")
    return True


def sync_milestones(dry_run: bool) -> None:
    append_line("### Milestone sync")
    append_line("")
    if not os.path.exists(MILESTONES_JSON):
        append_line(f"- Milestones config missing: `{MILESTONES_JSON}`")
        append_line("")
        return

    with open(MILESTONES_JSON, "r", encoding="utf-8") as f:
        desired = json.load(f)

    existing = list_existing_milestones(dry_run=dry_run)

    for m in desired:
        title = m["title"]
        desc = m.get("description", "")
        desired_due = m.get("due_on") or default_due_date()

        if title in existing:
            item = existing[title]
            number = item.get("number") or item.get("id")
            current_due = item.get("due_on")
            current_desc = item.get("description") or ""
            append_line(f"- Found existing milestone `{title}` (id: `{number}`)")
            changes: Dict[str, Any] = {}
            if current_due != desired_due:
                changes["due_on"] = desired_due
            if current_desc != desc:
                changes["description"] = desc
            if changes:
                patch_milestone(number, changes, dry_run=dry_run)
            else:
                append_line("  - No changes needed")
            continue

        body = {
            "title": title,
            "description": desc,
            "due_on": desired_due,
            "state": "open",
        }
        create_milestone(body, dry_run=dry_run)
    append_line("")


def parse_args():
    p = argparse.ArgumentParser(description="Sync GitHub labels and milestones; write markdown log.")
    p.add_argument("--dry-run", action="store_true", help="Preview only")
    return p.parse_args()


def main():
    args = parse_args()
    # Start new run: truncate existing log for clarity (append if you prefer)
    with open(LOG_FILE, "w", encoding="utf-8") as f:
        f.write("")  # clear previous log
    log_run_header()

    sync_labels(dry_run=args.dry_run)
    sync_milestones(dry_run=args.dry_run)

    log_run_footer()
    print(f"Markdown log written to: {LOG_FILE}")


if __name__ == "__main__":
    main()
