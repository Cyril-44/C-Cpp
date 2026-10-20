#!/usr/bin/env python3
"""Compile typical wrong solutions and confirm that the private data rejects them."""
import argparse
import json
import subprocess
import tempfile
from pathlib import Path

from common import ROOT, compile_cpp, executable, run_solution


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--build-dir', type=Path, default=ROOT / 'build')
    parser.add_argument('--cxx', default=None)
    args = parser.parse_args()
    build_dir = args.build_dir.resolve()
    source = (ROOT / 'solutions/std.cpp').read_text(encoding='utf-8')
    changes = [
        ('ways_always_one', "output.integer(solver.root().a_ways, '\\n');", "output.integer(1, '\\n');"),
        ('boolean_marks', 'tree[p].cnt += delta;', 'tree[p].cnt = (delta > 0);'),
        ('drop_tied_branch', 'v.b_ways = base_ways + split_ways;', 'v.b_ways = base_ways;'),
        ('wrong_odd_split', '(l + r) / 2', '(l + r - 1) / 2'),
        ('overflow_32bit_product', '1LL * left.', '1u * left.'),
        ('zero_empty_count', "output.integer(solver.root().a_ways, '\\n');",
         "output.integer(solver.root().a_cost ? solver.root().a_ways : 0, '\\n');"),
        ('add_child_counts', '1LL * left.a_ways * right.a_ways % MOD',
         '(left.a_ways + right.a_ways) % MOD'),
    ]
    # Escaped strings above contain literal \n in the C++ source.
    changes = [(name, old.replace('\\\\n', '\\n'), new.replace('\\\\n', '\\n'))
               for name, old, new in changes]
    cases = [x for x in json.loads((ROOT / 'manifest.json').read_text(encoding='utf-8'))['cases']
             if x['kind'] == 'private']
    records = []
    with tempfile.TemporaryDirectory(prefix='mutants_', dir=build_dir) as temp_name:
        temp = Path(temp_name)
        for name, old, new in changes:
            if old not in source: raise AssertionError('mutation target missing: ' + name)
            cpp = temp / (name + '.cpp')
            cpp.write_text(source.replace(old, new), encoding='utf-8')
            binary = executable(temp, name)
            compile_cpp(cpp, binary, args.cxx)
            killed = None
            for case in cases:
                output = temp / 'mutant.out'
                try:
                    run_solution(binary, ROOT / case['input'], output, timeout=10)
                    check = subprocess.run([str(executable(build_dir, 'checker')),
                                            str(ROOT / case['input']), str(output),
                                            str(ROOT / case['answer'])], capture_output=True, text=True)
                    if check.returncode:
                        killed = {'case': case['id'], 'reason': check.stderr.strip()}
                        break
                except (RuntimeError, subprocess.TimeoutExpired) as error:
                    killed = {'case': case['id'], 'reason': str(error)}
                    break
            if killed is None: raise AssertionError('mutant survived: ' + name)
            records.append({'mutant': name, **killed})
            print(f"KILLED {name}: {killed['case']}", flush=True)
    report = {'status': 'passed', 'mutants': records, 'killed': len(records)}
    (ROOT / 'reports/mutation.json').write_text(json.dumps(report, ensure_ascii=False, indent=2) + '\n', encoding='utf-8')


if __name__ == '__main__':
    main()
