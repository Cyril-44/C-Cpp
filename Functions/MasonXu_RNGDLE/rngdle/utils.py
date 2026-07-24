import math
import random

MINN = int(1e8)
MAXN = int(4e8)

pw10 = [10**i for i in range(19)]

def is_prime(n: int) -> bool:
    """Deterministic Miller-Rabin for n < 2^32 (enough for our 4e8 range)."""
    if n < 2:
        return False
    small_primes = [2,3,5,7,11,13,17,19,23,29]
    for p in small_primes:
        if n % p == 0:
            return n == p
    d = n-1
    s = 0
    while d % 2 == 0:
        d //= 2
        s += 1
    # bases sufficient for 32-bit
    for a in (2,7,61):
        if a % n == 0:
            continue
        x = pow(a, d, n)
        if x == 1 or x == n-1:
            continue
        skip = False
        for _ in range(s-1):
            x = (x*x) % n
            if x == n-1:
                skip = True
                break
        if skip:
            continue
        return False
    return True


def digits_list(x: int):
    a = [0]*10
    s = str(x).rjust(9,'0')
    for ch in s:
        a.append(int(ch))
    # return list of last 9 digits as integers (1..9 positions in original)
    return [int(ch) for ch in s]


def is_contain(x: int, y: int, yn: int) -> bool:
    s = str(x).rjust(9,'0')
    ys = str(y).rjust(yn,'0')
    return ys in s


def have_badge(x: int, y: int) -> bool:
    from .badges import BADGES
    if y < 1 or y >= len(BADGES):
        return False
    return BADGES[y].test(x)


def quicktest(x: int, maxcount: int=153) -> int:
    # import BADGES lazily to avoid circular imports
    from .badges import BADGES
    sumv = 0
    for i in range(1, min(maxcount, len(BADGES)-1)+1):
        if have_badge(x, i):
            # approximate points calculation from original: 100000/sqrt(poss)
            p = int(100000 / math.sqrt(BADGES[i].poss)) if BADGES[i].poss > 0 else 0
            sumv += p
    return sumv


def randomtest(n: int):
    res = []
    for _ in range(n):
        x = random.randint(MINN, MAXN)
        res.append(quicktest(x))
    res.sort()
    return {
        'bottom_1%': res[max(0, int(0.01*n)-1)],
        'top_50%': res[int(0.5*n)-1],
        'top_25%': res[int(0.75*n)-1],
        'top_10%': res[int(0.9*n)-1],
        'top_5%': res[int(0.95*n)-1],
        'top_1%': res[int(0.99*n)-1],
        'top_1': res[-1]
    }
