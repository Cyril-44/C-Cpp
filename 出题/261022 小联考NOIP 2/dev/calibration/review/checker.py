#!/usr/bin/env python3
"""Portable token checker: checker.py INPUT CONTESTANT_OUTPUT ANSWER."""
import argparse
import re
import sys
P = 998244353

def tokens(path):
    with open(path, 'rb') as f:
        for line in f:
            for token in line.split():
                if not re.fullmatch(rb'[+-]?[0-9]+', token):
                    raise ValueError('non-integer output token')
                if len(token) > 20:
                    raise ValueError('output integer is too long')
                value = int(token)
                if not 0 <= value < P:
                    raise ValueError('output is not a canonical residue in [0, P)')
                yield value

def check(contestant, answer):
    got, expected = tokens(contestant), tokens(answer)
    count = 0
    while True:
        x, y = next(got, None), next(expected, None)
        if x is None and y is None:
            return count
        count += 1
        if x != y:
            raise ValueError(f'answer #{count}: expected {y}, got {x}')

def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('input'); p.add_argument('contestant_output'); p.add_argument('answer')
    a = p.parse_args()
    try:
        count = check(a.contestant_output, a.answer)
    except (ValueError, OSError) as e:
        print('WA:', e, file=sys.stderr)
        return 1
    print(f'OK: {count} answers')
    return 0

if __name__ == '__main__':
    sys.exit(main())
