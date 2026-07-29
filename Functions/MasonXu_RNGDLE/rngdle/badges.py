import math
from dataclasses import dataclass
from typing import Callable, List

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

@dataclass
class Badge:
    name: str
    desc: str
    poss: int
    test_fn: Callable[[int], bool]

    def test(self, x: int) -> bool:
        return bool(self.test_fn(x))


# ---------------------------------------------------------------------------
# 通用工具
# ---------------------------------------------------------------------------
def buildA(x: int):
    """返回 (s9, a)  a 为 1-based 数组"""
    s9 = str(x).rjust(9, '0')
    a = [0] + [int(c) for c in s9]
    return s9, a


def digit_counts(x: int) -> List[int]:
    b = [0] * 10
    for c in str(x).rjust(9, '0'):
        b[int(c)] += 1
    return b


def freq_sorted(b: List[int]) -> List[int]:
    return sorted((c for c in b if c > 0), reverse=True)


# ---------------------------------------------------------------------------
# 工厂函数
# ---------------------------------------------------------------------------
def divisible(d: int):
    return lambda x: x % d == 0


def contains(pat: str):
    return lambda x: pat in str(x).rjust(9, '0')


def digit_complex(n: int):
    def f(x: int) -> bool:
        return sum(1 for c in digit_counts(x) if c > 0) == n
    return f


def repeat_at_least(r: int):
    def f(x: int) -> bool:
        return max(digit_counts(x)) >= r
    return f


def all_even():
    def f(x: int) -> bool:
        b = digit_counts(x)
        return sum(b[i] for i in (1, 3, 5, 7, 9)) == 0
    return f


def all_odd():
    def f(x: int) -> bool:
        b = digit_counts(x)
        return sum(b[i] for i in (0, 2, 4, 6, 8)) == 0
    return f


def almost_balanced():
    def f(x: int) -> bool:
        b = digit_counts(x)
        even = sum(b[i] for i in (0, 2, 4, 6, 8))
        return even in (4, 5)
    return f


def binary_soul():
    def f(x: int) -> bool:
        b = digit_counts(x)
        return b[0] + b[1] == 9
    return f


def low_ball():
    def f(x: int) -> bool:
        b = digit_counts(x)
        return sum(b[i] for i in range(5)) == 9
    return f


def high_roller():
    def f(x: int) -> bool:
        b = digit_counts(x)
        return sum(b[i] for i in range(5, 10)) == 8
    return f


def divisible_by_3_digits():
    def f(x: int) -> bool:
        b = digit_counts(x)
        return b[0] + b[3] + b[6] + b[9] == 9
    return f


def abyss():
    return lambda x: digit_counts(x)[0] == 0


def exactly_one(digit: int):
    def f(x: int) -> bool:
        return digit_counts(x)[digit] == 1
    return f


def skipping():
    def f(x: int) -> bool:
        b = digit_counts(x)
        return all(not (b[i] and b[i + 1]) for i in range(9))
    return f


def two_pair():
    def f(x: int) -> bool:
        fs = freq_sorted(digit_counts(x))
        return len(fs) >= 2 and fs[0] >= 2 and fs[1] >= 2
    return f


def three_pair():
    def f(x: int) -> bool:
        fs = freq_sorted(digit_counts(x))
        return len(fs) >= 3 and fs[0] >= 2 and fs[1] >= 2 and fs[2] >= 2
    return f


def four_pair():
    def f(x: int) -> bool:
        fs = freq_sorted(digit_counts(x))
        return len(fs) >= 4 and all(fs[i] >= 2 for i in range(4))
    return f


def full_house():
    def f(x: int) -> bool:
        fs = freq_sorted(digit_counts(x))
        return len(fs) >= 2 and fs[0] >= 3 and fs[1] >= 2
    return f


def two_triplet():
    def f(x: int) -> bool:
        fs = freq_sorted(digit_counts(x))
        return len(fs) >= 2 and fs[0] >= 3 and fs[1] >= 3
    return f


def three_triplet():
    def f(x: int) -> bool:
        fs = freq_sorted(digit_counts(x))
        return len(fs) >= 3 and fs[0] >= 3 and fs[1] >= 3 and fs[2] >= 3
    return f


