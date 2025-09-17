#!/usr/bin/env python3
import os
import sys
import time
import argparse
import logging
import re

LICENSE_HEADER = """\
Copyright 2025 Michael V. Schaefer

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at:

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""

COMMENT_STYLES = {
    '.py': '# ',
    '.js': '// ',
    '.c': '// ',
    '.cpp': '// ',
    '.h': '// ',
    '.java': '// ',
    '.ts': '// ',
    '.ino': '// ',
    '.sch': '# ',
    '.kicad_pcb': '# ',
    '.md': '<!-- ',
    '.m': '% ',
}

def setup_logging():
    script_name = os.path.splitext(os.path.basename(sys.argv[0]))[0]
    log_file = os.path.join(os.getcwd(), "..", "admin", "logs", "propogate-lic-log.md")
    logging.basicConfig(
        level=logging.INFO,
        format='%(asctime)s [%(levelname)s] %(message)s',
        handlers=[
            logging.FileHandler(log_file, mode='w', encoding='utf-8'),
            logging.StreamHandler(sys.stdout)
        ]
    )
    logging.info(f"Logging to {log_file}")

def format_header(ext):
    prefix = COMMENT_STYLES[ext]
    lines = LICENSE_HEADER.splitlines()
    if prefix == '<!-- ':
        body = '\n'.join(f"{prefix}{line}" for line in lines)
        return f"{prefix}\n{body}\n-->\n\n"
    else:
        return '\n'.join(f"{prefix}{line}" for line in lines) + '\n\n'

def build_removal_regex(ext):
    """
    Build a regex that matches the header as inserted by format_header for the given extension.
    It matches from the start of the file, allowing an optional shebang line before the header
    for scripts (e.g. #!/usr/bin/env python3). For HTML-style comment, the block is detected.
    """
    prefix = COMMENT_STYLES[ext]
    # Escape content of LICENSE_HEADER for safe regex usage
    escaped_lines = [re.escape(line) for line in LICENSE_HEADER.splitlines()]
    if prefix == '<!-- ':
        # HTML-style block: allow optional whitespace/newlines before/after
        pattern = r'^\s*<!--\s*\n'  # opening
        for ln in escaped_lines:
            pattern += r'(?:' + re.escape(prefix) + r')?' + ln + r'\n'
        pattern += r'-->\n*\n*'
    else:
        # For line-comment styles, match optional shebang, then consecutive commented lines forming the header
        shebang = r'(?:#![^\n]+\n)?'  # optional shebang
        commented_block = ''
        for ln in escaped_lines:
            # Each header line was prefixed with prefix (e.g. "# " or "// ")
            commented_block += re.escape(prefix) + ln + r'\n'
        pattern = r'^' + shebang + commented_block + r'\n*'
    return re.compile(pattern, flags=re.MULTILINE)

def insert_license_header(file_path, ext, dry_run):
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            content = f.read()
    except Exception as e:
        logging.error(f"Cannot read {file_path}: {e}")
        return 'error'

    marker = "Licensed under the Apache License"
    if marker in content:
        return 'skipped'

    header = format_header(ext)

    if dry_run:
        return 'dry-run'

    try:
        with open(file_path, 'w', encoding='utf-8') as f:
            f.write(header + content)
        return 'inserted'
    except Exception as e:
        logging.error(f"Cannot write {file_path}: {e}")
        return 'error'

def remove_license_header(file_path, ext, dry_run):
    """
    Remove the exact header inserted by format_header for the given extension,
    if present at the top of the file (allowing an optional shebang before the header).
    Returns 'removed', 'not-found', 'dry-run', or 'error'.
    """
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            content = f.read()
    except Exception as e:
        logging.error(f"Cannot read {file_path}: {e}")
        return 'error'

    # Build regex to match the header at the start (or after an optional shebang)
    regex = build_removal_regex(ext)

    if not regex.search(content):
        return 'not-found'

    if dry_run:
        return 'dry-run'

    try:
        new_content = regex.sub('', content, count=1)
        with open(file_path, 'w', encoding='utf-8') as f:
            f.write(new_content)
        return 'removed'
    except Exception as e:
        logging.error(f"Cannot write {file_path}: {e}")
        return 'error'

def path_contains_extern(dirpath):
    """
    Return True if any path component equals 'extern'.
    """
    parts = [p for p in dirpath.split(os.sep) if p]
    return 'extern' in parts

def scan_and_insert(root_dir, dry_run=False, report=False):
    counters = {
        'scanned': 0,
        'matched_ext': 0,
        'inserted': 0,
        'skipped': 0,
        'removed': 0,
        'not-found': 0,
        'dry-run': 0,
        'errors': 0,
    }
    modified_files = []

    logging.info(f"Starting license insertion/removal in: {os.path.abspath(root_dir)}")
    if dry_run:
        logging.info("Dry-run mode: no files will be modified")

    start_time = time.time()

    for dirpath, _, filenames in os.walk(root_dir):
        is_extern = path_contains_extern(os.path.relpath(dirpath, root_dir))
        for filename in filenames:
            counters['scanned'] += 1
            ext = os.path.splitext(filename)[1]
            file_path = os.path.join(dirpath, filename)

            if ext not in COMMENT_STYLES:
                continue

            counters['matched_ext'] += 1
            logging.info(f"[SCAN] {file_path} {'(extern)' if is_extern else ''}")

            if is_extern:
                # In extern folders: remove the header if present
                result = remove_license_header(file_path, ext, dry_run)
                if result == 'removed':
                    counters['removed'] += 1
                    modified_files.append(file_path)
                    logging.info("  → [REMOVED]")
                elif result == 'not-found':
                    counters['not-found'] += 1
                    logging.info("  → [NOT FOUND] no header to remove")
                elif result == 'dry-run':
                    counters['dry-run'] += 1
                    modified_files.append(file_path)
                    logging.info("  → [DRY-RUN] would remove header")
                else:
                    counters['errors'] += 1
            else:
                # Normal folders: insert header if missing
                result = insert_license_header(file_path, ext, dry_run)
                if result == 'inserted':
                    counters['inserted'] += 1
                    modified_files.append(file_path)
                    logging.info("  → [INSERTED]")
                elif result == 'skipped':
                    counters['skipped'] += 1
                    logging.info("  → [SKIPPED] already has license")
                elif result == 'dry-run':
                    counters['dry-run'] += 1
                    modified_files.append(file_path)
                    logging.info("  → [DRY-RUN] would insert license")
                else:
                    counters['errors'] += 1

    elapsed = time.time() - start_time
    logging.info("--- Summary ---")
    for k, v in counters.items():
        logging.info(f"{k.replace('-', ' ').capitalize():<25}: {v}")
    logging.info(f"Elapsed time              : {elapsed:.2f} seconds")

    if report:
        write_markdown_report(counters, modified_files, dry_run)

    return counters

def write_markdown_report(counters, modified_files, dry_run):
    report_path = os.path.join(os.getcwd(), "..", "admin", "logs", "propogate-lic-log.md")
    os.makedirs(os.path.dirname(report_path), exist_ok=True)
    with open(report_path, 'w', encoding='utf-8') as f:
        f.write("# 📝 License Header Insertion Report\n\n")
        f.write(f"**Run mode**: {'Dry-run' if dry_run else 'Actual insertion/removal'}\n\n")
        f.write("## 📊 Summary\n")
        for k, v in counters.items():
            f.write(f"- {k.replace('-', ' ').capitalize()}: {v}\n")
        f.write("\n## 📁 Modified Files\n")
        if modified_files:
            for path in modified_files:
                f.write(f"- `{path}`\n")
        else:
            f.write("_No files modified._\n")
    logging.info(f"Markdown report written to: {report_path}")

if __name__ == "__main__":
    setup_logging()

    parser = argparse.ArgumentParser(
        description="Insert Apache 2.0 license headers into source files. Remove headers inside 'extern' folders."
    )
    parser.add_argument(
        "root",
        nargs="?",
        default=".",
        help="Root directory to scan (default: current directory)"
    )
    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="Preview changes without modifying any files"
    )
    parser.add_argument(
        "--report",
        action="store_true",
        help="Generate a Markdown summary report"
    )
    args = parser.parse_args()

    scan_and_insert(args.root, dry_run=args.dry_run, report=args.report)
