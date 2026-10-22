#!/usr/bin/env python3
"""Build, validate, cross-check, and publish 40 formal cases and 6 public samples."""
import argparse
import csv
import hashlib
import json
import os
import platform
import re
import shlex
import shutil
import subprocess
import tempfile
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
CONTEST = ROOT.parents[1]
CHECK_TIMEOUT = 4
P = 998244353


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run(command, inp=None, out=None, timeout=None):
    started = time.perf_counter()
    with (inp.open('rb') if inp else open(os.devnull, 'rb')) as src:
        with (out.open('wb') if out else open(os.devnull, 'wb')) as dst:
            result = subprocess.run(list(map(str, command)), stdin=src, stdout=dst,
                                    stderr=subprocess.PIPE, timeout=timeout)
    if result.returncode:
        raise RuntimeError(f'{command}: {result.stderr.decode(errors="replace")}')
    return round(time.perf_counter() - started, 4)


def check(inp, out, expected=None):
    """Require one canonical residue per query and compare all answer values."""
    with inp.open() as f:
        n = int(f.readline())
        assert len(f.readline().split()) == n
        m = int(f.readline())
        queries = sum(f.readline().split()[0] == '3' for _ in range(m))
        assert not f.read().strip()
    tokens = out.read_text().split()
    assert len(tokens) == queries, f'{out}: expected {queries} answers, got {len(tokens)}'
    values = []
    for token in tokens:
        assert token.isascii() and token.isdigit(), f'{out}: noncanonical integer {token!r}'
        value = int(token)
        assert 0 <= value < P, f'{out}: answer outside [0,P)'
        values.append(value)
    if expected is not None:
        assert values == list(map(int, expected.read_text().split())), f'{out}: differs from {expected}'
    return queries


def statement_samples():
    """Retain the statement's exact input/output block contents, in section order."""
    text = (ROOT / 'statement.md').read_bytes().decode('utf-8')
    sections = re.findall(r'^## 样例[^\n]*\n(.*?)(?=^## |\Z)', text, re.M | re.S)
    assert len(sections) == 3, 'statement.md must contain exactly three sample sections'
    samples = []
    for index, section in enumerate(sections, 1):
        blocks = re.findall(r'^```text[ \t]*\r?\n(.*?)^```[ \t]*\r?$', section, re.M | re.S)
        assert len(blocks) == 2, f'statement sample {index} must contain input and output blocks'
        samples.append((f'calibration{index}', blocks[0].encode('utf-8'), blocks[1].encode('utf-8')))
    return samples


def compile_tools(build):
    repo = Path(subprocess.check_output(
        ['git', '-C', str(ROOT), 'rev-parse', '--show-toplevel'], text=True).strip())
    assert (repo / 'testlib.h').is_file(), f'Missing Git-root testlib.h: {repo / "testlib.h"}'
    cxx = shlex.split(os.environ.get('CXX', 'g++'))
    for name, source, flags in [
        ('gen', 'gen/gen.cpp', ['-I', str(repo)]),
        ('val', 'gen/val.cpp', ['-I', str(repo)]),
        ('std', 'sol/std_wblt.cpp', ['-DNDEBUG']),
        ('avl', 'sol/std_avl.cpp', ['-DNDEBUG']),
        ('brute', 'sol/partial_brute.cpp', ['-DNDEBUG']),
    ]:
        print(f'Compiling {source}', flush=True)
        run(cxx + ['-std=c++17', '-O2'] + flags + [ROOT / source, '-o', build / name])