def consecutive_pairs():
    def f(x: int) -> bool:
        b = digit_counts(x)
        for i in range(8):
            if b[i] >= 2 and b[i + 1] >= 2 and b[i + 2] >= 2:
                return True
        return False
    return f


def consecutive_pairs_plus():
    def f(x: int) -> bool:
        b = digit_counts(x)
        for i in range(7):
            if b[i] >= 2 and b[i + 1] >= 2 and b[i + 2] >= 2 and b[i + 3] >= 2:
                return True
        return False
    return f


def airplane():
    def f(x: int) -> bool:
        b = digit_counts(x)
        for i in range(9):
            if b[i] >= 3 and b[min(i + 1, 9)] >= 3:
                return True
        return False
    return f


def triple_airplane():
    def f(x: int) -> bool:
        b = digit_counts(x)
        for i in range(8):
            if b[i] >= 3 and b[i + 1] >= 3 and b[i + 2] >= 3:
                return True
        return False
    return f


def perfect_square():
    def f(x: int) -> bool:
        r = int(math.isqrt(x))
        return r * r == x
    return f


def perfect_cube():
    def f(x: int) -> bool:
        r = int(round(x ** (1 / 3)))
        return r * r * r == x
    return f


def perfect_fourth():
    def f(x: int) -> bool:
        r = int(round(x ** 0.25))
        return r ** 4 == x
    return f


def high_power():
    MAXN = int(1e10)
    # 预计算所有 ≤ MAXN 的 5 次及以上幂
    powers = set()
    for base in range(2, 100):
        p = base ** 5
        while p <= MAXN:
            powers.add(p)
            if p > MAXN // base:
                break
            p *= base
    return lambda x: x in powers


def wide():
    def f(x: int) -> bool:
        digits = [int(c) for c in str(x).rjust(9, '0')]
        return max(digits) - min(digits) == 9
    return f


def slim():
    def f(x: int) -> bool:
        digits = [int(c) for c in str(x).rjust(9, '0')]
        return max(digits) - min(digits) <= 3
    return f


def palindrome():
    def f(x: int) -> bool:
        s = str(x).rjust(9, '0')
        return s == s[::-1]
    return f


def mini_palindrome(k: int):
    def f(x: int) -> bool:
        s = str(x).rjust(9, '0')
        for i in range(10 - k):
            sub = s[i:i + k]
            if sub == sub[::-1]:
                return True
        return False
    return f


def mini_echo():
    def f(x: int) -> bool:
        a = buildA(x)[1]
        for i in range(1, 7):
            if a[i] == a[i + 2] and a[i + 1] == a[i + 3]:
                return True
        return False
    return f


def medium_echo():
    def f(x: int) -> bool:
        a = buildA(x)[1]
        for i in range(1, 5):
            if a[i] == a[i + 3] and a[i + 1] == a[i + 4] and a[i + 2] == a[i + 5]:
                return True
        return False
    return f


def major_echo():
    def f(x: int) -> bool:
        a = buildA(x)[1]
        for i in range(1, 3):
            if (a[i] == a[i + 4] and a[i + 1] == a[i + 5] and
                    a[i + 2] == a[i + 6] and a[i + 3] == a[i + 7]):
                return True
        return False
    return f


def echo_form():
    def f(x: int) -> bool:
        a = buildA(x)[1]
        for i in range(1, 5):
            if a[i] == a[i + 2] == a[i + 4] and a[i + 1] == a[i + 3] == a[i + 5]:
                return True
        return False
    return f


def echo_form_plus():
    def f(x: int) -> bool:
        a = buildA(x)[1]
        for i in range(1, 3):
            if (a[i] == a[i + 2] == a[i + 4] == a[i + 6] and
                    a[i + 1] == a[i + 3] == a[i + 5] == a[i + 7]):
                return True
        return False
    return f


def echo_form_echo_form():
    def f(x: int) -> bool:
        a = buildA(x)[1]
        return (a[1] == a[4] == a[7] and
                a[2] == a[5] == a[8] and
                a[3] == a[6] == a[9])
    return f


