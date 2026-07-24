import math
from dataclasses import dataclass
from typing import Callable, Optional

TYPE_SPECIAL = 0
TYPE_NEEDAB = 1
TYPE_DIVIDE = 2
TYPE_CONTAIN = 3
TYPE_DIFFER = 4

# Keep original raw data, then convert into Badge objects below.

from .utils import is_prime, MAXN


@dataclass
class Badge:
    name: str
    desc: str
    poss: int
    testtype: int
    test_fn: Callable[[int], bool] = (lambda x: False)

    def test(self, x: int) -> bool:
        # call the per-badge test function (assigned below)
        return bool(self.test_fn(x))


def _badge_test(badge: Badge, x: int) -> bool:
    t = badge.testtype
    name = badge.name
    # build 9-digit array a[1..9]
    s9 = str(x).rjust(9, '0')
    a = [0]*10
    for i,ch in enumerate(s9, start=1):
        a[i] = int(ch)

    if t == TYPE_SPECIAL:
        if name == 'Prime':
            return is_prime(x)
        if name == 'Second Power':
            r = int(math.isqrt(x))
            return r*r == x
        if name == 'Third Power':
            r = int(round(x ** (1/3)))
            return r*r*r == x
        if name == 'Fourth Power':
            r = int(round(x ** 0.25))
            return r**4 == x
        if name == 'High Power':
            for base in range(2, 101):
                p = base**5
                while p <= MAXN:
                    if p == x:
                        return True
                    p *= base
            return False
        if name in ('Hopscotch', 'Hyper Hopscotch', 'Ultra Hopscotch'):
            if name == 'Hopscotch':
                for i in range(1,6):
                    if a[i] == a[i+2] and a[i+2] == a[i+4]:
                        return True
                return False
            if name == 'Hyper Hopscotch':
                for i in range(1,4):
                    if a[i] == a[i+2] == a[i+4] == a[i+6]:
                        return True
                return False
            if name == 'Ultra Hopscotch':
                return a[1]==a[3]==a[5]==a[7]==a[9]
        if name in ('Neutrality', 'Almost Sorted'):
            cnt1 = cnt2 = 0
            for i in range(1,9):
                for j in range(i+1,10):
                    if a[i] > a[j]:
                        cnt1 +=1
                    if a[i] < a[j]:
                        cnt2 +=1
            if name == 'Neutrality':
                return cnt1 == cnt2
            return cnt1 == 1 or cnt2 == 1
        if name == 'Zipper':
            return a[1]==a[3]==a[5]==a[7]==a[9] and a[2]==a[4]==a[6]==a[8]
        if name == 'Echo Form':
            for i in range(1,5):
                if a[i]==a[i+2]==a[i+4] and a[i+1]==a[i+3]==a[i+5]:
                    return True
            return False
        if name == 'Echo Form+':
            for i in range(1,3):
                if a[i]==a[i+2]==a[i+4]==a[i+6] and a[i+1]==a[i+3]==a[i+5]==a[i+7]:
                    return True
            return False
        if name == 'Echo Form Echo Form':
            return a[1]==a[4]==a[7] and a[2]==a[5]==a[8] and a[3]==a[6]==a[9]
        feather_names = {
            'Beginner Feather': 25, 'Beginner Heavy': 50,
            'Intermediate Feather': 20, 'Intermediate Heavy': 55,
            'Advanced Feather': 15, 'Advanced Heavy': 60,
            'Expert Feather': 10, 'Expert Heavy': 65,
            'Grandmaster Feather': 5, 'Grandmaster Heavy': 70
        }
        if name in feather_names:
            s = sum(a[1:10])
            thresh = feather_names[name]
            if 'Feather' in name:
                return s <= thresh
            else:
                return s >= thresh
        if name == 'Harshed Number':
            s = sum(a[1:10])
            return s > 0 and x % s == 0
        if name.startswith('Mini Palindrome'):
            k = int(name.split()[-1])
            for i in range(1, 10-k+1):
                ok = True
                for j in range(k//2):
                    if a[i+j] != a[i+k-1-j]:
                        ok = False
                        break
                if ok:
                    return True
            return False
        if name == 'Mini Echo':
            for i in range(1,7):
                if a[i]==a[i+2] and a[i+1]==a[i+3]:
                    return True
            return False
        if name == 'Medium Echo':
            for i in range(1,5):
                if a[i]==a[i+3] and a[i+1]==a[i+4] and a[i+2]==a[i+5]:
                    return True
            return False
        if name == 'Major Echo':
            for i in range(1,3):
                if a[i]==a[i+4] and a[i+1]==a[i+5] and a[i+2]==a[i+6] and a[i+3]==a[i+7]:
                    return True
            return False
        if name == 'Wide':
            return (max(a[1:10]) - min(a[1:10])) == 9
        if name == 'Slim':
            return (max(a[1:10]) - min(a[1:10])) <= 3
        if name == 'Palindrome':
            return a[1]==a[9] and a[2]==a[8] and a[3]==a[7] and a[4]==a[6]

    if t == TYPE_DIVIDE:
        import re
        desc = badge.desc if hasattr(badge,'desc') else ''
        m = re.search(r"Divisible by\s*(\d+)", desc)
        if m:
            d = int(m.group(1))
            return x % d == 0
        if name == 'Clean Ten':
            return x % 10 == 0
        if name == 'Clean Hundred':
            return x % 100 == 0
        if name == 'Clean Thousand':
            return x % 1000 == 0
        if name == 'Clean Ten Thousand':
            return x % 10000 == 0
        if name == 'Clean Hundred Thousand':
            return x % 100000 == 0
        if name == 'Clean Million':
            return x % 1000000 == 0
        if name == 'Clean Ten Million':
            return x % 10000000 == 0
        if name == 'Clean Hundred Million':
            return x % 100000000 == 0
        if name == 'Binary Clean':
            return x % 1024 == 0
        if name == 'Double Binary Clean':
            return x % 1048576 == 0
        return False

    if t == TYPE_CONTAIN:
        import re
        m = re.search(r"(\d+)", badge.desc if hasattr(badge,'desc') else '')
        if m:
            pat = m.group(1)
            return pat in s9
        return False

    if t == TYPE_NEEDAB:
        b = [0]*10
        for i in range(1,10):
            b[a[i]] += 1
        import re
        m = re.search(r"Contains exactly 1 \"?(\d)\"?", badge.desc if hasattr(badge,'desc') else '')
        if m:
            d = int(m.group(1))
            return b[d] == 1
        if name == 'Abyss':
            return b[0] == 0
        if name == 'Binary Soul':
            return b[0] + b[1] == 9
        if name == 'Low Ball':
            return sum(b[i] for i in range(0,5)) == 9
        if name == 'High Roller':
            return sum(b[i] for i in range(5,10)) == 8
        if name == 'Divisible by 3':
            return b[0] + b[3] + b[6] + b[9] == 9
        cnt = sum(1 for i in range(10) if b[i] > 0)
        maxx = max(b)
        if name == 'Skipping':
            for i in range(0,9):
                if b[i] and b[i+1]:
                    return False
            return True
        if name == 'Consecutive Pairs':
            for i in range(0,8):
                if b[i] >=2 and b[i+1] >=2 and b[i+2] >=2:
                    return True
            return False
        if name == 'Airplane':
            for i in range(0,9):
                if i+1<=9 and b[i] >=3 and b[min(i+1,9)] >=3:
                    return True
            return False
        if name == 'Triple Airplane':
            for i in range(0,8):
                if b[i]>=3 and b[i+1]>=3 and b[i+2]>=3:
                    return True
            return False
        if name == 'Consecutive Pairs+':
            for i in range(0,7):
                if b[i]>=2 and b[i+1]>=2 and b[i+2]>=2 and b[i+3]>=2:
                    return True
            return False
        if name.startswith('Digit Complex'):
            need = int(name.split()[-1])
            return cnt == need
        if name.startswith('Repeat'):
            nrep = int(name.split()[-1])
            return maxx >= nrep
        if name == 'Odd Todd':
            return sum(b[i] for i in [0,2,4,6,8]) == 0
        if name == 'Even Steven':
            return sum(b[i] for i in [1,3,5,7,9]) == 0
        if name == 'Almost Balanced':
            evencnt = sum(b[i] for i in [0,2,4,6,8])
            return evencnt == 4 or evencnt == 5
        buc = [0]*10
        for i in range(10):
            buc[b[i]] += 1
        c = []
        for val in range(9,-1,-1):
            c.extend([val]*buc[val])
        freq_sorted = sorted([cnt for cnt in b if cnt>0], reverse=True)
        if name == 'Two Pair':
            return len(freq_sorted) >= 2 and freq_sorted[0] >=2 and freq_sorted[1] >=2
        if name == 'Three Pair':
            return len(freq_sorted) >= 3 and freq_sorted[0] >=2 and freq_sorted[1] >=2 and freq_sorted[2] >=2
        if name == 'Four Pair':
            return len(freq_sorted) >= 4 and all(freq_sorted[i] >=2 for i in range(4))
        if name == 'Full House':
            return len(freq_sorted) >=2 and freq_sorted[0] >=3 and freq_sorted[1] >=2
        if name == 'Two Triplet':
            return len(freq_sorted) >=2 and freq_sorted[0] >=3 and freq_sorted[1] >=3
        if name == 'Three Triplet':
            return len(freq_sorted) >=3 and freq_sorted[0] >=3 and freq_sorted[1] >=3 and freq_sorted[2] >=3
        return False

    if t == TYPE_DIFFER:
        diff = [0]*9
        for i in range(1,9):
            diff[i] = a[i+1] - a[i]
        if name == 'Even Spacing (Absolute)':
            for i in range(1,8):
                if diff[i]*diff[i] != diff[i+1]*diff[i+1]:
                    return False
            return True
        if name == 'Turtle':
            for i in range(1,9):
                if diff[i] > 1 or diff[i] < -1:
                    return False
            return True
        if name == 'German Whisper':
            for i in range(1,9):
                if diff[i] > -5 and diff[i] < 5:
                    return False
            return True
        if name == 'Hills':
            for i in range(1,8):
                if diff[i]*diff[i+1] >= 0:
                    return False
            return True
        if name == 'Dunes':
            for i in range(1,8):
                if diff[i]*diff[i+1] > 0:
                    return False
            return True
        if name in ('Mountain','Mesa','Valley','Canyon'):
            atl = 1
            if name in ('Mesa','Canyon'):
                atl = 0
            mtp = 1
            if name in ('Valley','Canyon'):
                mtp = -1
            xl = xr = 0
            for i in range(1,9):
                if diff[i]*mtp >= atl:
                    xl += 1
                else:
                    break
            for i in range(8,0,-1):
                if diff[i]*mtp <= -atl:
                    xr += 1
                else:
                    break
            return xl + xr >= 8
        if name.startswith('Consecutive') or name.startswith('Contiguous'):
            ava1, ava2 = 1, -1
            if name.startswith('Contiguous'):
                ava1 = ava2 = 0
            cur = 0
            maxx = 0
            for j in range(1,9):
                cur += 1
                if diff[j] != ava1 and diff[j] != ava2:
                    cur = 0
                if j == 8 or diff[j] != diff[j+1]:
                    maxx = max(maxx, cur)
                    cur = 0
            key = name
            if key in ('Consecutive 3','Contiguous 3'):
                return maxx >= 2
            if key in ('Consecutive 2','Contiguous 2'):
                return maxx >= 1
            if key in ('Consecutive 4','Contiguous 4'):
                return maxx >= 3
            if key in ('Consecutive 5','Contiguous 5'):
                return maxx >= 4
            if key in ('Consecutive 6','Contiguous 6'):
                return maxx >= 5
            if key in ('Consecutive 7','Contiguous 7'):
                return maxx >= 6
            if key in ('Consecutive 8','Contiguous 8'):
                return maxx >= 7
            if key == 'Consecutive 9':
                return maxx >= 8
        if name == 'Alternator':
            for i in range(1,9):
                if (diff[i] + 20) % 2 == 0:
                    return False
            return True
        if name in ('Two Contiguous Pair','Three Contiguous Pair','Four Contiguous Pair'):
            cnt = sum(1 for i in range(1,9) if diff[i] == 0)
            if name == 'Two Contiguous Pair':
                return cnt >= 2
            if name == 'Three Contiguous Pair':
                return cnt >= 3
            if name == 'Four Contiguous Pair':
                return cnt >= 4
        if name == 'Steps':
            for i in range(1,9):
                if diff[i] < 0:
                    return False
            return True
        if name == 'Slopes':
            for i in range(1,9):
                if diff[i] > 0:
                    return False
            return True
        return False




# --- Build per-badge test functions and attach to BADGES and BADGES_DATA ---
# Define helpers to build the 9-digit array and common checks

def buildA(x: int):
    s9 = str(x).rjust(9, '0')
    a = [0]*10
    for i,ch in enumerate(s9, start=1):
        a[i] = int(ch)
    return s9, a


def digitCounts(a):
    b = [0]*10
    for i in range(1,10):
        b[a[i]] += 1
    return b


def freqSorted(b):
    return sorted([cnt for cnt in b if cnt>0], reverse=True)


# small factories
def containsFactory(pat: str):
    def f(x: int) -> bool:
        s9, a = buildA(x)
        return pat in s9
    return f


def divisibleFactory(d: int):
    def f(x: int) -> bool:
        return (x % d) == 0
    return f


# explicit per-badge assignments (no fallback to _badge_test)

# --- Hand-written sample tests (Prime, Repeat 9, Clean Million) ---

def test_prime(x:int) -> bool:
    # primality test using existing is_prime
    return is_prime(x)


def test_repeat_9(x:int) -> bool:
    # all nine digits equal
    s9,a = buildA(x)
    # if a digit appears 9 times -> max count == 9
    b = digitCounts(a)
    return max(b) >= 9


def test_clean_million(x:int) -> bool:
    # ends with 6 zeros -> divisible by 1_000_000
    return (x % 1000000) == 0


# --- Override: hand-written explicit tests for first 20 badges (batch 1) ---
# factories used here (reuse helpers from above)

def digitComplexFactory(n:int):
    def f(x:int) -> bool:
        b = digitCounts(buildA(x)[1])
        return sum(1 for cnt in b if cnt>0) == n
    return f


def repeatFactory(r:int):
    def f(x:int) -> bool:
        b = digitCounts(buildA(x)[1])
        return max(b) >= r
    return f


# Assign explicit test functions for badges 1..20
try:
    BADGES[1].test_fn = divisibleFactory(10)  # Clean Ten
    BADGES[2].test_fn = digitComplexFactory(2)  # Digit Complex 2
    BADGES[3].test_fn = digitComplexFactory(3)
    BADGES[4].test_fn = digitComplexFactory(4)
    BADGES[5].test_fn = digitComplexFactory(5)
    BADGES[6].test_fn = digitComplexFactory(6)
    BADGES[7].test_fn = digitComplexFactory(7)
    BADGES[8].test_fn = digitComplexFactory(8)
    BADGES[9].test_fn = digitComplexFactory(9)
    BADGES[10].test_fn = repeatFactory(4)
    BADGES[11].test_fn = repeatFactory(5)
    BADGES[12].test_fn = repeatFactory(6)
    BADGES[13].test_fn = repeatFactory(7)
    BADGES[14].test_fn = repeatFactory(8)
    BADGES[15].test_fn = repeatFactory(9)
    BADGES[16].test_fn = is_prime
    BADGES[17].test_fn = divisibleFactory(2)
    BADGES[18].test_fn = divisibleFactory(3)
    BADGES[19].test_fn = divisibleFactory(5)
    BADGES[20].test_fn = divisibleFactory(7)
except Exception:
    # Defensive: if BADGES not fully built, skip assignments
    pass


BADGES = [None]
BADGES.append(Badge('Clean Ten', 'Ends with 0.', 30000001, 2, divisibleFactory(10)))
BADGES.append(Badge('Digit Complex 2', 'Use exactly 2 different digits.', 6886, 1, digitComplexFactory(2)))
BADGES.append(Badge('Digit Complex 3', 'Use exactly 3 different digits.', 653400, 1, digitComplexFactory(3)))
BADGES.append(Badge('Digit Complex 4', 'Use exactly 4 different digits.', 11748240, 1, digitComplexFactory(4)))
BADGES.append(Badge('Digit Complex 5', 'Use exactly 5 different digits.', 63059472, 1, digitComplexFactory(5)))
BADGES.append(Badge('Digit Complex 6', 'Use exactly 6 different digits.', 120022560, 1, digitComplexFactory(6)))
BADGES.append(Badge('Digit Complex 7', 'Use exactly 7 different digits.', 83825280, 1, digitComplexFactory(7)))
BADGES.append(Badge('Digit Complex 8', 'Use exactly 8 different digits.', 19595520, 1, digitComplexFactory(8)))
BADGES.append(Badge('Digit Complex 9', 'Use exactly 9 different digits.', 1088640, 1, digitComplexFactory(9)))
BADGES.append(Badge('Repeat 4', 'A same digit exists 4 times.', 24921841, 1, repeatFactory(4)))
BADGES.append(Badge('Repeat 5', 'A same digit exists 5 times.', 2672761, 1, repeatFactory(5)))
BADGES.append(Badge('Repeat 6', 'A same digit exists 6 times.', 192703, 1, repeatFactory(6)))
BADGES.append(Badge('Repeat 7', 'A same digit exists 7 times.', 8995, 1, repeatFactory(7)))
BADGES.append(Badge('Repeat 8', 'A same digit exists 8 times.', 247, 1, repeatFactory(8)))
BADGES.append(Badge('Repeat 9', 'A same digit exists 9 times.', 3, 1, repeatFactory(9)))
BADGES.append(Badge('Prime', 'Divisible only by 1 and itself.', 15574871, 0, is_prime))
BADGES.append(Badge('High Man', 'Divisible by 2.', 150000001, 2, divisibleFactory(2)))
BADGES.append(Badge('Basketball Champion', 'Divisible by 3.', 100000000, 2, divisibleFactory(3)))
BADGES.append(Badge('ICU', 'Divisible by 5.', 60000001, 2, divisibleFactory(5)))
BADGES.append(Badge('Monitor', 'Divisible by 7.', 42857143, 2, divisibleFactory(7)))
BADGES.append(Badge('Li Yin Yang', 'Divisible by 11.', 27272727, 2, (lambda x: _badge_test(BADGES[21], x))))
BADGES.append(Badge('Duck Neck', 'Divisible by 13.', 23076923, 2, (lambda x: _badge_test(BADGES[22], x))))
BADGES.append(Badge('Old Hill', 'Divisible by 15.', 20000000, 2, (lambda x: _badge_test(BADGES[23], x))))
BADGES.append(Badge('Negro', 'Divisible by 17.', 17647059, 2, (lambda x: _badge_test(BADGES[24], x))))
BADGES.append(Badge('Big Plate Chicken', 'Divisible by 19.', 15789474, 2, (lambda x: _badge_test(BADGES[25], x))))
BADGES.append(Badge('Chicken Dust', 'Divisible by 23.', 13043478, 2, (lambda x: _badge_test(BADGES[26], x))))
BADGES.append(Badge('Kevin (Divisible)', 'Divisible by 27.', 11111111, 2, (lambda x: _badge_test(BADGES[27], x))))
BADGES.append(Badge('Plus Seven', 'Divisible by 29.', 10344828, 2, (lambda x: _badge_test(BADGES[28], x))))
BADGES.append(Badge('Blue Archive', 'Divisible by 31.', 9677419, 2, (lambda x: _badge_test(BADGES[29], x))))
BADGES.append(Badge('Iron Ke', 'Divisible by 37.', 8108108, 2, (lambda x: _badge_test(BADGES[30], x))))
BADGES.append(Badge('Chicken Neck', 'Divisible by 41.', 7317073, 2, (lambda x: _badge_test(BADGES[31], x))))
BADGES.append(Badge('Mason Xu', 'Divisible by 43.', 6976744, 2, (lambda x: _badge_test(BADGES[32], x))))
BADGES.append(Badge('Red Chicken', 'Divisible by 47.', 6382979, 2, (lambda x: _badge_test(BADGES[33], x))))
BADGES.append(Badge('ZnSO4', 'Divisible by 49.', 6122449, 2, (lambda x: _badge_test(BADGES[34], x))))
BADGES.append(Badge('Even Steven', 'All digits are even.', 390626, 1, (lambda x: _badge_test(BADGES[35], x))))
BADGES.append(Badge('Odd Todd', 'All digits are odd.', 781250, 1, (lambda x: _badge_test(BADGES[36], x))))
BADGES.append(Badge('Homo 3', 'Contains 114.', 2794201, 3, (lambda x: _badge_test(BADGES[37], x))))
BADGES.append(Badge('Homo 4', 'Contains 1145.', 249977, 3, (lambda x: _badge_test(BADGES[38], x))))
BADGES.append(Badge('Homo 5', 'Contains 11451.', 21999, 3, (lambda x: _badge_test(BADGES[39], x))))
BADGES.append(Badge('Full Homo', 'Contains 114514.', 1900, 3, (lambda x: _badge_test(BADGES[40], x))))
BADGES.append(Badge('Half Homo B-Side', 'Contains 1919.', 248090, 3, (lambda x: _badge_test(BADGES[41], x))))
BADGES.append(Badge('Homo B-Side', 'Contains 1919810.', 160, 3, (lambda x: _badge_test(BADGES[42], x))))
BADGES.append(Badge('Kevin', 'Contains 27.', 29962957, 3, (lambda x: _badge_test(BADGES[43], x))))
BADGES.append(Badge('O Kevin', 'Contains 15.', 29962957, 3, (lambda x: _badge_test(BADGES[44], x))))
BADGES.append(Badge('Almost Balanced', 'The number of even and odd digits differs 1.', 147656250, 1, (lambda x: _badge_test(BADGES[45], x))))
BADGES.append(Badge('Consecutive 3', 'Contains 3 adjust consecutive digits in order.', 30406795, 4, (lambda x: _badge_test(BADGES[46], x))))
BADGES.append(Badge('Consecutive 2', 'Contains 2 adjust consecutive digits in order.', 239251563, 4, (lambda x: _badge_test(BADGES[47], x))))
BADGES.append(Badge('Consecutive 4', 'Contains 4 adjust consecutive digits in order.', 2323318, 4, (lambda x: _badge_test(BADGES[48], x))))
BADGES.append(Badge('Consecutive 5', 'Contains 5 adjust consecutive digits in order.', 161997, 4, (lambda x: _badge_test(BADGES[49], x))))
BADGES.append(Badge('Consecutive 6', 'Contains 6 adjust consecutive digits in order.', 11220, 4, (lambda x: _badge_test(BADGES[50], x))))
BADGES.append(Badge('Consecutive 7', 'Contains 7 adjust consecutive digits in order.', 742, 4, (lambda x: _badge_test(BADGES[51], x))))
BADGES.append(Badge('Consecutive 8', 'Contains 8 adjust consecutive digits in order.', 37, 4, (lambda x: _badge_test(BADGES[52], x))))
BADGES.append(Badge('Consecutive 9', 'Contains 9 adjust consecutive digits in order.', 1, 4, (lambda x: _badge_test(BADGES[53], x))))
BADGES.append(Badge('Contiguous 2', 'Contains 2 adjust same digits.', 170859838, 4, (lambda x: _badge_test(BADGES[54], x))))
BADGES.append(Badge('Contiguous 3', 'Contains 3 adjust same digits.', 18946444, 4, (lambda x: _badge_test(BADGES[55], x))))
BADGES.append(Badge('Contiguous 4', 'Contains 4 adjust same digits.', 1649218, 4, (lambda x: _badge_test(BADGES[56], x))))
BADGES.append(Badge('Contiguous 5', 'Contains 5 adjust same digits.', 138001, 4, (lambda x: _badge_test(BADGES[57], x))))
BADGES.append(Badge('Contiguous 6', 'Contains 6 adjust same digits.', 11101, 4, (lambda x: _badge_test(BADGES[58], x))))
BADGES.append(Badge('Contiguous 7', 'Contains 7 adjust same digits.', 841, 4, (lambda x: _badge_test(BADGES[59], x))))
BADGES.append(Badge('Contiguous 8', 'Contains 8 adjust same digits.', 58, 4, (lambda x: _badge_test(BADGES[60], x))))
BADGES.append(Badge('Alternator', 'Digits strictly alternate between even add odd.', 1171875, 4, (lambda x: _badge_test(BADGES[61], x))))
BADGES.append(Badge('Six Seven', 'Contains 67.', 20552997, 3, (lambda x: _badge_test(BADGES[62], x))))
BADGES.append(Badge('Six Seven (Divisible)', 'Divisible by 67.', 4477612, 2, (lambda x: _badge_test(BADGES[63], x))))
BADGES.append(Badge('Two Pair', 'Contains two different pairs.', 257081418, 1, (lambda x: _badge_test(BADGES[64], x))))
BADGES.append(Badge('Two Contiguous Pair', 'Contains two contiguous different pairs.', 56068582, 4, (lambda x: _badge_test(BADGES[65], x))))
BADGES.append(Badge('Three Pair', 'Contains three different pairs.', 109006128, 1, (lambda x: _badge_test(BADGES[66], x))))
BADGES.append(Badge('Three Contiguous Pair', 'Contains three contiguous different pairs.', 11427538, 4, (lambda x: _badge_test(BADGES[67], x))))
BADGES.append(Badge('Four Pair', 'Contains four different pairs.', 10478160, 1, (lambda x: _badge_test(BADGES[68], x))))
BADGES.append(Badge('Four Contiguous Pair', 'Contains four contiguous different pairs.', 1507306, 4, (lambda x: _badge_test(BADGES[69], x))))
BADGES.append(Badge('Second Power', 'A perfect square.', 10001, 0, (lambda x: _badge_test(BADGES[70], x))))
BADGES.append(Badge('Third Power', 'A perfect cube.', 272, 0, (lambda x: _badge_test(BADGES[71], x))))
BADGES.append(Badge('Fourth Power', 'A perfect fourth power.', 42, 0, (lambda x: _badge_test(BADGES[72], x))))
BADGES.append(Badge('High Power', 'A perfect fifth or more power.', 27, 0, (lambda x: _badge_test(BADGES[73], x))))
BADGES.append(Badge('Wide', 'Contains both 0 and 9.', 92051322, 0, (lambda x: _badge_test(BADGES[74], x))))
BADGES.append(Badge('Slim', 'Max and min digits differ not greater than 3.', 550458, 0, (lambda x: _badge_test(BADGES[75], x))))
BADGES.append(Badge('Palindrome', 'Reads the same forwards and backwards.', 30000, 0, (lambda x: _badge_test(BADGES[76], x))))
BADGES.append(Badge('Clean Hundred', 'Ends in 00.', 3000001, 2, (lambda x: _badge_test(BADGES[77], x))))
BADGES.append(Badge('Clean Thousand', 'Ends in 000.', 300001, 2, (lambda x: _badge_test(BADGES[78], x))))
BADGES.append(Badge('Clean Ten Thousand', 'Ends in 0000.', 30001, 2, (lambda x: _badge_test(BADGES[79], x))))
BADGES.append(Badge('Clean Hundred Thousand', 'Ends in 00000.', 3001, 2, (lambda x: _badge_test(BADGES[80], x))))
BADGES.append(Badge('Clean Million', 'Ends in 000000.', 301, 2, (lambda x: _badge_test(BADGES[81], x))))
BADGES.append(Badge('Clean Ten Million', 'Ends in 0000000.', 31, 2, (lambda x: _badge_test(BADGES[82], x))))
BADGES.append(Badge('Clean Hundred Million', 'Ends in 00000000.', 4, 2, (lambda x: _badge_test(BADGES[83], x))))
BADGES.append(Badge('Binary Soul', 'Contains only 0s and 1s.', 256, 1, (lambda x: _badge_test(BADGES[84], x))))
BADGES.append(Badge('Low Ball', 'Contains only digits from 0 to 4.', 1171876, 1, (lambda x: _badge_test(BADGES[85], x))))
BADGES.append(Badge('High Roller', 'Contains only digits from 5 to 9 expect for the first digit.', 1171875, 1, (lambda x: _badge_test(BADGES[86], x))))
BADGES.append(Badge('Steps', 'Digits never decrease.', 22308, 4, (lambda x: _badge_test(BADGES[87], x))))
BADGES.append(Badge('Slopes', 'Digits never increase.', 220, 4, (lambda x: _badge_test(BADGES[88], x))))
BADGES.append(Badge('Mountain', 'Digits ascend to a peak and then descend.', 28662, 4, (lambda x: _badge_test(BADGES[89], x))))
BADGES.append(Badge('Mesa', 'Digits rise to a peak, then fall (flat stretches allowed).', 1377302, 4, (lambda x: _badge_test(BADGES[90], x))))
BADGES.append(Badge('Valley', 'Digits descend to a trough and then ascend.', 532, 4, (lambda x: _badge_test(BADGES[91], x))))
BADGES.append(Badge('Canyon', 'Digits fall to a floor, then rise (flat stretches allowed).', 161292, 4, (lambda x: _badge_test(BADGES[92], x))))
BADGES.append(Badge('Hills', 'Digits strictly alternate between rising and falling.', 8461713, 4, (lambda x: _badge_test(BADGES[93], x))))
BADGES.append(Badge('Dunes', 'Rises and falls keep alternating (flat stretches allowed).', 40436845, 4, (lambda x: _badge_test(BADGES[94], x))))
BADGES.append(Badge('Divisible by 3', 'All digits are divisible by 3.', 65536, 1, (lambda x: _badge_test(BADGES[95], x))))
BADGES.append(Badge('Abyss', 'Contains no 0s.', 129140163, 1, (lambda x: _badge_test(BADGES[96], x))))
BADGES.append(Badge('Void', 'Contains exactly 1 "0".', 114791256, 1, (lambda x: _badge_test(BADGES[97], x))))
BADGES.append(Badge('Hydrogen', 'Contains exactly 1 "1".', 119574225, 1, (lambda x: _badge_test(BADGES[98], x))))
BADGES.append(Badge('Helium', 'Contains exactly 1 "2".', 119574225, 1, (lambda x: _badge_test(BADGES[99], x))))
BADGES.append(Badge('Lithium', 'Contains exactly 1 "3".', 119574225, 1, (lambda x: _badge_test(BADGES[100], x))))
BADGES.append(Badge('Beryllium', 'Contains exactly 1 "4".', 114791257, 1, (lambda x: _badge_test(BADGES[101], x))))
BADGES.append(Badge('Boron', 'Contains exactly 1 "5".', 114791256, 1, (lambda x: _badge_test(BADGES[102], x))))
BADGES.append(Badge('Carbon', 'Contains exactly 1 "6".', 114791256, 1, (lambda x: _badge_test(BADGES[103], x))))
BADGES.append(Badge('Nitrogen', 'Contains exactly 1 "7".', 114791256, 1, (lambda x: _badge_test(BADGES[104], x))))
BADGES.append(Badge('Oxygen', 'Contains exactly 1 "8".', 114791256, 1, (lambda x: _badge_test(BADGES[105], x))))
BADGES.append(Badge('Fluorine', 'Contains exactly 1 "9".', 114791256, 1, (lambda x: _badge_test(BADGES[106], x))))
BADGES.append(Badge('Mini Echo', 'Contains an adjacent 2-digit repeat.', 16346101, 0, (lambda x: _badge_test(BADGES[107], x))))
BADGES.append(Badge('Medium Echo', 'Contains an adjacent 3-digit repeat.', 1110001, 0, (lambda x: _badge_test(BADGES[108], x))))
BADGES.append(Badge('Major Echo', 'Contains an adjacent 4-digit repeat.', 57001, 0, (lambda x: _badge_test(BADGES[109], x))))
BADGES.append(Badge('Mini Palindrome 3', 'Contains a palindrome of 3 digits.', 156510931, 0, (lambda x: _badge_test(BADGES[110], x))))
BADGES.append(Badge('Mini Palindrome 4', 'Contains a palindrome of 4 digits.', 17565718, 0, (lambda x: _badge_test(BADGES[111], x))))
BADGES.append(Badge('Mini Palindrome 5', 'Contains a palindrome of 5 digits.', 14710441, 0, (lambda x: _badge_test(BADGES[112], x))))
BADGES.append(Badge('Mini Palindrome 6', 'Contains a palindrome of 6 digits.', 1198264, 0, (lambda x: _badge_test(BADGES[113], x))))
BADGES.append(Badge('Mini Palindrome 7', 'Contains a palindrome of 7 digits.', 899131, 0, (lambda x: _badge_test(BADGES[114], x))))
BADGES.append(Badge('Mini Palindrome 8', 'Contains a palindrome of 8 digits.', 59998, 0, (lambda x: _badge_test(BADGES[115], x))))
BADGES.append(Badge('Binary Clean', 'Divisible by 1024.', 292969, 2, (lambda x: _badge_test(BADGES[116], x))))
BADGES.append(Badge('Double Binary Clean', 'Divisible by 1048576.', 286, 2, (lambda x: _badge_test(BADGES[117], x))))
BADGES.append(Badge('Harshed Number', 'Divisible by the sum of its own digits.', 19647577, 0, (lambda x: _badge_test(BADGES[118], x))))
BADGES.append(Badge('Beginner Feather', 'The sum of its digits is not greater than 25.', 19085851, 0, (lambda x: _badge_test(BADGES[119], x))))
BADGES.append(Badge('Beginner Heavy', 'The sum of its digits is not less than 50.', 24237105, 0, (lambda x: _badge_test(BADGES[120], x))))
BADGES.append(Badge('Intermediate Feather', 'The sum of its digits is not greater than 20.', 4515006, 0, (lambda x: _badge_test(BADGES[121], x))))
BADGES.append(Badge('Intermediate Heavy', 'The sum of its digits is not less than 55.', 6242979, 0, (lambda x: _badge_test(BADGES[122], x))))
BADGES.append(Badge('Advanced Feather', 'The sum of its digits is not greater than 15.', 643591, 0, (lambda x: _badge_test(BADGES[123], x))))
BADGES.append(Badge('Advanced Heavy', 'The sum of its digits is not less than 60.', 997998, 0, (lambda x: _badge_test(BADGES[124], x))))
BADGES.append(Badge('Expert Feather', 'The sum of its digits is not greater than 10.', 43616, 0, (lambda x: _badge_test(BADGES[125], x))))
BADGES.append(Badge('Expert Heavy', 'The sum of its digits is not less than 65.', 80930, 0, (lambda x: _badge_test(BADGES[126], x))))
BADGES.append(Badge('Grandmaster Feather', 'The sum of its digits is not greater than 5.', 706, 0, (lambda x: _badge_test(BADGES[127], x))))
BADGES.append(Badge('Grandmaster Heavy', 'The sum of its digits is not less than 70.', 1947, 0, (lambda x: _badge_test(BADGES[128], x))))
BADGES.append(Badge('German Whisper', 'All consecutive digits differ by at least 5.', 64763, 4, (lambda x: _badge_test(BADGES[129], x))))
BADGES.append(Badge('Turtle', 'All consecutive digits differ by at most 1.', 15104, 4, (lambda x: _badge_test(BADGES[130], x))))
BADGES.append(Badge('Full House', 'Contains a triplet and a pair.', 122770458, 1, (lambda x: _badge_test(BADGES[131], x))))
BADGES.append(Badge('Two Triplet', 'Contains two different triplet.', 13851054, 1, (lambda x: _badge_test(BADGES[132], x))))
BADGES.append(Badge('Echo Form', 'Contains an adjust 2-digit repeat for 3 times.', 111001, 0, (lambda x: _badge_test(BADGES[133], x))))
BADGES.append(Badge('Echo Form+', 'Contains an adjust 2-digit repeat for 4 times.', 571, 0, (lambda x: _badge_test(BADGES[134], x))))
BADGES.append(Badge('Echo Form Echo Form', 'Contains an adjust 3-digit repeat for 3 times.', 300, 0, (lambda x: _badge_test(BADGES[135], x))))
BADGES.append(Badge('Consecutive Pairs', 'Contains 3 pairs that are consecutive in value.', 9601368, 1, (lambda x: _badge_test(BADGES[136], x))))
BADGES.append(Badge('Airplane', 'Contains 2 triplets that are consecutive in value.', 2978906, 1, (lambda x: _badge_test(BADGES[137], x))))
BADGES.append(Badge('Zipper', 'Two digits alternating perfectly.', 30, 0, (lambda x: _badge_test(BADGES[138], x))))
BADGES.append(Badge('Three Triplet', 'Contains three different triplet.', 60480, 1, (lambda x: _badge_test(BADGES[139], x))))
BADGES.append(Badge('Triple Airplane', 'Contains 3 triplets that are consecutive in value.', 4480, 1, (lambda x: _badge_test(BADGES[140], x))))
BADGES.append(Badge('Consecutive Pairs+', 'Contains 4 pairs that are consecutive in value.', 370440, 1, (lambda x: _badge_test(BADGES[141], x))))
BADGES.append(Badge('Neutrality', 'The number of reverse pairs equal to order pairs.', 12979991, 0, (lambda x: _badge_test(BADGES[142], x))))
BADGES.append(Badge('Even Spacing (Absolute)', 'All digits have the same absolute spacing.', 848, 4, (lambda x: _badge_test(BADGES[143], x))))
BADGES.append(Badge('Almost Sorted', 'The number of reverse pairs or order pairs is exactly 1.', 94943, 0, (lambda x: _badge_test(BADGES[144], x))))
BADGES.append(Badge('Skipping', 'No two digits differ by 1.', 3182417, 1, (lambda x: _badge_test(BADGES[145], x))))
BADGES.append(Badge('Six Six Six', 'Contains 666.', 1649217, 3, (lambda x: _badge_test(BADGES[146], x))))
BADGES.append(Badge('Calender', 'Contains 365.', 2794201, 3, (lambda x: _badge_test(BADGES[147], x))))
BADGES.append(Badge('Emergency', 'Contains 911.', 1798200, 3, (lambda x: _badge_test(BADGES[148], x))))
BADGES.append(Badge('Not Found', 'Contains 404.', 1786266, 3, (lambda x: _badge_test(BADGES[149], x))))
BADGES.append(Badge('Slience in Sea', 'Contains 543.', 1798200, 3, (lambda x: _badge_test(BADGES[150], x))))
BADGES.append(Badge('Hopscotch', 'A digit appears at every other position (3 times).', 13940401, 0, (lambda x: _badge_test(BADGES[151], x))))
BADGES.append(Badge('Hyper Hopscotch', 'A digit appears at every other position (4 times).', 869431, 0, (lambda x: _badge_test(BADGES[152], x))))
BADGES.append(Badge('Ultra Hopscotch', 'A digit appears at every other position (5 times).', 30000, 0, (lambda x: _badge_test(BADGES[153], x))))

# Assign explicit test functions for badges 1..20 (handwritten batch)
try:
    BADGES[1].test_fn = divisibleFactory(10)
    BADGES[2].test_fn = digitComplexFactory(2)
    BADGES[3].test_fn = digitComplexFactory(3)
    BADGES[4].test_fn = digitComplexFactory(4)
    BADGES[5].test_fn = digitComplexFactory(5)
    BADGES[6].test_fn = digitComplexFactory(6)
    BADGES[7].test_fn = digitComplexFactory(7)
    BADGES[8].test_fn = digitComplexFactory(8)
    BADGES[9].test_fn = digitComplexFactory(9)
    BADGES[10].test_fn = repeatFactory(4)
    BADGES[11].test_fn = repeatFactory(5)
    BADGES[12].test_fn = repeatFactory(6)
    BADGES[13].test_fn = repeatFactory(7)
    BADGES[14].test_fn = repeatFactory(8)
    BADGES[15].test_fn = repeatFactory(9)
    BADGES[16].test_fn = is_prime
    BADGES[17].test_fn = divisibleFactory(2)
    BADGES[18].test_fn = divisibleFactory(3)
    BADGES[19].test_fn = divisibleFactory(5)
    BADGES[20].test_fn = divisibleFactory(7)
except Exception:
    pass

# Ensure all badges have a callable test_fn; wrap ones that are not callable
for i in range(1, len(BADGES)):
    b = BADGES[i]
    if not callable(getattr(b, 'test_fn', None)):
        b.test_fn = (lambda bb=b: (lambda x: _badge_test(bb, x)))()

MAX_BADGE_INDEX = len(BADGES)-1
