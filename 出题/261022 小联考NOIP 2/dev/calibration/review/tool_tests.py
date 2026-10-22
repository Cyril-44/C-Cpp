#!/usr/bin/env python3
"""Positive/negative testlib validator cases, generator strategy checks, checker tests."""
import argparse
import json
import subprocess
import tempfile
from pathlib import Path
from check_output import check
from checker import check as token_check

ROOT = Path(__file__).resolve().parents[1]

def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--build', type=Path, default=ROOT / '.build')
    a = p.parse_args()
    tests = []
    def validate(name, text, accepted, group=None):
        command = [str(a.build / 'val')]
        if group is not None:
            command += ['--group', str(group)]
        result = subprocess.run(command, input=text.encode(), stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        assert (result.returncode == 0) == accepted, (name, result.stderr.decode())
        tests.append({'id': name, 'expected': 'accept' if accepted else 'reject', 'passed': True})
    minimal = '1\n0\n1\n3 1 1\n'
    validate('minimal', minimal, True, 1)
    for path in sorted((ROOT / 'review/samples').glob('*.in')):
        validate('sample_' + path.stem, path.read_text(), True, 1)
    invalid = {
        'n_zero': minimal.replace('1\n0', '0\n0', 1),
        'n_above_max': '500001\n',
        'm_zero': '1\n0\n0\n',
        'm_above_max': '1\n0\n500001\n',
        'negative_initial': minimal.replace('\n0\n', '\n-1\n'),
        'initial_modulus': minimal.replace('\n0\n', '\n998244353\n'),
        'no_query': '1\n0\n1\n2 1 1 0 0\n',
        'left_zero': minimal.replace('3 1 1', '3 0 1'),
        'right_above_n': minimal.replace('3 1 1', '3 1 2'),
        'reversed_range': '2\n0 0\n1\n3 2 1\n',
        'op_zero': minimal.replace('3 1 1', '0 1 1'),
        'op_four': minimal.replace('3 1 1', '4 1 1'),
        'single_point_calibration': '1\n0\n2\n1 1 1 1\n3 1 1\n',
        'zero_k': '2\n0 0\n2\n1 1 2 0\n3 1 2\n',
        'k_above_max': '2\n0 0\n2\n1 1 2 2\n3 1 2\n',
        'u_modulus': '1\n0\n2\n2 1 1 998244353 0\n3 1 1\n',
        'v_negative': '1\n0\n2\n2 1 1 1 -1\n3 1 1\n',
        'trailing_token': minimal + '1\n',
        'missing_operation': '1\n0\n1\n',
    }
    for name, text in invalid.items():
        validate(name, text, False)
    validate('wrong_group', minimal, False, 2)
    validate('invalid_group', minimal, False, 0)
    profiles = ['mixed', 'affine', 'queries', 'point', 'boundaries', 'offset', 'nested', 'gc', 'noop', 'small_k', 'large_k']
    generated = 0
    for group, n, m in [(1, 16, 100), (2, 20, 2001), (3, 20, 2001), (4, 20, 2001), (5, 20, 20001)]:
        for profile in profiles + (['sweep'] if group == 1 else []) + (['preserved_seed'] if group in (1, 4, 5) else []):
            text = subprocess.check_output([str(a.build / 'gen'), str(n), str(m), str(group), 'edges', profile, str(20261022 + generated)])
            validate(f'generator_g{group}_{profile}', text.decode(), True, group)
            generated += 1
    for group in [2, 3]:
        result = subprocess.run([str(a.build / 'gen'), '20', '2001', str(group), 'random', 'preserved_seed', '1'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        assert result.returncode != 0
    with tempfile.TemporaryDirectory(prefix='calibration_tools_') as tmp:
        tmp = Path(tmp)
        inp, ans, out = [tmp / name for name in ['case.in', 'case.ans', 'case.out']]
        inp.write_text(minimal)
        ans.write_text('0\n')
        for text, good in [('0\n', True), (' 0 \n', True), ('998244353\n', False), ('-1\n', False), ('1\n', False), ('0\n0\n', False), ('', False), ('text\n', False)]:
            out.write_text(text)
            try:
                check(inp, out, ans)
            except (AssertionError, ValueError):
                assert not good, text
            else:
                assert good, text
            try:
                token_check(out, ans)
            except (ValueError, OSError):
                assert not good, text
            else:
                assert good, text
    report = dict(status='passed', validator_and_generator_tests=len(tests), generated_strategies=generated, output_shape_tests=8, token_checker_tests=8, tests=tests)
    (ROOT / 'review/tool_tests.json').write_text(json.dumps(report, ensure_ascii=False, indent=2) + '\n')
    print(json.dumps({key: value for key, value in report.items() if key != 'tests'}))

if __name__ == '__main__':
    main()