def generate(build, data, down, report_path=None):
    stage = build / 'stage'
    stage.mkdir()
    cases = []
    with (ROOT / 'gen/cases.tsv').open() as f:
        for row in csv.reader((line for line in f if not line.startswith('#')), delimiter='\t'):
            if row:
                name, group, n, m, array, profile, seed, purpose = row
                assert name == f'{int(group):02d}_{sum(c[1] == group for c in cases) + 1:02d}'
                cases.append((name, group, n, m, array, profile, seed, purpose))
    assert len(cases) == 40
    large_samples = [
        ('calibration4', '4', '5000', '5000', 'random', 'mixed', '2026102201', '一般混合'),
        ('calibration5', '5', '50000', '50000', 'edges', 'gc', '2026102202', '共享和覆盖'),
        ('calibration6', '5', '100000', '100000', 'random', 'preserved_seed', '2026102203', '最大规模保留随机种子'),
    ]
    report = {
        'status': 'running', 'platform': platform.platform(),
        'cross_check_timeout_seconds': CHECK_TIMEOUT,
        'data_directory': str(data), 'sample_directory': str(down),
        'formal_cases': len(cases), 'statement_samples': 3, 'large_samples': len(large_samples),
        'cases': [], 'samples': [],
    }
    public_names = []
    for name, input_bytes, answer_bytes in statement_samples():
        print(f'Checking {name} from statement.md', flush=True)
        inp, ans, std, avl, brute = [stage / (name + ext) for ext in ['.in', '.ans', '.std', '.ref', '.brute']]
        inp.write_bytes(input_bytes)
        ans.write_bytes(answer_bytes)
        run([build / 'val', '--group', '1'], inp=inp)
        std_seconds = run([build / 'std'], inp, std, CHECK_TIMEOUT)
        avl_seconds = run([build / 'avl'], inp, avl, CHECK_TIMEOUT)
        queries = check(inp, ans, std)
        check(inp, avl, ans)
        run([build / 'brute'], inp, brute, CHECK_TIMEOUT)
        check(inp, brute, ans)
        lines = input_bytes.splitlines()
        row = dict(id=name, kind='statement', statement_sample=len(public_names) + 1,
                   group=1, n=int(lines[0]), m=int(lines[2]), queries=queries,
                   std_seconds=std_seconds, avl_seconds=avl_seconds,
                   input_bytes=len(input_bytes), answer_bytes=len(answer_bytes),
                   input_sha256=sha(inp), answer_sha256=sha(ans))
        report['samples'].append(row)
        public_names.append(name)
    for sample, rows in [(False, cases), (True, large_samples)]:
        for name, group, n, m, array, profile, seed, purpose in rows:
            print(f'Generating {name} n={n} m={m} {profile}', flush=True)
            inp, ans, ref, brute = [stage / (name + ext) for ext in ['.in', '.ans', '.ref', '.brute']]
            run([build / 'gen', n, m, group, array, profile, seed], out=inp)
            run([build / 'val', '--group', group], inp=inp)
            std_seconds = run([build / 'std'], inp, ans, CHECK_TIMEOUT)
            avl_seconds = run([build / 'avl'], inp, ref, CHECK_TIMEOUT)
            queries = check(inp, ans, ref)
            if group == '1':
                run([build / 'brute'], inp, brute, CHECK_TIMEOUT)
                check(inp, brute, ans)
            row = dict(id=name, group=int(group), n=int(n), m=int(m), queries=queries,
                       profile=profile, seed=seed, purpose=purpose, std_seconds=std_seconds,
                       avl_seconds=avl_seconds, input_bytes=inp.stat().st_size,
                       answer_bytes=ans.stat().st_size, input_sha256=sha(inp), answer_sha256=sha(ans))
            if sample:
                row['kind'] = 'large'
                public_names.append(name)
            report['samples' if sample else 'cases'].append(row)
    # Publish only declared names after every validation succeeds; preserve unrelated files.
    for names, destination, public in [([row[0] for row in cases], data, False), (public_names, down, True)]:
        destination.mkdir(parents=True, exist_ok=True)
        for name in names:
            for ext in ['.in', '.ans']:
                published = destination / (name + ext)
                shutil.copyfile(stage / (name + ext), published)
                if public:
                    published.chmod(0o664)
    report['status'] = 'passed'
    if report_path is not None:
        report_path.parent.mkdir(parents=True, exist_ok=True)
        report_path.write_text(json.dumps(report, ensure_ascii=False, indent=2) + '\n')
    print(f'Done: {len(cases)} formal cases, 3 statement samples, {len(large_samples)} large samples; all answers match independent AVL.', flush=True)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('data_dir', nargs='?', type=Path, default=CONTEST / 'data/calibration')
    parser.add_argument('down_dir', nargs='?', type=Path, default=CONTEST / 'down/calibration')
    parser.add_argument('--report', type=Path, help='optional JSON summary path; no report file by default')
    args = parser.parse_args()
    data, down = args.data_dir.resolve(), args.down_dir.resolve()
    with tempfile.TemporaryDirectory(prefix='.genall.', dir=ROOT) as temporary:
        build = Path(temporary)
        compile_tools(build)
        generate(build, data, down, args.report)


if __name__ == '__main__':
    main()
