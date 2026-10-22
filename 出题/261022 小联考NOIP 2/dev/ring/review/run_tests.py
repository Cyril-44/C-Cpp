#!/usr/bin/env python3
"""Compile, differential-test and audit the ring package. Python 3 stdlib only."""
import argparse
import csv
import itertools
import random
import stat
import subprocess
import tempfile
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / 'review/.build'
REPO = Path(subprocess.check_output(['git', '-C', str(ROOT), 'rev-parse', '--show-toplevel'], text=True).strip())

def run(name, data, expect_ok=True, args=()):
    result = subprocess.run([str(BUILD/name), *args], input=data, stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=10)
    if expect_ok:
        assert result.returncode == 0, (name, result.returncode, result.stderr.decode())
        return result.stdout.strip()
    assert result.returncode != 0, (name, 'unexpected acceptance', data[:150])

def encode(parents):
    return (str(len(parents)+1)+'\n'+' '.join(map(str, parents))+'\n').encode()

def generation_plan(destination):
    with (ROOT/'gen/cases.tsv').open(newline='') as source:
        return [record for record in csv.DictReader(source, delimiter='\t')
                if record['destination'] == destination]

def statement_sample():
    return tuple((ROOT/'review'/f'statement_sample{suffix}').read_bytes()
                 for suffix in ['.in', '.ans'])

def reproduce(record):
    if record['type'] == 'statement':
        data, _ = statement_sample()
        assert int(data.splitlines()[0]) == int(record['n'])
        return data
    options = [f"--type={record['type']}", f"--n={record['n']}", f"--seed={record['seed']}"]
    if record['leaves_parameter'] != '-':
        options.append(f"--leaves={record['leaves_parameter']}")
    return subprocess.check_output([str(BUILD/'gen'), *options])

def check_custom_generation(config):
    with tempfile.TemporaryDirectory(prefix='custom output ', dir=BUILD) as temporary:
        folder = Path(temporary)
        custom_data, custom_down = folder/'data with spaces', folder/'down with spaces'
        for target in [custom_data, custom_down]:
            target.mkdir()
            (target/'unrelated.in').write_text('keep this unrelated file\n')
            (target/'nested').mkdir()
            (target/'nested/keep.txt').write_text('keep this nested file\n')
        subprocess.run(['bash', str(ROOT/'gen/genall.sh'), custom_data.name, custom_down.name],
                       cwd=folder, check=True, stdout=subprocess.PIPE)
        for target, original, destination in [(custom_data, config.data, 'data'), (custom_down, config.down, 'down')]:
            assert (target/'unrelated.in').read_text() == 'keep this unrelated file\n'
            assert (target/'nested/keep.txt').read_text() == 'keep this nested file\n'
            expected_names = {record['file'] for record in generation_plan(destination)}
            expected_names |= {str(Path(name).with_suffix('.ans')) for name in expected_names}
            assert {path.name for path in target.iterdir()} == expected_names | {'unrelated.in', 'nested'}
            for name in expected_names:
                assert (target/name).read_bytes() == (original/name).read_bytes(), name
                if destination == 'down':
                    assert stat.S_IMODE((target/name).stat().st_mode) == 0o664, target/name
    print('Custom generation passed: relative paths with spaces, unrelated files preserved, public permissions 664', flush=True)

def audit_generation(config):
    total = 0
    for destination, directory in [('data', config.data.resolve()), ('down', config.down.resolve())]:
        records = generation_plan(destination)
        expected_names = {record['file'] for record in records}
        expected_names |= {str(Path(name).with_suffix('.ans')) for name in expected_names}
        assert {path.name for path in directory.iterdir()} == expected_names, directory
        for record in records:
            path = directory/record['file']
            data = path.read_bytes()
            answer = path.with_suffix('.ans').read_bytes()
            options = [f"--subtask={record['subtask']}"] if destination == 'data' else []
            assert subprocess.run([str(BUILD/'val'), *options], input=data, capture_output=True).returncode == 0, path
            actual = subprocess.check_output([str(BUILD/'std')], input=data, timeout=10)
            assert actual == answer, ('standard-answer mismatch', path)
            assert reproduce(record) == data, ('nonreproducible', path)
            if record['type'] == 'statement':
                assert (data, answer) == statement_sample(), path
            if destination == 'down':
                assert stat.S_IMODE(path.stat().st_mode) == 0o664, path
                assert stat.S_IMODE(path.with_suffix('.ans').stat().st_mode) == 0o664, path
        total += len(records)
        print(f'Validated and reproduced {len(records)} cases in {directory}; only .in/.ans present', flush=True)
    if config.check_generation:
        check_custom_generation(config)
    print(f'PASS: generation audit, official/public={total}', flush=True)

