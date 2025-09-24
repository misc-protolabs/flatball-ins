#!/usr/bin/env python3
import os
import sys
import time
import argparse
import logging
import re
from typing import Optional, Tuple, List

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

# Base default styles (line comments).
# HTML-style block handled specially.
BASE_COMMENT_STYLES = {
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
    '.md': '<!-- ',  # block comment handled in format_header()
    # '.m' handled via resolver (MATLAB vs Objective-C)
}

# For normalization when detecting existing headers, we’ll consider these prefixes as valid comment starters.
NORMALIZATION_PREFIXES = {
    'line': ['# ', '// ', '% '],
    'block_start': ['<!--', '/*'],
    'block_end': ['-->', '*/'],
}

APACHE_MARKER = "Licensed under the Apache License"


def setup_logging():
    script_name = os.path.splitext(os.path.basename(sys.argv[0]))[0]
    log_dir = os.path.join(os.getcwd(), "..", "admin", "logs")
    os.makedirs(log_dir, exist_ok=True)
    log_file = os.path.join(log_dir, "propagate-lic-log.md")
    logging.basicConfig(
        level=logging.INFO,
        format='%(asctime)s [%(levelname)s] %(message)s',
        handlers=[
            logging.FileHandler(log_file, mode='w', encoding='utf-8'),
            logging.StreamHandler(sys.stdout)
        ]
    )
    logging.info(f"Logging to {log_file}")


def is_matlab_m(file_path: str, first_kb: int = 4096) -> bool:
    """
    Heuristic to detect MATLAB/Octave .m vs Objective-C .m
    - MATLAB commonly starts with '%' comments, 'function', 'classdef'
    - Objective-C commonly has '#import', '@interface', '@implementation', or C includes
    """
    try:
        with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
            head = f.read(first_kb)
    except Exception:
        # Default to MATLAB if unreadable; safer for codebases using MATLAB
        return True

    head_stripped = head.strip()
    matlab_indicators = [
        r'^\s*%', r'^\s*%%', r'^\s*function\b', r'^\s*classdef\b',
        r'%#ok', r'\bend\b'
    ]
    objc_indicators = [
        r'#import\b', r'@interface\b', r'@implementation\b',
        r'#include\b', r'^\s*//', r'/\*'
    ]

    for pat in objc_indicators:
        if re.search(pat, head, flags=re.MULTILINE):
            return False
    for pat in matlab_indicators:
        if re.search(pat, head_stripped, flags=re.MULTILINE):
            return True

    # Fallback: if there are semicolons, numeric arrays, and no Obj-C tokens, lean MATLAB.
    if ';' in head and '@interface' not in head and '#import' not in head:
        return True

    # Final fallback: assume MATLAB (less damaging for scientific repos)
    return True


def resolve_comment_style(file_path: str, ext: str, override_style: Optional[str] = None) -> str:
    """
    Resolve comment prefix style for a given file:
    - override_style: explicit string comment prefix (e.g., '% ', '// ', '<!-- ')
    - .m handled via heuristics
    """
    if override_style:
        return override_style

    if ext == '.m':
        return '% ' if is_matlab_m(file_path) else '// '

    if ext in BASE_COMMENT_STYLES:
        return BASE_COMMENT_STYLES[ext]

    # Unknown extension → no style
    return ''


def format_header(ext: str, prefix: str) -> str:
    """
    Produce the canonical header for the given extension and resolved prefix.
    Special-case HTML-style block for .md using <!-- ... -->.
    """
    lines = LICENSE_HEADER.splitlines()
    if ext == '.md' or prefix.startswith('<!--'):
        body = '\n'.join(f"<!-- {line}" for line in lines)
        return f"<!--\n{body}\n-->\n\n"
    else:
        return '\n'.join(f"{prefix}{line}" for line in lines) + '\n\n'


def normalize_text_block(lines: List[str]) -> str:
    """
    Normalize extracted header lines:
    - Strip known comment prefixes from line starts
    - Trim whitespace
    """
    normalized = []
    for ln in lines:
        s = ln.rstrip('\n')
        # Remove block comment markers if present inline
        s = re.sub(r'^\s*(?:<!--\s*|/\*\s*)', '', s)
        s = re.sub(r'\s*(?:-->|\/\*)\s*$', '', s)
        # Remove known line prefixes
        for pfx in NORMALIZATION_PREFIXES['line']:
            if s.startswith(pfx):
                s = s[len(pfx):]
                break
        normalized.append(s.strip())
    return '\n'.join(normalized).strip()


