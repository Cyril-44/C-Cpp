#!/usr/bin/env python3
"""Check q output rows, exact integer syntax and legal answer ranges."""
import pathlib, re, sys
inp, ans = map(pathlib.Path,sys.argv[1:])
with inp.open('rb') as f:
    n,q = map(int,f.readline().split())
rows = ans.read_bytes().splitlines()
assert len(rows)==q, '{}: expected {} answer rows, got {}'.format(ans,q,len(rows))
for i,row in enumerate(rows,1):
    assert re.fullmatch(rb'[0-9]+ [0-9]+',row), '{}:{} malformed answer row'.format(ans,i)
    cost,ways=map(int,row.split())
    assert 0<=cost<=(2*n-1)*10**9 and 0<=ways<998244353, '{}:{} invalid range'.format(ans,i)
