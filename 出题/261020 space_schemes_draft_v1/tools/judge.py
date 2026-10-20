#!/usr/bin/env python3
"""Local time/output judge implementing independent all-or-nothing subtasks.

Memory limits are documented for the target judge, not enforced by this portable runner.
"""
import argparse
import json
import subprocess
import tempfile
from pathlib import Path

from common import ROOT, compile_cpp, executable, run_solution


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('program', type=Path)
    parser.add_argument('--build-dir', type=Path, default=ROOT / 'build')
    parser.add_argument('--time-limit', type=float)
    parser.add_argument('--subtasks', help='comma-separated ids; default all')
    parser.add_argument('--report', type=Path)
    args = parser.parse_args()
    settings = json.loads((ROOT / 'config/subtasks.json').read_text(encoding='utf-8'))
    cases = json.loads((ROOT / 'manifest.json').read_text(encoding='utf-8'))['cases']
    limit = args.time_limit or settings['time_limit_seconds']
    selected = set(map(int, args.subtasks.split(','))) if args.subtasks else set(range(1, 11))
    checker = executable(args.build_dir, 'checker')
    if not checker.exists(): compile_cpp(ROOT / 'tools/checker.cpp', checker)
    results = []
    with tempfile.TemporaryDirectory(prefix='space_schemes_judge_') as temp:
        for case in cases:
            if case['kind'] != 'private' or case['subtask'] not in selected: continue
            output = Path(temp) / (case['id'] + '.out')
            elapsed, message = None, ''
            try:
                elapsed = run_solution(args.program, ROOT / case['input'], output, limit)
                checked = subprocess.run([str(checker), str(ROOT / case['input']),
                                          str(output), str(ROOT / case['answer'])],
                                         capture_output=True, text=True)
                verdict = 'AC' if checked.returncode == 0 else ('WA' if checked.returncode == 1 else 'FAIL')
                message = checked.stderr.strip()
            except subprocess.TimeoutExpired:
                verdict = 'TLE'
            except RuntimeError as error:
                verdict, message = 'RE', str(error)
            entry = {'id': case['id'], 'subtask': case['subtask'], 'verdict': verdict,
                     'seconds': elapsed, 'message': message}
            results.append(entry)
            print(f"{case['id']}: {verdict}" + (f' ({elapsed:.3f}s)' if elapsed is not None else ''), flush=True)
    groups = []
    total = 0
    for subtask in settings['subtasks']:
        if subtask['id'] not in selected: continue
        group = [x for x in results if x['subtask'] == subtask['id']]
        score = subtask['score'] if group and all(x['verdict'] == 'AC' for x in group) else 0
        total += score
        groups.append({'id': subtask['id'], 'score': score, 'maximum': subtask['score']})
        print(f"Subtask {subtask['id']}: {score}/{subtask['score']}")
    report = {'score': total, 'subtasks': groups, 'cases': results,
              'time_limit_seconds': limit, 'memory_limit_enforced': False}
    if args.report:
        args.report.parent.mkdir(parents=True, exist_ok=True)
        args.report.write_text(json.dumps(report, ensure_ascii=False, indent=2) + '\n', encoding='utf-8')
    print(f'Total: {total}')
    if any(x['verdict'] == 'FAIL' for x in results): raise SystemExit(2)
    raise SystemExit(0 if all(x['verdict'] == 'AC' for x in results) else 1)


if __name__ == '__main__':
    main()