def zipper():
    def f(x: int) -> bool:
        a = buildA(x)[1]
        return (a[1] == a[3] == a[5] == a[7] == a[9] and
                a[2] == a[4] == a[6] == a[8])
    return f


def hopscotch():
    def f(x: int) -> bool:
        a = buildA(x)[1]
        return any(a[i] == a[i + 2] == a[i + 4] for i in range(1, 6))
    return f


def hyper_hopscotch():
    def f(x: int) -> bool:
        a = buildA(x)[1]
        return any(a[i] == a[i + 2] == a[i + 4] == a[i + 6] for i in range(1, 4))
    return f


def ultra_hopscotch():
    def f(x: int) -> bool:
        a = buildA(x)[1]
        return a[1] == a[3] == a[5] == a[7] == a[9]
    return f


def harshed():
    def f(x: int) -> bool:
        s = sum(int(c) for c in str(x))
        return s > 0 and x % s == 0
    return f


def digit_sum_le(thresh: int):
    return lambda x: sum(int(c) for c in str(x)) <= thresh


def digit_sum_ge(thresh: int):
    return lambda x: sum(int(c) for c in str(x)) >= thresh


def neutrality():
    def f(x: int) -> bool:
        a = buildA(x)[1]
        cnt1 = cnt2 = 0
        for i in range(1, 9):
            for j in range(i + 1, 10):
                if a[i] > a[j]:
                    cnt1 += 1
                elif a[i] < a[j]:
                    cnt2 += 1
        return cnt1 == cnt2
    return f


def almost_sorted():
    def f(x: int) -> bool:
        a = buildA(x)[1]
        cnt1 = cnt2 = 0
        for i in range(1, 9):
            for j in range(i + 1, 10):
                if a[i] > a[j]:
                    cnt1 += 1
                elif a[i] < a[j]:
                    cnt2 += 1
        return cnt1 == 1 or cnt2 == 1
    return f


# ---------- 差值类徽章 ----------
def _diffs(x: int):
    a = buildA(x)[1]
    return [a[i + 1] - a[i] for i in range(1, 9)]


def steps():
    return lambda x: all(d >= 0 for d in _diffs(x))


def slopes():
    return lambda x: all(d <= 0 for d in _diffs(x))


def hills():
    def f(x: int) -> bool:
        d = _diffs(x)
        return all(d[i] * d[i + 1] < 0 for i in range(7))
    return f


def dunes():
    def f(x: int) -> bool:
        d = _diffs(x)
        return all(d[i] * d[i + 1] <= 0 for i in range(7))
    return f


def mountain_like(allow_flat: bool, invert: bool):
    def f(x: int) -> bool:
        d = _diffs(x)
        atl = 0 if allow_flat else 1
        mtp = -1 if invert else 1
        xl = xr = 0
        for i in range(8):
            if d[i] * mtp >= atl:
                xl += 1
            else:
                break
        for i in range(7, -1, -1):
            if d[i] * mtp <= -atl:
                xr += 1
            else:
                break
        return xl + xr >= 8
    return f


def even_spacing_absolute():
    def f(x: int) -> bool:
        d = _diffs(x)
        return all(d[i] * d[i] == d[i + 1] * d[i + 1] for i in range(7))
    return f


def turtle():
    return lambda x: all(abs(d) <= 1 for d in _diffs(x))


def german_whisper():
    return lambda x: all(abs(d) >= 5 for d in _diffs(x))


def alternator():
    def f(x: int) -> bool:
        d = _diffs(x)
        return all((d[i] + 20) % 2 != 0 for i in range(8))
    return f


def contiguous_pairs(n: int):
    def f(x: int) -> bool:
        d = _diffs(x)
        return sum(1 for v in d if v == 0) >= n
    return f


def consecutive_or_contiguous(length: int, allow_flat: bool):
    def f(x: int) -> bool:
        d = _diffs(x)
        ava = {0} if allow_flat else {1, -1}
        cur = maxx = 0
        for i in range(8):
            if d[i] in ava:
                cur += 1
                maxx = max(maxx, cur)
            else:
                cur = 0
        # 原逻辑里 maxx 的阈值和 length 的对应关系保持一致
        need = length - 1
        return maxx >= need
    return f


