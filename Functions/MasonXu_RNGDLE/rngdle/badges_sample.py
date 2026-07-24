import math
from dataclasses import dataclass
from typing import Callable, List, Tuple

# badge type constants (kept for compatibility)
TYPE_SPECIAL = 0
TYPE_NEEDAB = 1
TYPE_DIVIDE = 2
TYPE_CONTAIN = 3
TYPE_DIFFER = 4

from .utils import is_prime, MAXN


@dataclass
class Badge:
    name: str
    desc: str
    poss: int
    testtype: int
    test_fn: Callable[[int], bool]

    def test(self, x: int) -> bool:
        return bool(self.test_fn(x))


# helpers used by several tests

def _build_a(x: int) -> Tuple[str, List[int]]:
    s9 = str(x).rjust(9, '0')
    a = [0] * 10
    for i, ch in enumerate(s9, start=1):
        a[i] = int(ch)
    return s9, a


def _digit_counts(a: List[int]) -> List[int]:
    b = [0] * 10
    for i in range(1, 10):
        b[a[i]] += 1
    return b


# small factories (def style for readability and reuse)

def contains_factory(pat: str):
    def f(x: int) -> bool:
        s9, _ = _build_a(x)
        return pat in s9
    return f


def divisible_factory(d: int):
    def f(x: int) -> bool:
        return (x % d) == 0
    return f


# Sample BADGES list (1-based index: put None at 0) using explicit lambdas / defs
# These are examples for your review. If approved, all badges will be written similarly.
BADGES = [None,
    # 1: Clean Ten (divisible by 10)
    Badge("Clean Ten", "Ends with 0.", 30000001, TYPE_DIVIDE, divisible_factory(10)),
    # 2: Repeat 9 (a same digit exists 9 times)
    Badge("Repeat 9", "A same digit exists 9 times.", 3, TYPE_NEEDAB,
          lambda x: (lambda a: max([_ for _ in ((lambda b: b)[_])]) )(None) if False else (lambda x: (lambda s9,a: max(_digit_counts(a)) >= 9)(* _build_a(x)))(x)),
    # 3: Prime
    Badge("Prime", "Divisible only by 1 and itself.", 15574871, TYPE_SPECIAL, is_prime),
    # 4: Full House (triplet + pair)
    Badge("Full House", "Contains a triplet and a pair.", 122770458, TYPE_NEEDAB,
          lambda x: (lambda b: (lambda f: len(f) >= 2 and f[0] >= 3 and f[1] >= 2)(sorted([cnt for cnt in b if cnt > 0], reverse=True)))(_digit_counts(_build_a(x)[1])) ),
    # 5: Even Steven (all digits even)
    Badge("Even Steven", "All digits are even.", 390626, TYPE_NEEDAB,
          lambda x: sum(_digit_counts(_build_a(x)[1])[i] for i in [1,3,5,7,9]) == 0),
    # 6: Homo 3 (contains '114')
    Badge("Homo 3", "Contains 114.", 2794201, TYPE_CONTAIN, contains_factory('114')),
    # 7: Clean Million (divisible by 1_000_000)
    Badge("Clean Million", "Ends in 000000.", 301, TYPE_DIVIDE, divisible_factory(1000000)),
]

MAX_BADGE_INDEX = len(BADGES) - 1

# End of sample badges
