#!/usr/bin/env python3
"""Check every published file against manifest.json (standard library only)."""
import argparse
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]

def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--skip-data', action='store_true', help='verify the source-only archive')
    args = ap.parse_args()
    manifest = json.loads((ROOT/'manifest.json').read_text(encoding='utf-8'))
    count = 0
    for item in manifest['files']:
        relative = Path(item['path'])
        if relative.is_absolute() or '..' in relative.parts:
            raise SystemExit('Unsafe manifest path')
        if args.skip_data and relative.parts[0]=='data' and relative.suffix in ('.in','.ans'):
            continue
        file = ROOT/relative
        if not file.is_file() or file.stat().st_size != item['bytes']:
            raise SystemExit(f'Missing file or size mismatch: {relative}')
        digest = hashlib.sha256()
        with file.open('rb') as source:
            for chunk in iter(lambda:source.read(1<<20),b''):
                digest.update(chunk)
        if digest.hexdigest()!=item['sha256']:
            raise SystemExit(f'SHA-256 mismatch: {relative}')
        count += 1
    print(f'PASS: {count} files; all sizes and SHA-256 hashes agree')

if __name__=='__main__':
    main()
