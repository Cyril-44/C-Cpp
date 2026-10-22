#!/usr/bin/env python3
"""Generate, testlib-validate, independently cross-check, then publish 40+3 pairs."""
import argparse
import csv
import hashlib
import json
import platform
import shutil
import subprocess
import time
from pathlib import Path
from check_output import check

ROOT = Path(__file__).resolve().parents[1]

def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

def run(command, inp=None, out=None, timeout=None):
    started = time.perf_counter()
    with (inp.open('rb') if inp else open('/dev/null', 'rb')) as src:
        with (out.open('wb') if out else open('/dev/null', 'wb')) as dst:
            result = subprocess.run(list(map(str, command)), stdin=src, stdout=dst,
                                    stderr=subprocess.PIPE, timeout=timeout)
    if result.returncode:
        raise RuntimeError(f'{command}: {result.stderr.decode(errors="replace")}')
    return round(time.perf_counter() - started, 4)

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--build', required=True, type=Path)
    parser.add_argument('--data', required=True, type=Path)
    parser.add_argument('--down', required=True, type=Path)
    args = parser.parse_args()
    stage = args.build / 'stage'
    stage.mkdir()
    cases = []
    with (ROOT / 'gen/cases.tsv').open() as f:
        for row in csv.reader((line for line in f if not line.startswith('#')), delimiter='\t'):
            if row:
                name, group, n, m, array, profile, seed, purpose = row
                assert name == f'{int(group):02d}_{sum(c[1] == group for c in cases) + 1:02d}'
                cases.append((name, group, n, m, array, profile, seed, purpose))
    assert len(cases) == 40
    samples = [('calibration1', '4', '5000', '5000', 'random', 'mixed', '2026102201', '一般混合'),
               ('calibration2', '5', '50000', '50000', 'edges', 'gc', '2026102202', '共享和覆盖'),
               ('calibration3', '5', '500000', '500000', 'random', 'preserved_seed', '2026102203', '最大规模保留随机种子')]
    report = {'status': 'running', 'platform': platform.platform(), 'time_limit_seconds': 4,
              'data_directory': str(args.data.resolve()), 'large_sample_directory': str(args.down.resolve()),
              'cases': [], 'samples': []}
    for sample, rows in [(False, cases), (True, samples)]:
        for name, group, n, m, array, profile, seed, purpose in rows:
            print(f'Generating {name} n={n} m={m} {profile}', flush=True)
            inp, ans, ref, brute = [stage / (name + ext) for ext in ['.in', '.ans', '.ref', '.brute']]
            run([args.build / 'gen', n, m, group, array, profile, seed], out=inp)
            run([args.build / 'val', '--group', group], inp=inp)
            std_seconds = run([args.build / 'std'], inp, ans, 4)
            avl_seconds = run([args.build / 'avl'], inp, ref, 4)
            answers = check(inp, ans, ref)
            if group == '1':
                run([args.build / 'brute'], inp, brute, 4)
                check(inp, brute, ans)
            row = dict(id=name, group=int(group), n=int(n), m=int(m), queries=answers,
                       profile=profile, seed=seed, purpose=purpose, std_seconds=std_seconds,
                       avl_seconds=avl_seconds, input_bytes=inp.stat().st_size,
                       answer_bytes=ans.stat().st_size, input_sha256=sha(inp), answer_sha256=sha(ans))
            report['samples' if sample else 'cases'].append(row)
    # Publish only declared names after every validation succeeds; preserve all unrelated files.
    for rows, destination in [(cases, args.data), (samples, args.down)]:
        for row in rows:
            for ext in ['.in', '.ans']:
                shutil.copyfile(stage / (row[0] + ext), destination / (row[0] + ext))
    report['status'] = 'passed'
    (ROOT / 'review/generation.json').write_text(json.dumps(report, ensure_ascii=False, indent=2) + '\n')
    print(f'Done: {len(cases)} formal cases, {len(samples)} large samples; all answers match independent AVL.', flush=True)

if __name__ == '__main__':
    main()
