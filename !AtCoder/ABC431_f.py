n, d = map(int, input().split())
a = list(map(int, input().split()))

MOD = 998244353
from collections import Counter
import math

mp = Counter(a)
values = sorted(mp.keys())

res = 1
len = 0

for val in values:
    cnt = mp[val]
    res = (res * math.comb(len + cnt, cnt)) % MOD
    len += cnt

print(res)