#!/usr/bin/env python3
"""Check exact answer count and canonical residues; optionally compare a reference."""
import sys
from pathlib import Path
P=998244353

def check(inp, out, expected=None):
    with Path(inp).open() as f:
        n=int(f.readline()); assert len(f.readline().split())==n
        m=int(f.readline()); q=sum(f.readline().split()[0]=='3' for _ in range(m))
        assert not f.read().strip()
    tokens=Path(out).read_text().split()
    assert len(tokens)==q, f'{out}: expected {q} answers, got {len(tokens)}'
    values=[]
    for s in tokens:
        assert s.isascii() and s.isdigit(), f'{out}: noncanonical integer {s!r}'
        x=int(s); assert 0<=x<P, f'{out}: answer outside [0,P)'
        values.append(x)
    if expected is not None:
        assert values==list(map(int,Path(expected).read_text().split())), f'{out}: differs from {expected}'
    return q
if __name__=='__main__':
    if len(sys.argv) not in (3,4): raise SystemExit('usage: check_output.py input output [reference]')
    print(f'OK answers={check(*sys.argv[1:])}')