# ---------------------------------------------------------------------------
# 构建 BADGES 列表（全部使用独立工厂）
# ---------------------------------------------------------------------------
BADGES = [None,
Badge('Clean Ten',               'Ends with 0.',                               30000001, divisible(10)),
Badge('Digit Complex 2',         'Use exactly 2 different digits.',            6886,     digit_complex(2)),
Badge('Digit Complex 3',         'Use exactly 3 different digits.',            653400,   digit_complex(3)),
Badge('Digit Complex 4',         'Use exactly 4 different digits.',            11748240, digit_complex(4)),
Badge('Digit Complex 5',         'Use exactly 5 different digits.',            63059472, digit_complex(5)),
Badge('Digit Complex 6',         'Use exactly 6 different digits.',            120022560,digit_complex(6)),
Badge('Digit Complex 7',         'Use exactly 7 different digits.',            83825280, digit_complex(7)),
Badge('Digit Complex 8',         'Use exactly 8 different digits.',            19595520, digit_complex(8)),
Badge('Digit Complex 9',         'Use exactly 9 different digits.',            1088640,  digit_complex(9)),
Badge('Repeat 4',                'A same digit exists 4 times.',               24921841, repeat_at_least(4)),
Badge('Repeat 5',                'A same digit exists 5 times.',               2672761,  repeat_at_least(5)),
Badge('Repeat 6',                'A same digit exists 6 times.',               192703,   repeat_at_least(6)),
Badge('Repeat 7',                'A same digit exists 7 times.',               8995,     repeat_at_least(7)),
Badge('Repeat 8',                'A same digit exists 8 times.',               247,      repeat_at_least(8)),
Badge('Repeat 9',                'A same digit exists 9 times.',               3,        repeat_at_least(9)),
Badge('Prime',                   'Divisible only by 1 and itself.',            15574871, is_prime),
Badge('High Man',                'Divisible by 2.',                            150000001,divisible(2)),
Badge('Basketball Champion',     'Divisible by 3.',                            100000000,divisible(3)),
Badge('ICU',                     'Divisible by 5.',                            60000001, divisible(5)),
Badge('Monitor',                 'Divisible by 7.',                            42857143, divisible(7)),
Badge('Li Yin Yang',             'Divisible by 11.',                           27272727, divisible(11)),
Badge('Duck Neck',               'Divisible by 13.',                           23076923, divisible(13)),
Badge('Old Hill',                'Divisible by 15.',                           20000000, divisible(15)),
Badge('Negro',                   'Divisible by 17.',                           17647059, divisible(17)),
Badge('Big Plate Chicken',       'Divisible by 19.',                           15789474, divisible(19)),
Badge('Chicken Dust',            'Divisible by 23.',                           13043478, divisible(23)),
Badge('Kevin (Divisible)',       'Divisible by 27.',                           11111111, divisible(27)),
Badge('Plus Seven',              'Divisible by 29.',                           10344828, divisible(29)),
Badge('Blue Archive',            'Divisible by 31.',                           9677419,  divisible(31)),
Badge('Iron Ke',                 'Divisible by 37.',                           8108108,  divisible(37)),
Badge('Chicken Neck',            'Divisible by 41.',                           7317073,  divisible(41)),
Badge('Mason Xu',                'Divisible by 43.',                           6976744,  divisible(43)),
Badge('Red Chicken',             'Divisible by 47.',                           6382979,  divisible(47)),
Badge('ZnSO4',                   'Divisible by 49.',                           6122449,  divisible(49)),
Badge('Even Steven',             'All digits are even.',                       390626,   all_even()),
Badge('Odd Todd',                'All digits are odd.',                        781250,   all_odd()),
Badge('Homo 3',                  'Contains 114.',                              2794201,  contains('114')),
Badge('Homo 4',                  'Contains 1145.',                             249977,   contains('1145')),
Badge('Homo 5',                  'Contains 11451.',                            21999,    contains('11451')),
Badge('Full Homo',               'Contains 114514.',                           1900,     contains('114514')),
Badge('Half Homo B-Side',        'Contains 1919.',                             248090,   contains('1919')),
Badge('Homo B-Side',             'Contains 1919810.',                          160,      contains('1919810')),
Badge('Kevin',                   'Contains 27.',                               29962957, contains('27')),
Badge('O Kevin',                 'Contains 15.',                               29962957, contains('15')),
Badge('Almost Balanced',         'The number of even and odd digits differs 1.',147656250,almost_balanced()),
Badge('Consecutive 3',           'Contains 3 consecutive digits in order.',    30406795, consecutive_or_contiguous(3, False)),
Badge('Consecutive 2',           'Contains 2 consecutive digits in order.',    239251563,consecutive_or_contiguous(2, False)),
Badge('Consecutive 4',           'Contains 4 consecutive digits in order.',    2323318,  consecutive_or_contiguous(4, False)),
Badge('Consecutive 5',           'Contains 5 consecutive digits in order.',    161997,   consecutive_or_contiguous(5, False)),
Badge('Consecutive 6',           'Contains 6 consecutive digits in order.',    11220,    consecutive_or_contiguous(6, False)),
Badge('Consecutive 7',           'Contains 7 consecutive digits in order.',    742,      consecutive_or_contiguous(7, False)),
Badge('Consecutive 8',           'Contains 8 consecutive digits in order.',    37,       consecutive_or_contiguous(8, False)),
Badge('Consecutive 9',           'Contains 9 consecutive digits in order.',    1,        consecutive_or_contiguous(9, False)),
Badge('Contiguous 2',            'Contains 2 contiguous same digits.',         170859838,consecutive_or_contiguous(2, True)),
Badge('Contiguous 3',            'Contains 3 contiguous same digits.',         18946444, consecutive_or_contiguous(3, True)),
Badge('Contiguous 4',            'Contains 4 contiguous same digits.',         1649218,  consecutive_or_contiguous(4, True)),
Badge('Contiguous 5',            'Contains 5 contiguous same digits.',         138001,   consecutive_or_contiguous(5, True)),
Badge('Contiguous 6',            'Contains 6 contiguous same digits.',         11101,    consecutive_or_contiguous(6, True)),
Badge('Contiguous 7',            'Contains 7 contiguous same digits.',         841,      consecutive_or_contiguous(7, True)),
Badge('Contiguous 8',            'Contains 8 contiguous same digits.',         58,       consecutive_or_contiguous(8, True)),
Badge('Alternator',              'Digits strictly alternate even/odd.',        1171875,  alternator()),
Badge('Six Seven',               'Contains 67.',                               20552997, contains('67')),
Badge('Six Seven (Divisible)',   'Divisible by 67.',                           4477612,  divisible(67)),
Badge('Two Pair',                'Contains two different pairs.',              257081418,two_pair()),
Badge('Two Contiguous Pair',     'Contains two contiguous pairs.',             56068582, contiguous_pairs(2)),
Badge('Three Pair',              'Contains three different pairs.',            109006128,three_pair()),
Badge('Three Contiguous Pair',   'Contains three contiguous pairs.',           11427538, contiguous_pairs(3)),
Badge('Four Pair',               'Contains four different pairs.',             10478160, four_pair()),
Badge('Four Contiguous Pair',    'Contains four contiguous pairs.',            1507306,  contiguous_pairs(4)),
Badge('Second Power',            'A perfect square.',                          10001,    perfect_square()),
Badge('Third Power',             'A perfect cube.',                            272,      perfect_cube()),
Badge('Fourth Power',            'A perfect fourth power.',                    42,       perfect_fourth()),
Badge('High Power',              'A perfect fifth or higher power.',           27,       high_power()),
Badge('Wide',                    'Contains both 0 and 9.',                     92051322, wide()),
Badge('Slim',                    'Max-min digits differ ≤ 3.',                 550458,   slim()),
Badge('Palindrome',              'Reads the same forwards and backwards.',     30000,    palindrome()),
Badge('Clean Hundred',           'Ends in 00.',                                3000001,  divisible(100)),
Badge('Clean Thousand',          'Ends in 000.',                               300001,   divisible(1000)),
Badge('Clean Ten Thousand',      'Ends in 0000.',                              30001,    divisible(10000)),
Badge('Clean Hundred Thousand',  'Ends in 00000.',                             3001,     divisible(100000)),
Badge('Clean Million',           'Ends in 000000.',                            301,      divisible(1000000)),
Badge('Clean Ten Million',       'Ends in 0000000.',                           31,       divisible(10000000)),
Badge('Clean Hundred Million',   'Ends in 00000000.',                          4,        divisible(100000000)),
Badge('Binary Soul',             'Only 0s and 1s.',                            256,      binary_soul()),
Badge('Low Ball',                'Only digits 0-4.',                           1171876,  low_ball()),
Badge('High Roller',             'Only digits 5-9 (except first).',            1171875,  high_roller()),
Badge('Steps',                   'Digits never decrease.',                     22308,    steps()),
Badge('Slopes',                  'Digits never increase.',                     220,      slopes()),
Badge('Mountain',                'Ascend then descend.',                       28662,    mountain_like(False, False)),
Badge('Mesa',                    'Rise then fall (flat allowed).',             1377302,  mountain_like(True, False)),
Badge('Valley',                  'Descend then ascend.',                       532,      mountain_like(False, True)),
Badge('Canyon',                  'Fall then rise (flat allowed).',             161292,   mountain_like(True, True)),
Badge('Hills',                   'Strictly alternate rise/fall.',              8461713,  hills()),
Badge('Dunes',                   'Alternate rise/fall (flat allowed).',        40436845, dunes()),
Badge('Divisible by 3',          'All digits divisible by 3.',                 65536,    divisible_by_3_digits()),
Badge('Abyss',                   'Contains no 0s.',                            129140163,abyss()),
Badge('Void',                    'Contains exactly 1 "0".',                    114791256,exactly_one(0)),
Badge('Hydrogen',                'Contains exactly 1 "1".',                    119574225,exactly_one(1)),
Badge('Helium',                  'Contains exactly 1 "2".',                    119574225,exactly_one(2)),
Badge('Lithium',                 'Contains exactly 1 "3".',                    119574225,exactly_one(3)),
Badge('Beryllium',               'Contains exactly 1 "4".',                    114791257,exactly_one(4)),
Badge('Boron',                   'Contains exactly 1 "5".',                    114791256,exactly_one(5)),
Badge('Carbon',                  'Contains exactly 1 "6".',                    114791256,exactly_one(6)),
Badge('Nitrogen',                'Contains exactly 1 "7".',                    114791256,exactly_one(7)),
Badge('Oxygen',                  'Contains exactly 1 "8".',                    114791256,exactly_one(8)),
Badge('Fluorine',                'Contains exactly 1 "9".',                    114791256,exactly_one(9)),
Badge('Mini Echo',               'Adjacent 2-digit repeat.',                   16346101, mini_echo()),
Badge('Medium Echo',             'Adjacent 3-digit repeat.',                   1110001,  medium_echo()),
Badge('Major Echo',              'Adjacent 4-digit repeat.',                   57001,    major_echo()),
Badge('Mini Palindrome 3',       'Contains a 3-digit palindrome.',             156510931,mini_palindrome(3)),
Badge('Mini Palindrome 4',       'Contains a 4-digit palindrome.',             17565718, mini_palindrome(4)),
Badge('Mini Palindrome 5',       'Contains a 5-digit palindrome.',             14710441, mini_palindrome(5)),
Badge('Mini Palindrome 6',       'Contains a 6-digit palindrome.',             1198264,  mini_palindrome(6)),
Badge('Mini Palindrome 7',       'Contains a 7-digit palindrome.',             899131,   mini_palindrome(7)),
Badge('Mini Palindrome 8',       'Contains a 8-digit palindrome.',             59998,    mini_palindrome(8)),
Badge('Binary Clean',            'Divisible by 1024.',                         292969,   divisible(1024)),
Badge('Double Binary Clean',     'Divisible by 1048576.',                      286,      divisible(1048576)),
Badge('Harshed Number',          'Divisible by sum of its digits.',            19647577, harshed()),
Badge('Beginner Feather',        'Digit sum ≤ 25.',                            19085851, digit_sum_le(25)),
Badge('Beginner Heavy',          'Digit sum ≥ 50.',                            24237105, digit_sum_ge(50)),
Badge('Intermediate Feather',    'Digit sum ≤ 20.',                            4515006,  digit_sum_le(20)),
Badge('Intermediate Heavy',      'Digit sum ≥ 55.',                            6242979,  digit_sum_ge(55)),
Badge('Advanced Feather',        'Digit sum ≤ 15.',                            643591,   digit_sum_le(15)),
Badge('Advanced Heavy',          'Digit sum ≥ 60.',                            997998,   digit_sum_ge(60)),
Badge('Expert Feather',          'Digit sum ≤ 10.',                            43616,    digit_sum_le(10)),
Badge('Expert Heavy',            'Digit sum ≥ 65.',                            80930,    digit_sum_ge(65)),
Badge('Grandmaster Feather',     'Digit sum ≤ 5.',                             706,      digit_sum_le(5)),
Badge('Grandmaster Heavy',       'Digit sum ≥ 70.',                            1947,     digit_sum_ge(70)),
Badge('German Whisper',          'All consecutive digits differ ≥ 5.',         64763,    german_whisper()),
Badge('Turtle',                  'All consecutive digits differ ≤ 1.',         15104,    turtle()),
Badge('Full House',              'Contains a triplet and a pair.',             122770458,full_house()),
Badge('Two Triplet',             'Contains two different triplets.',           13851054, two_triplet()),
Badge('Echo Form',               '2-digit pattern repeated 3 times.',          111001,   echo_form()),
Badge('Echo Form+',              '2-digit pattern repeated 4 times.',          571,      echo_form_plus()),
Badge('Echo Form Echo Form',     '3-digit pattern repeated 3 times.',          300,      echo_form_echo_form()),
Badge('Consecutive Pairs',       '3 consecutive-value pairs.',                 9601368,  consecutive_pairs()),
Badge('Airplane',                '2 consecutive-value triplets.',              2978906,  airplane()),
Badge('Zipper',                  'Two digits alternating perfectly.',          30,       zipper()),
Badge('Three Triplet',           'Contains three different triplets.',         60480,    three_triplet()),
Badge('Triple Airplane',         '3 consecutive-value triplets.',              4480,     triple_airplane()),
Badge('Consecutive Pairs+',      '4 consecutive-value pairs.',                 370440,   consecutive_pairs_plus()),
Badge('Neutrality',              'Reverse pairs == order pairs.',              12979991, neutrality()),
Badge('Even Spacing (Absolute)', 'All absolute spacings equal.',               848,      even_spacing_absolute()),
Badge('Almost Sorted',           'Exactly 1 reverse or order pair.',           94943,    almost_sorted()),
Badge('Skipping',                'No two digits differ by 1.',                 3182417,  skipping()),
Badge('Six Six Six',             'Contains 666.',                              1649217,  contains('666')),
Badge('Calender',                'Contains 365.',                              2794201,  contains('365')),
Badge('Emergency',               'Contains 911.',                              1798200,  contains('911')),
Badge('Not Found',               'Contains 404.',                              1786266,  contains('404')),
Badge('Slience in Sea',          'Contains 543.',                              1798200,  contains('543')),
Badge('Hopscotch',               'Digit every other position ×3.',             13940401, hopscotch()),
Badge('Hyper Hopscotch',         'Digit every other position ×4.',             869431,   hyper_hopscotch()),
Badge('Ultra Hopscotch',         'Digit every other position ×5.',             30000,    ultra_hopscotch()),
Badge('Slience in Sea 69 56',    'Contains 5436956.',                          30,       contains('5436956'))
]

def have_badge(x: int, y: int) -> bool:
    if y < 1 or y >= len(BADGES):
        return False
    return BADGES[y].test(x)

