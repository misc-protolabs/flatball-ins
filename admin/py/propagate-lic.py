#!/usr/bin/env python3
import os, sys, time, argparse, re
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
    '.md': '*IGNORE*',   # don’t insert, but clean if present
    # .m handled via resolver
}

APACHE_MARKER = "Licensed under the Apache License"
PRUNE_DIRS = {"node_modules", ".git", "__pycache__"}

# ---------- Comment style resolution ----------

def is_matlab_m(file_path: str) -> bool:
    try:
        with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
            head = f.read(4096)
    except Exception:
        return True
    if re.search(r'@interface|@implementation|#import|#include', head):
        return False
    if re.search(r'^\s*%|^\s*%%|^\s*function\b|^\s*classdef\b', head, flags=re.MULTILINE):
        return True
    return True

def resolve_comment_style(file_path: str, ext: str, override: Optional[str]) -> Optional[str]:
    if override:
        return override
    if ext == '.m':
        return '% ' if is_matlab_m(file_path) else '// '
    style = BASE_COMMENT_STYLES.get(ext)
    if style == "*IGNORE*":
        return None
    return style

def format_header(ext: str, prefix: str) -> str:
    lines = LICENSE_HEADER.splitlines()
    if ext == '.md' or (prefix and prefix.startswith('<!--')):
        body = '\n'.join(f"<!-- {line}" for line in lines)
        return f"<!--\n{body}\n-->\n\n"
    return '\n'.join(f"{prefix}{line}" for line in lines) + '\n\n'

# ---------- Header detection ----------

def has_correct_header(content: str, expected: str) -> bool:
    if content.startswith(expected):
        return True
    if content.startswith('#!'):
        nl = content.find('\n')
        if nl != -1:
            return content[nl+1:].startswith(expected)
    return False

def find_license_block_span(content: str) -> Optional[Tuple[int,int]]:
    lines = content.splitlines(keepends=True)[:200]
    text = ''.join(lines)
    if APACHE_MARKER not in text:
        return None
    start = 0
    if lines and lines[0].startswith('#!'):
        start = len(lines[0])
        text = ''.join(lines[1:])
    marker_pos = text.find(APACHE_MARKER)
    if marker_pos == -1:
        return None
    after = text[marker_pos:]
    cut_rel = after.find('\n\n')
    if cut_rel == -1: cut_rel = len(after)
    end_rel = marker_pos + cut_rel
    return (start, start+end_rel)

# ---------- Core operations ----------

def _prepend_with_shebang_preserved(content: str, header: str) -> str:
    if content.startswith('#!'):
        nl = content.find('\n')
        if nl != -1:
            return content[:nl+1] + header + content[nl+1:]
    return header + content

def insert_or_fix(file_path: str, ext: str, prefix: str, dry: bool) -> str:
    try:
        with open(file_path,'r',encoding='utf-8') as f: content=f.read()
    except: return 'error'
    expected = format_header(ext,prefix)
    if has_correct_header(content,expected): return 'skipped'
    span = find_license_block_span(content)
    if span:
        s,e=span
        content_wo_old = content[:s]+content[e:]
        new_content = _prepend_with_shebang_preserved(content_wo_old,expected)
        if dry: return 'dry-run'
        try:
            with open(file_path,'w',encoding='utf-8') as f: f.write(new_content)
            return 'fixed'
        except: return 'error'
    else:
        new_content = _prepend_with_shebang_preserved(content,expected)
        if dry: return 'dry-run'
        try:
            with open(file_path,'w',encoding='utf-8') as f: f.write(new_content)
            return 'inserted'
        except: return 'error'

def remove_if_present(file_path: str, dry: bool) -> str:
    try:
        with open(file_path,'r',encoding='utf-8') as f: content=f.read()
    except: return 'error'
    span = find_license_block_span(content)
    if not span: return 'not-found'
    if dry: return 'dry-run'
    s,e=span
    try:
        with open(file_path,'w',encoding='utf-8') as f: f.write(content[:s]+content[e:])
        return 'removed'
    except: return 'error'

# ---------- Unified Markdown logging ----------

class MdLogger:
    def __init__(self,path:str):
        self._fh=open(path,'w',encoding='utf-8')
    def write(self,text:str): self._fh.write(text)
    def close(self): self._fh.close()

# ---------- Scanner ----------

