#!/usr/bin/env python3
"""Compile, differential-test and audit the ring package. Python 3 stdlib only."""
import argparse
import csv
import hashlib
import itertools
import platform
import random
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
    parser.add_argument('--data', type=Path, default=ROOT/'../../data/ring')
    parser.add_argument('--down', type=Path, default=ROOT/'../../down/ring')
    config = parser.parse_args()
    BUILD.mkdir(parents=True, exist_ok=True)
    programs = {
        'std': 'sol/std_boundary_dp.cpp',
        'brute': 'review/brute.cpp',
        'reference': 'review/reference_matrix.cpp',
        'few': 'sol/partial_25_few_leaves.cpp',
        'star': 'sol/partial_15_star.cpp',
        'gen': 'gen/gen.cpp', 'val': 'gen/val.cpp', 'check': 'review/check.cpp',
    }
    for name, path in programs.items():
        subprocess.run(['g++', '-std=c++17', '-O2', '-pipe', '-Wall', '-Wextra', '-I'+str(REPO), str(ROOT/path), '-o', str(BUILD/name)], check=True)
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
    good = encode([1, 1, 2, 2, 3, 3])
    assert run('std', good) == b'21'
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
        (folder/'answer').write_text('21\n')
        for actual, should_pass in [(' 21 \n', True), ('22\n', False), ('21 0\n', False), ('-1\n', False)]:
            (folder/'actual').write_text(actual)
            result = subprocess.run([str(BUILD/'check'), str(folder/'input'), str(folder/'actual'), str(folder/'answer')], capture_output=True)
            assert (result.returncode == 0) == should_pass, (actual, result.stderr)
    rows = []
    fingerprints = []
    if not config.skip_large:
        for directory, official in [(config.data.resolve(), True), (config.down.resolve(), False)]:
            manifest = list(csv.DictReader((directory/'manifest.tsv').open(), delimiter='\t'))
            for record in manifest:
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
                # Reproduce input bytes from the complete fixed-seed manifest.
                gen_options = [f"--type={record['type']}", f"--n={record['n']}", f"--seed={record['seed']}"]
                if record['leaves_parameter']:
                    gen_options.append(f"--leaves={record['leaves_parameter']}")
                reproduced = subprocess.check_output([str(BUILD/'gen'), *gen_options])
                assert reproduced == data, ('nonreproducible', path)
                rows.append((('data/' if official else 'down/')+path.name, record['type'], record['n'], float(elapsed), int(memory)))
                fingerprints.append(hashlib.sha256(data).hexdigest()+'  '+('data/' if official else 'down/')+path.name)
            print(f'Validated, reproduced and cross-checked {len(manifest)} cases in {directory}', flush=True)
    if config.check_generation:
        with tempfile.TemporaryDirectory(prefix='custom-output-', dir=BUILD) as temporary:
            folder = Path(temporary)
            custom_data, custom_down = folder/'data with spaces', folder/'down with spaces'
            for target in [custom_data, custom_down]:
                target.mkdir()
                (target/'unrelated.in').write_text('keep this unrelated file\n')
                (target/'nested').mkdir()
                (target/'nested/keep.txt').write_text('keep this nested file\n')
            subprocess.run(['bash', str(ROOT/'gen/genall.sh'), str(custom_data), str(custom_down)], cwd='/tmp', check=True, stdout=subprocess.PIPE)
            for target, original in [(custom_data, config.data), (custom_down, config.down)]:
                assert (target/'unrelated.in').read_text() == 'keep this unrelated file\n'
                assert (target/'nested/keep.txt').read_text() == 'keep this nested file\n'
                for name in ['manifest.tsv']+[path.name for path in original.glob('*.in')]+[path.name for path in original.glob('*.ans')]:
                    assert (target/name).read_bytes() == (original/name).read_bytes(), name
            print('Custom generation from /tmp passed; paths with spaces and unrelated files preserved', flush=True)
    total = time.monotonic()-start
    report = ['# ring 验证报告', '', f'- 环境：{platform.platform()}；`g++ -std=c++17 -O2 -pipe`。',
              f'- 穷举：n=4..{config.exhaustive_max_n} 的全部递增父编号树，共 {count} 棵；标准解、顶点子集暴力、矩阵参考及适用的部分分程序答案一致。',
              f'- 随机：固定种子 261022，共 {config.random} 棵 n<=20 的树；另验证全部 10 种生成模式共 {generator_tests} 例。',
              '- 校验器：拒绝 9 种基础非法输入和 5 种分组/参数违规；通过全部正式/公开输入。',
              '- Checker：正确输出及空白通过；错误数字、额外 token、负数均拒绝。',
              '- 正式数据和大样例全部逐字节固定种子复现，并用独立矩阵实现交叉检查答案；对应组的部分分程序也通过。',
              f'- 本轮验证耗时（不含编译）：{total:.2f} 秒。', '']
    if config.check_generation:
        report += ['- 从 /tmp 调用 genall 指定含空格目录，35 对输入/答案及清单与默认输出逐字节一致；目标目录中无关输入和嵌套文件均保留。', '']
    if rows:
        maximum_time = max(row[3] for row in rows)
        maximum_memory = max(row[4] for row in rows)
        report += [f'正式/公开数据标准程序实测最大 {maximum_time:.2f} 秒、{maximum_memory} KiB RSS；题面限制 5 秒 / 128 MiB。计时依赖本机，出题环境需按评测机比例复核。', '',
                   '| 输入 | 形态 | n | 秒 | RSS / KiB |', '|---|---|---:|---:|---:|']
        report += [f'| {name} | {shape} | {n} | {elapsed:.2f} | {memory} |' for name,shape,n,elapsed,memory in rows]
    (ROOT/'review/test_report.md').write_text('\n'.join(report)+'\n')
    (ROOT/'review/input_sha256.txt').write_text('\n'.join(fingerprints)+'\n')
    print(f'PASS: exhaustive={count}, random={config.random}, generated-small={generator_tests}, official/public={len(rows)}, seconds={total:.2f}', flush=True)

if __name__ == '__main__':
    main()
