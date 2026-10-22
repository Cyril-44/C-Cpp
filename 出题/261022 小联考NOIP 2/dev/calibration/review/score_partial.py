#!/usr/bin/env python3
"""Score compiled partial solutions on the final dataset (default: 1s / 512 MiB)."""
import argparse
import csv
import json
import resource
import subprocess
import tempfile
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
DATA = ROOT / '../../data/calibration'
POINTS = {1: 20, 2: 15, 3: 15, 4: 20, 5: 30}

def limits():
    resource.setrlimit(resource.RLIMIT_AS, (512 * 1024**2, 512 * 1024**2))

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--build', type=Path, default=ROOT / '.build')
    parser.add_argument('--data', type=Path, default=DATA)
    parser.add_argument('--time-limit', type=float, default=1)
    parser.add_argument('--case', action='append', help='restrict to selected case IDs; repeatable')
    args = parser.parse_args()
    if args.time_limit <= 0:
        parser.error('--time-limit must be positive')
    with (ROOT / 'gen/cases.tsv').open() as f:
        cases = list(csv.reader((line for line in f if not line.startswith('#')), delimiter='\t'))
    if args.case:
        selected = set(args.case)
        assert selected <= {row[0] for row in cases}, 'unknown case ID'
        cases = [row for row in cases if row[0] in selected]
    reports = {}
    for name in ['brute', 'partial']:
        report = dict(status='running', time_limit_seconds=args.time_limit, memory_limit_mib=512, groups={}, cases=[])
        with tempfile.TemporaryFile() as output:
            for row in cases:
                case, group = row[0], int(row[1])
                if report['groups'].get(group) is False:
                    report['cases'].append(dict(id=case, status='skipped_after_group_failure'))
                    continue
                start = time.perf_counter()
                output.seek(0); output.truncate()
                try:
                    with (args.data / (case + '.in')).open('rb') as inp:
                        result = subprocess.run([str(args.build.resolve() / name)], stdin=inp, stdout=output,
                                                stderr=subprocess.PIPE, timeout=args.time_limit, preexec_fn=limits)
                    seconds = round(time.perf_counter() - start, 4)
                    if result.returncode:
                        status = 'runtime_error'
                    else:
                        output.seek(0)
                        expected = (args.data / (case + '.ans')).read_bytes().split()
                        status = 'passed' if output.read().split() == expected else 'wrong_answer'
                except subprocess.TimeoutExpired:
                    seconds, status = args.time_limit, 'time_limit'
                report['groups'][group] = report['groups'].get(group, True) and status == 'passed'
                report['cases'].append(dict(id=case, status=status, seconds=seconds))
                print(f'{name}: {case}: {status} {seconds:.3f}s', file=sys.stderr, flush=True)
        report.update(status='finished')
        if args.case:
            report['selected_cases_only'] = True
        else:
            report['score'] = sum(POINTS[g] for g, passed in report['groups'].items() if passed)
        reports[name] = report
    print(json.dumps(reports, ensure_ascii=False, indent=2))

if __name__ == '__main__':
    main()
