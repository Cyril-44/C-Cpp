#!/usr/bin/env python3
"""Regenerate Subtask 4 cases using products of Fibonacci tiling counts.

Each factor F_i is realized by an all-white 2 x (i-1) rectangle. Independent
components are separated by one all-black column, so their tiling counts
multiply while the complete construction still has N=2.

The script updates only data/4_*.in, data/4_*.ans, subtask_4.zip, and data.zip.
It reads data/conf.json when assembling data.zip but never edits that file.
"""

from pathlib import Path
from zipfile import ZIP_DEFLATED, ZipFile


ROOT = Path(__file__).resolve().parent
DATA = ROOT / "data"
MAX_K = 10**18

# Fibonacci indexing: F_1 = F_2 = 1, F_i = F_{i-1} + F_{i-2}.
# Each tuple lists the indices of Fibonacci factors in K.
FACTOR_INDICES = [
    (2,),
    (3,),
    (4,),
    (3, 3),
    (3, 4),
    (4, 5),
    (5, 6),
    (3, 3, 3),
    (6, 7),
    (8, 9),
    (10, 10),
    (12, 13),
    (20, 25),
    (30, 35),
    (44, 45),
]


def fibonacci_values(max_index: int) -> list[int]:
    values = [0] * (max_index + 1)
    values[1] = values[2] = 1
    for i in range(3, max_index + 1):
        values[i] = values[i - 1] + values[i - 2]
    return values


def write_cases() -> list[tuple[int, tuple[int, ...]]]:
    fib = fibonacci_values(max(max(terms) for terms in FACTOR_INDICES))
    cases = []
    seen = set()

    for terms in FACTOR_INDICES:
        k = 1
        width = max(0, len(terms) - 1)  # one black separator between components
        for index in terms:
            k *= fib[index]
            width += index - 1

        assert 1 <= k <= MAX_K, (terms, k)
        assert width <= 10000, (terms, width)
        assert k not in seen, ("duplicate K", k)
        seen.add(k)
        cases.append((k, terms))

    for case_no, (k, _terms) in enumerate(cases, 1):
        stem = f"4_{case_no}"
        (DATA / f"{stem}.in").write_text(f"4 {k}\n", encoding="ascii")
        (DATA / f"{stem}.ans").write_bytes(b"")

    # Remove excess old Subtask 4 cases if the case count is reduced later.
    for path in DATA.glob("4_*.in"):
        if int(path.stem.split("_")[1]) > len(cases):
            path.unlink()
    for path in DATA.glob("4_*.ans"):
        if int(path.stem.split("_")[1]) > len(cases):
            path.unlink()
    return cases


def make_archives(case_count: int) -> None:
    subtask_zip = ROOT / "subtask_4.zip"
    with ZipFile(subtask_zip, "w", ZIP_DEFLATED) as archive:
        for case_no in range(1, case_count + 1):
            for suffix in ("in", "ans"):
                path = DATA / f"4_{case_no}.{suffix}"
                archive.write(path, arcname=path.name)

    # Preserve the current configuration byte-for-byte: it is only included.
    full_zip = ROOT / "data.zip"
    with ZipFile(full_zip, "w", ZIP_DEFLATED) as archive:
        for name in ("conf.json", "checker.cpp", "testlib.h"):
            archive.write(DATA / name, arcname=name)
        for path in sorted(DATA.glob("*.in")):
            archive.write(path, arcname=path.name)
        for path in sorted(DATA.glob("*.ans")):
            archive.write(path, arcname=path.name)


def main() -> None:
    cases = write_cases()
    make_archives(len(cases))
    print(f"Generated {len(cases)} Subtask 4 cases.")
    for case_no, (k, terms) in enumerate(cases, 1):
        factors = " * ".join(f"F_{index}" for index in terms)
        print(f"4_{case_no}: K={k} = {factors}")
    print("Updated subtask_4.zip and data.zip; data/conf.json was not modified.")


if __name__ == "__main__":
    main()