def check(data, use_few=False, use_star=False):
    expected = run('std', data)
    assert run('brute', data) == expected, ('brute mismatch', data.decode())
    assert run('reference', data) == expected, ('matrix mismatch', data.decode())
    if use_few:
        assert run('few', data) == expected, ('few-leaf mismatch', data.decode())
    if use_star:
        assert run('star', data) == expected, ('star mismatch', data.decode())

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--exhaustive-max-n', type=int, default=8)
    parser.add_argument('--random', type=int, default=400)
    parser.add_argument('--skip-large', action='store_true')
    parser.add_argument('--check-generation', action='store_true', help='also test custom output paths and unrelated-file preservation')
    parser.add_argument('--generation-only', action='store_true', help='audit the planned generated files without algorithm differential tests')
    parser.add_argument('--data', type=Path, default=ROOT/'../../data/ring')
    parser.add_argument('--down', type=Path, default=ROOT/'../../down/ring')
    config = parser.parse_args()
    BUILD.mkdir(parents=True, exist_ok=True)
    programs = {
        'std': 'sol/std_boundary_dp.cpp',
        'brute': 'sol/partial_15_bruteforce.cpp',
        'reference': 'review/reference_matrix.cpp',
        'few': 'sol/partial_25_few_leaves.cpp',
        'star': 'sol/partial_15_star.cpp',
        'gen': 'gen/gen.cpp', 'val': 'gen/val.cpp', 'check': 'review/check.cpp',
    }
    if config.generation_only:
        programs = {name: path for name, path in programs.items() if name in ['std', 'gen', 'val']}
    for name, path in programs.items():
        subprocess.run(['g++', '-std=c++17', '-O2', '-pipe', '-Wall', '-Wextra', '-I'+str(REPO), str(ROOT/path), '-o', str(BUILD/name)], check=True)
    if config.generation_only:
        audit_generation(config)
        return
    start = time.monotonic()
    count = 0
    for n in range(4, config.exhaustive_max_n+1):
        for parents in itertools.product(*(range(1, u) for u in range(2, n+1))):
            check(encode(parents), use_few=True, use_star=all(p == 1 for p in parents))
            count += 1
        print(f'Exhaustive n <= {n}: {count} trees passed', flush=True)
    rng = random.Random(261022)
    for test in range(config.random):
        n = rng.randint(4, 20 if test < 40 else 17)
        parents = [rng.randrange(1, u) for u in range(2, n+1)]
        leaves = n - len(set(parents))
        check(encode(parents), use_few=leaves <= 8, use_star=all(p == 1 for p in parents))
    # Every generator mode, particularly randomized DFS leaf order and odd combs.
    generator_tests = 0
    for shape in ['chain', 'two', 'star', 'balanced', 'random', 'comb', 'broom', 'binary_random', 'few', 'few_random']:
        for n in [4, 15, 19, 20]:
            leaves = min(8, (n+1)//2)
            options = [f'--type={shape}', f'--n={n}', '--seed=261022']
            if shape in ['few', 'few_random']:
                options.append(f'--leaves={leaves}')
            data = run('gen', None, args=options)
            # strip() above removed the final newline; restore strict format.
            data += b'\n'
            result = subprocess.run([str(BUILD/'val')], input=data, capture_output=True)
            assert result.returncode == 0, result.stderr.decode()
            check(data, use_few=(shape in ['chain', 'two', 'few', 'few_random']), use_star=shape == 'star')
            generator_tests += 1
    good, sample_answer = statement_sample()
    run('val', good)
    assert run('std', good) == sample_answer.strip(), 'statement sample answer mismatch'
    invalid = [b'3\n1 2\n', b'1000001\n', b'4\n0 1 1\n', b'4\n2 1 1\n',
               b'4\n1 1\n', b'4\n1 1 1\nextra\n', b'4\n1 1 1 2\n', b'4\n1 1 1', b'4\n1  1 1\n']
    for data in invalid:
        run('val', data, expect_ok=False)
    run('val', encode([1]*20), expect_ok=False, args=['--subtask=1'])
    run('val', encode([1]*9), expect_ok=False, args=['--subtask=2'])
    run('val', encode(list(range(1, 100001))), expect_ok=False, args=['--subtask=2'])
    run('val', encode([1, 2, 3]), expect_ok=False, args=['--subtask=3'])
    run('val', good, expect_ok=False, args=['--subtask=0'])
    # Checker accepts whitespace but rejects an incorrect integer or extra token.
    with tempfile.TemporaryDirectory(dir=BUILD) as temporary:
        folder = Path(temporary)
        (folder/'input').write_bytes(good)
        (folder/'answer').write_bytes(sample_answer)
        answer = sample_answer.strip().decode()
        wrong = (int(answer) + 1) % 1000000007
        for actual, should_pass in [(f' {answer} \n', True), (f'{wrong}\n', False), (f'{answer} 0\n', False), ('-1\n', False)]:
            (folder/'actual').write_text(actual)
            result = subprocess.run([str(BUILD/'check'), str(folder/'input'), str(folder/'actual'), str(folder/'answer')], capture_output=True)
            assert (result.returncode == 0) == should_pass, (actual, result.stderr)
    rows = []
    if not config.skip_large:
        for directory, official in [(config.data.resolve(), True), (config.down.resolve(), False)]:
            records = generation_plan('data' if official else 'down')
            for record in records:
                path = directory/record['file']
                data = path.read_bytes()
                expected = path.with_suffix('.ans').read_bytes().strip()
                options = [f"--subtask={record['subtask']}"] if official else []
                assert subprocess.run([str(BUILD/'val'), *options], input=data, capture_output=True).returncode == 0
                with path.open('rb') as input_file:
                    timing = BUILD/'timing.txt'
                    result = subprocess.run(['/usr/bin/time', '-f', '%e %M', '-o', str(timing), str(BUILD/'std')], stdin=input_file, capture_output=True, timeout=5)
                assert result.returncode == 0 and result.stdout.strip() == expected, path
                elapsed, memory = timing.read_text().split()
                assert run('reference', data) == expected, ('large reference mismatch', path)
                if official and record['subtask'] == '1':
                    assert run('brute', data) == expected
                if official and record['subtask'] == '2':
                    assert run('few', data) == expected
                if official and record['subtask'] == '3':
                    assert run('star', data) == expected
                # Generation parameters stay with the generator; the statement prefix is copied verbatim.
                assert reproduce(record) == data, ('nonreproducible', path)
                if record['type'] == 'statement':
                    assert (data, path.with_suffix('.ans').read_bytes()) == statement_sample(), path
                rows.append((('data/' if official else 'down/')+path.name, record['type'], record['n'], float(elapsed), int(memory)))
            print(f'Validated, reproduced and cross-checked {len(records)} cases in {directory}', flush=True)
    if config.check_generation:
        check_custom_generation(config)
    total = time.monotonic()-start
    if rows:
        print(f'Standard maximum: {max(row[3] for row in rows):.2f} seconds, {max(row[4] for row in rows)} KiB RSS', flush=True)
    print(f'PASS: exhaustive={count}, random={config.random}, generated-small={generator_tests}, official/public={len(rows)}, seconds={total:.2f}', flush=True)

if __name__ == '__main__':
    main()
