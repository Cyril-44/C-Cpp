#!/usr/bin/env python3
"""Validate official inputs, produce/check answers, and cross-check executables."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import platform
import re
import subprocess
import sys
from checker import check
from runner import run_measured

ROOT = Path(__file__).resolve().parents[1]

def digest(path):
    h = hashlib.sha256()
    with open(path, 'rb') as f:
        for chunk in iter(lambda: f.read(1 << 20), b''):
            h.update(chunk)
    return h.hexdigest()

def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--std', required=True, type=Path)
    ap.add_argument('--validator', required=True, type=Path)
    ap.add_argument('--reference', type=Path)
    ap.add_argument('--brute', type=Path)
    ap.add_argument('--partial', type=Path)
    ap.add_argument('--data-dir', type=Path, default=ROOT/'data')
    ap.add_argument('--scratch', required=True, type=Path)
    ap.add_argument('--report', type=Path, default=ROOT/'audit'/'full_verification.json')
    ap.add_argument('--case', action='append')
    ap.add_argument('--timeout', type=float, default=120)
    ap.add_argument('--memory-mb', type=int, default=512)
    ap.add_argument('--write-answers', action='store_true')
    args = ap.parse_args()
    if args.write_answers and args.reference is None:
        ap.error('--write-answers requires --reference for an independent all-case cross-check')
    manifest = json.loads((args.data_dir/'generation_manifest.json').read_text(encoding='utf-8'))
    cases = manifest['cases']
    if args.case:
        cases = [c for c in cases if c['id'] in args.case]
        if len(cases) != len(set(args.case)):
            ap.error('unknown or duplicate --case selection')
    args.scratch.mkdir(parents=True, exist_ok=True)
    report = {'platform': platform.platform(), 'python': platform.python_version(),
              'timeout_seconds': args.timeout, 'memory_limit_mib': args.memory_mb,
              'cross_check': bool(args.reference), 'cases': []}
    args.report.parent.mkdir(parents=True, exist_ok=True)
    for c in cases:
        case = {**c}; name = c['id']; src = args.data_dir/c['input']
        answer = args.data_dir/(name+'.ans')
        with open(src, 'rb') as f:
            valid = subprocess.run([str(args.validator.resolve()), '--group', str(c['group']),
                                    '--max-n', str(manifest['limits']['n_max']),
                                    '--max-m', str(manifest['limits']['m_max']),
                                    '--small-max', str(manifest['limits']['small_max']),
                                    '--medium-max', str(manifest['limits']['medium_max'])],
                                   stdin=f, capture_output=True, timeout=60)
        case['validator'] = {'returncode': valid.returncode,
                             'message': valid.stderr.decode('utf-8', errors='replace').strip()}
        if valid.returncode:
            raise SystemExit(f'INVALID {name}: {case["validator"]}')
        out = args.scratch/(name+'.std.out')
        case['std'] = run_measured([args.std.resolve()], src, out, args.timeout, args.memory_mb)
        if case['std']['status'] != 'OK':
            raise SystemExit(f'STD FAILED {name}: {case["std"]}')
        expected = out if args.write_answers else answer
        if not expected.exists():
            raise SystemExit(f'Missing answer: {answer}; use --write-answers to create it')
        case['answers'] = check(out, expected)
        query_count = re.search(r'queries=([0-9]+)', case['validator']['message'])
        if query_count is None or case['answers'] != int(query_count.group(1)):
            raise SystemExit(f'Wrong output count for {name}: validator={query_count}, answers={case["answers"]}')
        for label, executable in [('reference', args.reference), ('brute', args.brute), ('partial', args.partial)]:
            if executable is None or (label == 'brute' and c['group'] != 1) or (label == 'partial' and c['group'] > 3):
                continue
            alt = args.scratch/(name+'.'+label+'.out')
            result = run_measured([executable.resolve()], src, alt, args.timeout, args.memory_mb)
            if result['status'] == 'OK':
                try:
                    check(alt, expected)
                except ValueError as exc:
                    result['status'] = 'WA'; result['mismatch'] = str(exc)
            case[label] = result
            if result['status'] != 'OK':
                report['cases'].append(case)
                args.report.write_text(json.dumps(report, ensure_ascii=False, indent=2)+'\n', encoding='utf-8')
                raise SystemExit(f'{label.upper()} FAILED {name}: {result}')
        if args.write_answers:
            # Publish only after the independent implementation has agreed.
            # Canonical LF answers have identical hashes on Windows and Linux.
            temp_answer = answer.with_suffix('.ans.tmp')
            with open(out, 'rb') as src_out, open(temp_answer, 'wb') as dst_answer:
                for line in src_out:
                    dst_answer.write(line.rstrip(b'\r\n')+b'\n')
            temp_answer.replace(answer)
        case['input_bytes'] = src.stat().st_size
        case['answer_bytes'] = answer.stat().st_size
        case['input_sha256'] = digest(src); case['answer_sha256'] = digest(answer)
        report['cases'].append(case)
        args.report.write_text(json.dumps(report, ensure_ascii=False, indent=2)+'\n', encoding='utf-8')
        print(f'PASS {name}: std={case["std"]["wall_seconds"]:.3f}s '
              f'memory={case["std"]["peak_working_set_bytes"]/1048576:.1f}MiB answers={case["answers"]}', flush=True)
    report['all_passed'] = True
    report['total_cases'] = len(report['cases'])
    report['max_std_seconds'] = max(c['std']['wall_seconds'] for c in report['cases'])
    report['max_std_working_set_bytes'] = max(c['std']['peak_working_set_bytes'] for c in report['cases'])
    report['total_input_bytes'] = sum(c['input_bytes'] for c in report['cases'])
    report['total_answer_bytes'] = sum(c['answer_bytes'] for c in report['cases'])
    args.report.write_text(json.dumps(report, ensure_ascii=False, indent=2)+'\n', encoding='utf-8')
    print('ALL PASSED:', len(cases), 'cases')

if __name__ == '__main__':
    main()
