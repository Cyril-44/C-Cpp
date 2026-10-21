#!/usr/bin/env python3
"""Compile the package with an explicitly selectable C++17 compiler."""
import argparse
import os
from pathlib import Path
import shutil
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]

def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--cxx', default=os.environ.get('CXX', 'g++'))
    ap.add_argument('--out', type=Path, default=ROOT/'build')
    ap.add_argument('--audit', action='store_true')
    ap.add_argument('--no-gc', action='store_true')
    args = ap.parse_args()
    args.out.mkdir(parents=True, exist_ok=True)
    exe = '.exe' if os.name == 'nt' else ''
    targets = [('std', 'solutions/std.cpp', ['-DNDEBUG', '-DPROFILE']),
               ('reference_avl', 'solutions/reference_avl.cpp', ['-DNDEBUG', '-DPROFILE']),
               ('brute', 'solutions/brute.cpp', []),
               ('validator', 'tools/validator.cpp', []),
               ('partial_special', 'solutions/partial_special.cpp', [])]
    if args.audit:
        targets += [('std_audit', 'solutions/std.cpp', ['-DAUDIT', '-DPROFILE']),
                    ('reference_avl_audit', 'solutions/reference_avl.cpp', ['-DAVL_AUDIT', '-DPROFILE']),
                    ('wblt_stress', 'tools/wblt_stress.cpp', ['-DAUDIT']),
                    ('wblt_exhaustive', 'tools/wblt_exhaustive.cpp', ['-DAUDIT'])]
    if args.no_gc:
        targets += [('std_no_gc', 'solutions/std.cpp', ['-DNDEBUG', '-DPROFILE', '-DNO_GC'])]
    for name, source, flags in targets:
        src = ROOT/source
        if not src.exists():
            raise SystemExit(f'Missing package source: {src}')
        cmd = [args.cxx, '-std=c++17', '-O2', '-Wall', '-Wextra', *flags, str(src), '-o', str(args.out/(name+exe))]
        subprocess.run(cmd, check=True)
        print('built', name, flush=True)
    return 0

if __name__ == '__main__':
    sys.exit(main())