def find_license_block_span(content: str) -> Optional[Tuple[int, int, List[str]]]:
    """
    Attempt to find an Apache 2.0 license header block at the top of the file.
    Returns (start_index, end_index, lines) if found, else None.
    Strategy:
    - Consider only the first ~120 lines to avoid scanning entire files unnecessarily.
    - Look for either:
      - A contiguous run of line-commented lines containing the marker; or
      - An HTML/C-style block comment containing the marker.
    """
    head_limit = 120
    lines = content.splitlines(keepends=True)
    head = lines[:head_limit]

    # Detect optional shebang line at very top
    idx = 0
    if idx < len(head) and head[idx].startswith('#!'):
        idx += 1

    # Try block comment first: <!-- ... --> or /* ... */
    # Scan for block start within first few lines
    for start in range(idx, min(idx + 10, len(head))):
        s = head[start].lstrip()
        if s.startswith('<!--') or s.startswith('/*'):
            # Collect until block end
            end = start
            block_lines = [head[start]]
            while end + 1 < len(head):
                end += 1
                block_lines.append(head[end])
                if head[end].strip().endswith('-->') or head[end].strip().endswith('*/'):
                    break
            block_text = ''.join(block_lines)
            if APACHE_MARKER in block_text:
                # Compute absolute span in original content
                start_idx = sum(len(l) for l in lines[:start])
                end_idx = sum(len(l) for l in lines[:end + 1])
                return (start_idx, end_idx, block_lines)

    # Try line-commented header: consecutive lines starting with known prefixes
    start = idx
    collected = []
    while start < len(head):
        ln = head[start]
        stripped = ln.lstrip()
        if any(stripped.startswith(p) for p in NORMALIZATION_PREFIXES['line']):
            collected.append(ln)
            start += 1
            continue
        break

    if collected and APACHE_MARKER in ''.join(collected):
        start_idx = sum(len(l) for l in lines[:idx])
        end_idx = sum(len(l) for l in lines[:idx + len(collected)])
        return (start_idx, end_idx, collected)

    return None


def has_correct_header(content: str, expected_header: str) -> bool:
    """
    Strict check: does the file start with expected_header (accounting for optional shebang)?
    """
    if content.startswith(expected_header):
        return True
    if content.startswith('#!'):
        # skip first line
        first_newline = content.find('\n')
        if first_newline != -1:
            after_shebang = content[first_newline + 1:]
            return after_shebang.startswith(expected_header)
    return False


def header_is_logically_apache(block_lines: List[str]) -> bool:
    """
    Normalize a found block and compare to LICENSE_HEADER text only (ignoring prefixes/spacing).
    """
    normalized_block = normalize_text_block(block_lines)
    normalized_expected = '\n'.join(LICENSE_HEADER.splitlines()).strip()
    return normalized_block == normalized_expected


def path_contains_extern(dirpath: str) -> bool:
    """
    Return True if any path component equals 'extern'.
    """
    parts = [p for p in dirpath.split(os.sep) if p]
    return 'extern' in parts


def insert_or_fix_header(file_path: str, ext: str, prefix: str, dry_run: bool) -> str:
    """
    Insert or fix a license header at the top of the file.
    Returns: 'inserted', 'fixed', 'skipped', 'dry-run', or 'error'
    """
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            content = f.read()
    except Exception as e:
        logging.error(f"Cannot read {file_path}: {e}")
        return 'error'

    expected = format_header(ext, prefix)

    # Strict correctness check
    if has_correct_header(content, expected):
        return 'skipped'

    # If some license block exists but is incorrect, remove it then insert canonical
    found = find_license_block_span(content)
    if found:
        start_idx, end_idx, block_lines = found
        if header_is_logically_apache(block_lines):
            # Replace malformed or wrong-prefix header
            new_content = content[:start_idx] + content[end_idx:]
            if dry_run:
                return 'dry-run'
            try:
                with open(file_path, 'w', encoding='utf-8') as f:
                    f.write(expected + new_content)
                return 'fixed'
            except Exception as e:
                logging.error(f"Cannot write {file_path}: {e}")
                return 'error'
        else:
            # Some other header present; we won't modify unless marker is Apache (avoid nuking foreign licenses)
            # If you want to force-replace any header containing the marker, handle above.
            if APACHE_MARKER in ''.join(block_lines):
                # Replace anyway (Apache but text differs)
                new_content = content[:start_idx] + content[end_idx:]
                if dry_run:
                    return 'dry-run'
                try:
                    with open(file_path, 'w', encoding='utf-8') as f:
                        f.write(expected + new_content)
                    return 'fixed'
                except Exception as e:
                    logging.error(f"Cannot write {file_path}: {e}")
                    return 'error'
            else:
                # Different license header; skip
                return 'skipped'

    # No existing header → insert
    if dry_run:
        return 'dry-run'

    try:
        # Preserve shebang
        if content.startswith('#!'):
            nl = content.find('\n')
            if nl != -1:
                shebang = content[:nl + 1]
                body = content[nl + 1:]
                new_content = shebang + expected + body
            else:
                # odd edge-case: shebang without newline
                new_content = content + '\n' + expected
        else:
            new_content = expected + content

        with open(file_path, 'w', encoding='utf-8') as f:
            f.write(new_content)
        return 'inserted'
    except Exception as e:
        logging.error(f"Cannot write {file_path}: {e}")
        return 'error'