def scan(root:str,dry:bool,verify:bool,override_map:dict,log_path:str):
    counters={k:0 for k in['scanned','matched','inserted','fixed','removed','skipped','dry-run','errors','incorrect','correct']}
    modified,incorrect,correct=[],[],[]
    logger=MdLogger(log_path)
    start=time.time()
    logger.write(f"# License Header Run Log\n\nMode: {'Verify' if verify else ('Dry-run' if dry else 'Apply')}\n\n")
    for dirpath,dirnames,filenames in os.walk(root):
        dirnames[:]=[d for d in dirnames if d not in PRUNE_DIRS]
        if "node_modules" in dirpath.split(os.sep): continue
        is_extern='extern' in dirpath.split(os.sep)
        for fn in filenames:
            counters['scanned']+=1
            ext=os.path.splitext(fn)[1]
            if ext not in BASE_COMMENT_STYLES and ext!='.m' and ext not in override_map: continue
            counters['matched']+=1
            path=os.path.join(dirpath,fn)
            prefix=resolve_comment_style(path,ext,override_map.get(ext))
            if verify:
                try:
                    with open(path,'r',encoding='utf-8') as f: content=f.read()
                except: counters['errors']+=1; continue
                if prefix is None:
                    # ignore mode: should not have header
                    if find_license_block_span(content):
                        counters['incorrect']+=1; incorrect.append(path); logger.write(f"- INCORRECT (ignored ext has header): {path}\n")
                    else:
                        counters['correct']+=1; correct.append(path)
                else:
                    expected=format_header(ext,prefix)
                    if has_correct_header(content,expected):
                        counters['correct']+=1; correct.append(path)
                    else:
                        counters['incorrect']+=1; incorrect.append(path); logger.write(f"- INCORRECT: {path}\n")
                continue
            # modify mode
            if is_extern or prefix is None:
                res=remove_if_present(path,dry)
            else:
                res=insert_or_fix(path,ext,prefix,dry)
            counters[res]=counters.get(res,0)+1
            if res in('inserted','fixed','removed','dry-run'):
                modified.append(path); logger.write(f"- {res.upper()}: {path}\n")
    elapsed=time.time()-start
    logger.write("\n## Summary\n")
    for k,v in counters.items(): logger.write(f"- {k}: {v}\n")
    logger.write(f"- elapsed: {elapsed:.2f}s\n")
    if verify:
        logger.write("\n## Incorrect files\n")
        if incorrect:
            for p in incorrect:
                logger.write(f"- {p}\n")
        else:
            logger.write("_None_\n")

        logger.write("\n## Correct files\n")
        if correct:
            for p in correct:
                logger.write(f"- {p}\n")
        else:
            logger.write("_None_\n")
    else:
        logger.write("\n## Modified files\n")
        if modified:
            for p in modified:
                logger.write(f"- {p}\n")
        else:
            logger.write("_None_\n")

    logger.close()
    return counters

# ---------- CLI ----------

def parse_args():
    ap = argparse.ArgumentParser(
        description="Insert/verify Apache 2.0 headers. Removes headers in 'extern'. Writes a single Markdown log."
    )
    ap.add_argument("root", nargs="?", default=".", help="Root directory to scan (default: .)")
    ap.add_argument("--dry-run", action="store_true", help="Preview changes without modifying files")
    ap.add_argument("--verify", action="store_true", help="Verify correctness only; no modifications")
    ap.add_argument("--override", action="append", metavar="EXT=PREFIX",
                    help="Override comment prefix per extension (e.g., .m=%  or .md=<!-- )")
    ap.add_argument("--log", metavar="PATH", help="Path to the Markdown log file")
    return ap.parse_args()

def build_override_map(override_args: Optional[List[str]]) -> dict:
    overrides = {}
    if not override_args:
        return overrides
    for item in override_args:
        if '=' not in item:
            continue
        ext, pfx = item.split('=', 1)
        ext = ext.strip()
        pfx = pfx.strip()
        if not ext.startswith('.'):
            continue
        overrides[ext] = pfx
    return overrides

def default_log_path(script_dir: str) -> str:
    # Default to ../admin/logs/propagate-lic-log.md relative to the script file location
    return os.path.abspath(os.path.join(script_dir, "..", "logs", "propagate-lic-log.md"))

def main():
    args = parse_args()
    script_dir = os.path.dirname(os.path.abspath(sys.argv[0]))
    log_path = args.log if args.log else default_log_path(script_dir)
    os.makedirs(os.path.dirname(log_path), exist_ok=True)

    override_map = build_override_map(args.override)
    counters = scan(args.root, dry=args.dry_run, verify=args.verify,
                    override_map=override_map, log_path=log_path)

    if args.verify and (counters['incorrect'] > 0 or counters['errors'] > 0):
        sys.exit(1)

if __name__ == "__main__":
    main()
