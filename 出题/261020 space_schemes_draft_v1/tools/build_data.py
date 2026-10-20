#!/usr/bin/env python3
"""Compile std/val/checker, regenerate all private tests and public large samples."""
import argparse
import json
import platform
import subprocess
import time
from pathlib import Path

from common import ROOT, compile_cpp, executable, run_solution, sha256
from gen import generate, write_case


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--build-dir', type=Path, default=ROOT / 'build')
    parser.add_argument('--cxx', default=None)
    parser.add_argument('--only', help='comma-separated case ids; default all')
    args = parser.parse_args()
    build_dir = args.build_dir.resolve()
    warnings = {}
    for name, source in [('std', ROOT / 'solutions/std.cpp'),
                         ('val', ROOT / 'tools/val.cpp'),
                         ('checker', ROOT / 'tools/checker.cpp')]:
        warnings[name] = compile_cpp(source, executable(build_dir, name), args.cxx)
    config = json.loads((ROOT / 'config/cases.json').read_text(encoding='utf-8'))
    selected = set(args.only.split(',')) if args.only else None
    available = {c['id'] for key in ('private_cases', 'large_samples') for c in config[key]}
    if selected and not selected <= available:
        parser.error('unknown case ids: ' + ', '.join(sorted(selected - available)))
    records = []
    started = time.perf_counter()
    for kind, section, directory in [('private', 'private_cases', 'data'),
                                      ('large_sample', 'large_samples', 'samples/large')]:
        for case in config[section]:
            if selected and case['id'] not in selected: continue
            input_path = ROOT / directory / (case['id'] + '.in')
            answer_path = input_path.with_suffix('.ans')
            operations = generate(case['n'], case['q'], case['seed'], case['mode'])
            write_case(input_path, case['n'], operations)
            del operations
            val_cmd = [str(executable(build_dir, 'val')), '--input', str(input_path)]
            if kind == 'private': val_cmd += ['--subtask', str(case['subtask'])]
            validation = subprocess.run(val_cmd, capture_output=True, text=True)
            if validation.returncode:
                raise RuntimeError(case['id'] + ': ' + validation.stderr)
            elapsed = run_solution(executable(build_dir, 'std'), input_path, answer_path)
            check = subprocess.run([str(executable(build_dir, 'checker')), str(input_path),
                                    str(answer_path), str(answer_path)], capture_output=True, text=True)
            if check.returncode: raise RuntimeError(case['id'] + ': ' + check.stderr)
            record = dict(case, kind=kind,
                          input=input_path.relative_to(ROOT).as_posix(),
                          answer=answer_path.relative_to(ROOT).as_posix(),
                          input_bytes=input_path.stat().st_size,
                          answer_bytes=answer_path.stat().st_size,
                          input_sha256=sha256(input_path), answer_sha256=sha256(answer_path),
                          std_seconds=round(elapsed, 6))
            records.append(record)
            print(f"{case['id']:>8}  n={case['n']:6} q={case['q']:6}  "
                  f"{case['mode']:<16} std={elapsed:.3f}s", flush=True)
    manifest_path = ROOT / 'manifest.json'
    generated_count = len(records)
    if selected and manifest_path.exists():
        old = json.loads(manifest_path.read_text(encoding='utf-8'))['cases']
        records += [x for x in old if x['id'] not in selected]
    records.sort(key=lambda x: (x['kind'] != 'private', x['id']))
    manifest = {'version': 'draft-v1', 'date': '2026-10-20',
                'subtasks_file': 'config/subtasks.json', 'cases': records}
    manifest_path.write_text(json.dumps(manifest, ensure_ascii=False, indent=2) + '\n', encoding='utf-8')
    report = {'platform': platform.platform(), 'python': platform.python_version(),
              'compile_warnings': warnings, 'generated_cases_this_run': generated_count,
              'wall_seconds': round(time.perf_counter()-started, 3),
              'max_std_seconds': max(x['std_seconds'] for x in records)}
    report_path = ROOT / 'reports/generation.json'
    report_path.parent.mkdir(parents=True, exist_ok=True)
    report_path.write_text(json.dumps(report, ensure_ascii=False, indent=2) + '\n', encoding='utf-8')
    print(f'Done: {len(records)} cases in manifest; max std {report["max_std_seconds"]:.3f}s')


if __name__ == '__main__':
    main()
