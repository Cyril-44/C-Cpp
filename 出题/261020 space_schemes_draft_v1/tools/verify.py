#!/usr/bin/env python3
"""Rebuild and verify the full package against independent and restricted oracles."""
import argparse
import importlib.util
import json
import shutil
import subprocess
import tempfile
import time
from pathlib import Path

from common import ROOT, compile_cpp, executable, run_solution, sha256
from gen import generate, write_case


def same_answers(first, second):
    # Token comparison deliberately ignores line endings and harmless whitespace.
    return Path(first).read_bytes().split() == Path(second).read_bytes().split()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--build-dir', type=Path, default=ROOT / 'build')
    parser.add_argument('--cxx', default=None)
    args = parser.parse_args()
    build_dir = args.build_dir.resolve()
    sources = {'std': 'solutions/std.cpp', 'brute': 'solutions/brute.cpp',
               'recompute': 'solutions/recompute.cpp',
               'whole_singleton': 'solutions/whole_singleton.cpp',
               'short': 'solutions/short.cpp', 'val': 'tools/val.cpp',
               'checker': 'tools/checker.cpp', 'snapshot_oracle': 'tools/snapshot_oracle.cpp'}
    warnings = {name: compile_cpp(ROOT / source, executable(build_dir, name), args.cxx)
                for name, source in sources.items()}
    reports_dir = ROOT / 'reports'
    reports_dir.mkdir(exist_ok=True)
    start = time.perf_counter()
    for name, report_name in [('verify_judge_tools', 'judge_tools_report.json'),
                              ('verify_snapshot', 'snapshot_report.json')]:
        path = ROOT / 'tools/tests' / (name + '.py')
        spec = importlib.util.spec_from_file_location(name, path)
        module = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(module)
        module.ROOT = build_dir
        module.main()
        shutil.copyfile(build_dir / report_name, reports_dir / report_name)

    cross_checks = []
    with tempfile.TemporaryDirectory(prefix='package_verify_', dir=build_dir) as directory:
        temp = Path(directory)
        input_path, actual, expected = temp / 'case.in', temp / 'std.out', temp / 'oracle.out'
        for n in range(1, 6):
            q = 1 << (n * (n + 1) // 2)
            write_case(input_path, n, generate(n, q, 20261020 + n, 'gray'))
            run_solution(executable(build_dir, 'std'), input_path, actual)
            run_solution(executable(build_dir, 'brute'), input_path, expected)
            if not same_answers(actual, expected): raise AssertionError(f'exhaustive n={n}')
            cross_checks.append({'type': 'all_request_subsets', 'n': n, 'q': q})
        for n in (6, 7, 8):
            for seed in range(5):
                q = 1200
                write_case(input_path, n, generate(n, q, 17564000 + n*10 + seed, 'random'))
                run_solution(executable(build_dir, 'std'), input_path, actual)
                run_solution(executable(build_dir, 'brute'), input_path, expected)
                if not same_answers(actual, expected): raise AssertionError(f'random n={n},seed={seed}')
                cross_checks.append({'type': 'random_brute', 'n': n, 'q': q, 'seed': seed})
        run_solution(executable(build_dir, 'std'), ROOT / 'samples/sample1.in', actual)
        if not same_answers(actual, ROOT / 'samples/sample1.ans'): raise AssertionError('sample1')
        print(f'PASS: {sum(x["q"] for x in cross_checks)} operations checked against exhaustive P enumeration', flush=True)

        records = json.loads((ROOT / 'manifest.json').read_text(encoding='utf-8'))['cases']
        case_checks = []
        for case in records:
            inp, ans = ROOT / case['input'], ROOT / case['answer']
            if sha256(inp) != case['input_sha256'] or sha256(ans) != case['answer_sha256']:
                raise AssertionError('hash mismatch: ' + case['id'])
            command = [str(executable(build_dir, 'val')), '--input', str(inp)]
            if case['kind'] == 'private': command += ['--subtask', str(case['subtask'])]
            result = subprocess.run(command, capture_output=True, text=True)
            if result.returncode: raise AssertionError(case['id'] + ': ' + result.stderr)
            result = subprocess.run([str(executable(build_dir, 'snapshot_oracle')), str(inp), str(ans), '40'],
                                    capture_output=True, text=True, timeout=60)
            if result.returncode: raise AssertionError(case['id'] + ': ' + result.stderr)
            used = ['snapshot_recompute']
            if case['n'] * case['q'] <= 2000000:
                run_solution(executable(build_dir, 'recompute'), inp, expected, timeout=60)
                if not same_answers(expected, ans): raise AssertionError('full recompute: ' + case['id'])
                used.append('full_recompute')
            if case.get('subtask') in (6, 7):
                name = 'whole_singleton' if case['subtask'] == 6 else 'short'
                run_solution(executable(build_dir, name), inp, expected)
                if not same_answers(expected, ans): raise AssertionError('restricted oracle: ' + case['id'])
                used.append(name)
            case_checks.append({'id': case['id'], 'oracles': used,
                                'snapshot_message': (result.stdout + result.stderr).strip()})
            print(f"PASS {case['id']}: {', '.join(used)}", flush=True)
    report = {'status': 'passed', 'date': '2026-10-20', 'compile_warnings': warnings,
              'exhaustive_cross_checks': cross_checks,
              'brute_compared_operations': sum(x['q'] for x in cross_checks),
              'case_checks': case_checks, 'wall_seconds': round(time.perf_counter()-start, 3)}
    (reports_dir / 'verification.json').write_text(json.dumps(report, ensure_ascii=False, indent=2) + '\n', encoding='utf-8')
    print(f'PASS: {len(case_checks)} data/large-sample files verified')


if __name__ == '__main__':
    main()