def remove_header_if_present(file_path: str, dry_run: bool) -> str:
    """
    Remove any detected Apache header block at the top (line or block style).
    Returns: 'removed', 'not-found', 'dry-run', or 'error'
    """
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            content = f.read()
    except Exception as e:
        logging.error(f"Cannot read {file_path}: {e}")
        return 'error'

    found = find_license_block_span(content)
    if not found:
        return 'not-found'

    start_idx, end_idx, block_lines = found
    if APACHE_MARKER not in ''.join(block_lines):
        # Only remove Apache header; leave foreign headers alone
        return 'not-found'

    if dry_run:
        return 'dry-run'

    try:
        new_content = content[:start_idx] + content[end_idx:]
        with open(file_path, 'w', encoding='utf-8') as f:
            f.write(new_content)
        return 'removed'
    except Exception as e:
        logging.error(f"Cannot write {file_path}: {e}")
        return 'error'


def scan(root_dir: str, dry_run: bool = False, report: bool = False,
         verify: bool = False, override_map: Optional[dict] = None) -> dict:
    """
    Main scanning routine:
    - In extern folders: remove Apache headers
    - Else: insert or fix headers
    - verify: only report correctness; no modifications
    - override_map: {".m": "% ", ".md": "<!-- ", ...} to explicitly set style per extension
    """
    counters = {
        'scanned': 0,
        'matched_ext': 0,
        'inserted': 0,
        'fixed': 0,
        'skipped': 0,
        'removed': 0,
        'not-found': 0,
        'dry-run': 0,
        'errors': 0,
        'incorrect': 0,  # verify mode: incorrect/missing
        'correct': 0,    # verify mode: correct
    }
    modified_files = []
    incorrect_files = []
    correct_files = []

    logging.info(f"Starting license propagation in: {os.path.abspath(root_dir)}")
    if dry_run:
        logging.info("Dry-run mode: no files will be modified")
    if verify:
        logging.info("Verify mode: reporting correctness only (no modifications)")

    start_time = time.time()

    for dirpath, dirnames, filenames in os.walk(root_dir):
        # prune unwanted dirs
        dirnames[:] = [d for d in dirnames if d not in ("node_modules", ".git", "__pycache__")]

        rel_dir = os.path.relpath(dirpath, root_dir)
        is_extern = path_contains_extern(rel_dir)

        for filename in filenames:
            counters['scanned'] += 1
            ext = os.path.splitext(filename)[1]
            file_path = os.path.join(dirpath, filename)

            # Include .m plus known styles; if override_map present, treat if ext in overrides
            known_exts = set(BASE_COMMENT_STYLES.keys()) | {'.m'}
            if ext not in known_exts and not (override_map and ext in override_map):
                continue

            counters['matched_ext'] += 1
            logging.info(f"[SCAN] {file_path} {'(extern)' if is_extern else ''}")

            # Resolve style
            override_style = (override_map or {}).get(ext)
            prefix = resolve_comment_style(file_path, ext, override_style)

            # Verify-only branch
            if verify:
                try:
                    with open(file_path, 'r', encoding='utf-8') as f:
                        content = f.read()
                except Exception as e:
                    logging.error(f"Cannot read {file_path}: {e}")
                    counters['errors'] += 1
                    continue

                expected = format_header(ext, prefix)
                if has_correct_header(content, expected):
                    counters['correct'] += 1
                    correct_files.append(file_path)
                    logging.info("  → [OK] correct license header")
                else:
                    counters['incorrect'] += 1
                    incorrect_files.append(file_path)
                    logging.info("  → [INCORRECT] missing or malformed license header")
                continue

            # Modify mode
            if is_extern:
                result = remove_header_if_present(file_path, dry_run)
                if result == 'removed':
                    counters['removed'] += 1
                    modified_files.append(file_path)
                    logging.info("  → [REMOVED]")
                elif result == 'not-found':
                    counters['not-found'] += 1
                    logging.info("  → [NOT FOUND] no Apache header to remove")
                elif result == 'dry-run':
                    counters['dry-run'] += 1
                    modified_files.append(file_path)
                    logging.info("  → [DRY-RUN] would remove header")
                else:
                    counters['errors'] += 1
            else:
                result = insert_or_fix_header(file_path, ext, prefix, dry_run)
                if result == 'inserted':
                    counters['inserted'] += 1
                    modified_files.append(file_path)
                    logging.info("  → [INSERTED]")
                elif result == 'fixed':
                    counters['fixed'] += 1
                    modified_files.append(file_path)
                    logging.info("  → [FIXED] corrected malformed/wrong-prefix header")
                elif result == 'skipped':
                    counters['skipped'] += 1
                    logging.info("  → [SKIPPED] correct license present")
                elif result == 'dry-run':
                    counters['dry-run'] += 1
                    modified_files.append(file_path)
                    logging.info("  → [DRY-RUN] would insert/fix license")
                else:
                    counters['errors'] += 1

    elapsed = time.time() - start_time
    logging.info("--- Summary ---")
    for k, v in counters.items():
        logging.info(f"{k.replace('-', ' ').capitalize():<25}: {v}")
    logging.info(f"Elapsed time              : {elapsed:.2f} seconds")

    if report:
        write_markdown_report(counters, modified_files, incorrect_files, correct_files, dry_run, verify)

    return counters


