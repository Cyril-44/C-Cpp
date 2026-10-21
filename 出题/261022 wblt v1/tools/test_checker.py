#!/usr/bin/env python3
"""Positive and negative tests for the output checker."""
import argparse
import json
from pathlib import Path
import tempfile
from checker import check

ROOT = Path(__file__).resolve().parents[1]

def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--report', type=Path, default=ROOT/'audit'/'checker_tests.json')
    a = p.parse_args()
    cases = [
        ('exact', b'0\n1\n998244352\n', True),
        ('whitespace', b' \t0  1\r\n998244352 \n', True),
        ('leading_zero_and_plus', b'+0 0001 +998244352', True),
        ('wrong_value', b'0 2 998244352', False),
        ('missing_answer', b'0 1', False),
        ('extra_answer', b'0 1 998244352 0', False),
        ('empty', b'', False),
        ('negative_residue', b'0 1 -1', False),
        ('modulus_as_residue', b'0 1 998244353', False),
        ('decimal', b'0 1 998244352.0', False),
        ('scientific_notation', b'0 1 9.98244352e8', False),
        ('junk_suffix', b'0 1 998244352x', False),
        ('utf8_bom', b'\xef\xbb\xbf0 1 998244352', False),
        ('huge_integer', b'0 1 '+b'9'*10000, False),
        ('nan', b'0 1 NaN', False),
    ]
    results = []
    with tempfile.TemporaryDirectory(prefix='window_checker_') as temp:
        temp = Path(temp); answer = temp/'answer'; candidate = temp/'candidate'
        answer.write_bytes(b'0\n1\n998244352\n')
        for name, text, expected in cases:
            candidate.write_bytes(text)
            try:
                check(candidate, answer); accepted = True
            except ValueError:
                accepted = False
            if accepted != expected:
                raise AssertionError(name)
            results.append({'name': name, 'expected_accepted': expected, 'passed': True})
    report = {'all_passed': True, 'tests': len(results), 'cases': results}
    a.report.parent.mkdir(parents=True, exist_ok=True)
    a.report.write_text(json.dumps(report, indent=2)+'\n', encoding='utf-8')
    print(f'PASS {len(results)} checker tests')

if __name__ == '__main__':
    main()
