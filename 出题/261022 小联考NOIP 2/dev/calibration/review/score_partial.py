#!/usr/bin/env python3
"""Score the shipped partial solutions on the final dataset under 4s / 512 MiB."""
import csv
import json
import resource
import subprocess
import tempfile
import time
from pathlib import Path
from check_output import check

ROOT = Path(__file__).resolve().parents[1]
DATA = ROOT / '../../data/calibration'
POINTS = {1: 20, 2: 15, 3: 15, 4: 20, 5: 30}

def limits():
    resource.setrlimit(resource.RLIMIT_AS, (512 * 1024**2, 512 * 1024**2))

def main():
    with (ROOT / 'gen/cases.tsv').open() as f:
        cases = list(csv.reader((line for line in f if not line.startswith('#')), delimiter='\t'))
    for name in ['brute', 'partial']:
        report = dict(status='running', time_limit_seconds=4, memory_limit_mib=512, groups={}, cases=[])
        with tempfile.TemporaryFile() as output:
            for row in cases:
                case, group = row[0], int(row[1])
                if report['groups'].get(group) is False:
                    report['cases'].append(dict(id=case, status='skipped_after_group_failure'))
                    continue
                start = time.perf_counter()
                output.seek(0); output.truncate()
                try:
                    with (DATA / (case + '.in')).open('rb') as inp:
                        result = subprocess.run([str(ROOT / '.build' / name)], stdin=inp, stdout=output,
                                                stderr=subprocess.PIPE, timeout=4, preexec_fn=limits)
                    seconds = round(time.perf_counter() - start, 4)
                    if result.returncode:
                        status = 'runtime_error'
                    else:
                        output.seek(0)
                        expected = (DATA / (case + '.ans')).read_bytes().split()
                        status = 'passed' if output.read().split() == expected else 'wrong_answer'
                except subprocess.TimeoutExpired:
                    seconds, status = 4, 'time_limit'
                report['groups'][group] = report['groups'].get(group, True) and status == 'passed'
                report['cases'].append(dict(id=case, status=status, seconds=seconds))
                print(f'{name}: {case}: {status} {seconds:.3f}s', flush=True)
        report.update(status='passed', score=sum(POINTS[g] for g, passed in report['groups'].items() if passed))
        (ROOT / 'review' / f'score_{name}.json').write_text(json.dumps(report, indent=2) + '\n')

if __name__ == '__main__':
    main()