def write_markdown_report(counters: dict, modified_files: List[str],
                          incorrect_files: List[str], correct_files: List[str],
                          dry_run: bool, verify: bool):
    log_dir = os.path.join(os.getcwd(), "..", "admin", "logs")
    report_path = os.path.join(log_dir, "propagate-lic-report.md")
    os.makedirs(os.path.dirname(report_path), exist_ok=True)
    with open(report_path, 'w', encoding='utf-8') as f:
        f.write("# 📝 License Header Propagation Report\n\n")
        f.write(f"**Run mode:** {'Verify' if verify else ('Dry-run' if dry_run else 'Actual insertion/removal')}\n\n")
        f.write("## 📊 Summary\n")
        for k, v in counters.items():
            f.write(f"- {k.replace('-', ' ').capitalize()}: {v}\n")

        if verify:
            f.write("\n## ✅ Correct files\n")
            if correct_files:
                for path in correct_files:
                    f.write(f"- `{path}`\n")
            else:
                f.write("_No correct files found._\n")

            f.write("\n## ❌ Incorrect or missing headers\n")
            if incorrect_files:
                for path in incorrect_files:
                    f.write(f"- `{path}`\n")
            else:
                f.write("_No incorrect files found._\n")
        else:
            f.write("\n## 📁 Modified files\n")
            if modified_files:
                for path in modified_files:
                    f.write(f"- `{path}`\n")
            else:
                f.write("_No files modified._\n")
    logging.info(f"Markdown report written to: {report_path}")


def parse_args():
    parser = argparse.ArgumentParser(
        description="Insert/verify Apache 2.0 license headers into source files. Remove headers inside 'extern' folders."
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
    parser.add_argument(
        "--verify",
        action="store_true",
        help="Verify correctness only; do not modify files. Exits non-zero if any incorrect."
    )
    parser.add_argument(
        "--override",
        action="append",
        metavar="EXT=PREFIX",
        help="Override comment prefix per extension, e.g. .m=%  or  .md=<!-- "
    )
    return parser.parse_args()


def build_override_map(override_args: Optional[List[str]]) -> dict:
    """
    Build override map from CLI args: ['.m=% ', '.md=<!-- '] -> {'.m': '% ', '.md': '<!-- '}
    """
    overrides = {}
    if not override_args:
        return overrides
    for item in override_args:
        try:
            ext, pfx = item.split('=', 1)
            ext = ext.strip()
            pfx = pfx.strip()
            if not ext.startswith('.'):
                logging.warning(f"Ignoring override '{item}': extension must start with '.'")
                continue
            overrides[ext] = pfx
        except ValueError:
            logging.warning(f"Ignoring override '{item}': must be in EXT=PREFIX format")
    return overrides


def main():
    setup_logging()
    args = parse_args()
    override_map = build_override_map(args.override)
    counters = scan(args.root, dry_run=args.dry_run, report=args.report, verify=args.verify, override_map=override_map)
    if args.verify and (counters['incorrect'] > 0 or counters['errors'] > 0):
        # Non-zero exit for CI if verification fails
        sys.exit(1)


if __name__ == "__main__":
    main()
