#!/usr/bin/env python3
"""Generate the sample set for 餐巾纸. Run: python3 generator.py [output_dir]"""

import random
import sys
from pathlib import Path


def main():
    out_dir = Path(sys.argv[1]) if len(sys.argv) > 1 else Path("data")
    out_dir.mkdir(parents=True, exist_ok=True)
    rng = random.Random(1007)

    # 01–03 belong to the 30-point subtask (x = 0).
    cases = [
        (0, 0),
        (0, 1),
        (0, 299),
        # 04–10 cover the full problem; several use the largest total.
        (2, 0),
        (598, 0),
        (2, 298),
        (400, 99),
    ]

    # Add three reproducible cases with varied x and a positive remainder.
    for _ in range(3):
        x = 2 * rng.randint(0, 299)
        y = rng.randint(0, 299 - x // 2)
        cases.append((x, y))

    for i, (x, y) in enumerate(cases, start=1):
        (out_dir / f"{i:02d}.in").write_text(f"{x} {y}\n", encoding="utf-8")
        answer = 300 - x // 2 - y
        (out_dir / f"{i:02d}.ans").write_text(f"{answer}\n", encoding="utf-8")


if __name__ == "__main__":
    main()
