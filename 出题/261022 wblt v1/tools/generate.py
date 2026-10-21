#!/usr/bin/env python3
"""Deterministic, streaming official-input generator. No answers are generated."""

from __future__ import annotations

import argparse
import hashlib
import itertools
import json
from pathlib import Path
import random
import re
import sys
from typing import Iterator, TextIO


ROOT = Path(__file__).resolve().parents[1]
MOD = 998244353
ARRAY_MASK = 0xA0761D6478BD642F
OP_MASK = 0xE7037ED1A0B428DB
SMALL_PERIODS = (1, 2, 3, 4, 5, 7, 8, 15, 16, 17, 31, 32, 33,
                 63, 64, 65, 127, 128, 129, 255, 256, 257, 1023, 1024, 1025)
EDGE_VALUES = (0, 1, MOD - 1, MOD - 2, MOD // 2, 2)


def load_json(path: Path) -> dict:
    with path.open("r", encoding="utf-8") as src:
        return json.load(src)


def resolve_size(value: int | str, limits: dict) -> int:
    if isinstance(value, int):
        return value
    match = re.fullmatch(r"(N_MAX|M_MAX)([+-][0-9]+)?", value)
    if not match:
        raise ValueError(f"Unsupported size expression: {value!r}")
    key = "n_max" if match[1] == "N_MAX" else "m_max"
    return limits[key] + int(match[2] or 0)


def resolve_cases(spec: dict, limits: dict) -> list[dict]:
    result = []
    seen = set()
    for source in spec["cases"]:
        case = dict(source)
        if case["id"] in seen:
            raise ValueError(f"Duplicate case id: {case['id']}")
        seen.add(case["id"])
        case["n"] = resolve_size(case["n"], limits)
        case["m"] = resolve_size(case["m"], limits)
        n, m, group = case["n"], case["m"], case["group"]
        if not (1 <= n <= limits["n_max"] and 1 <= m <= limits["m_max"]):
            raise ValueError(f"Case {case['id']} exceeds global limits: n={n}, m={m}")
        if group == 1 and max(n, m) > limits["small_max"]:
            raise ValueError(f"Case {case['id']} exceeds group 1 limits")
        if group in (2, 3, 4) and max(n, m) <= limits["small_max"]:
            raise ValueError(f"Case {case['id']} falls inside group 1")
        if group == 4 and (max(n, m) > limits["medium_max"] or n < 4 or m < 2):
            raise ValueError(f"Case {case['id']} violates group 4 size constraints")
        if group == 5 and (max(n, m) <= limits["medium_max"] or n < 4 or m < 2):
            raise ValueError(f"Case {case['id']} violates group 5 size constraints")
        if group == 3 and (n < 3 or m < 2):
            raise ValueError(f"Case {case['id']} cannot contain calibration and query")
        if group not in range(1, 6):
            raise ValueError(f"Unknown group: {group}")
        result.append(case)
    return result


def initial_values(case: dict) -> Iterator[int]:
    rng = random.Random(case["seed"] ^ ARRAY_MASK)
    kind, n = case["array"], case["n"]
    if kind == "random":
        for _ in range(n):
            yield rng.randrange(MOD)
    elif kind == "edges":
        offset = rng.randrange(len(EDGE_VALUES))
        for i in range(n):
            yield EDGE_VALUES[(i + offset) % len(EDGE_VALUES)]
    elif kind == "periodic":
        period = (3, 7, 16, 31)[case["seed"] % 4]
        values = [rng.randrange(MOD) for _ in range(period)]
        for i in range(n):
            yield values[i % period]
    elif kind == "polynomial":
        # a_i = A*i^2+B*i+C; update by first differences to avoid huge integers.
        a, b, c = (rng.randrange(MOD) for _ in range(3))
        value, difference = (a + b + c) % MOD, (3 * a + b) % MOD
        second = 2 * a % MOD
        for _ in range(n):
            yield value
            value = (value + difference) % MOD
            difference = (difference + second) % MOD
    else:
        raise ValueError(f"Unknown array profile: {kind}")


def boundary_range(n: int, step: int, calibration: bool) -> tuple[int, int]:
    mode = step % 14
    if mode == 0:
        left, right = 1, n
    elif mode == 1:
        left, right = 1, max(1, n - 1)
    elif mode == 2:
        left, right = min(2, n), n
    elif mode == 3:
        left = right = 1
    elif mode == 4:
        left = right = n
    elif mode == 5:
        left = right = (n + 1) // 2
    elif mode in (6, 7):
        left, right = (1, min(n, 3)) if mode == 6 else (max(1, n - 2), n)
    elif mode in (8, 9):
        left, right = (1, min(n, 4)) if mode == 8 else (max(1, n - 3), n)
    else:
        length = min(n, SMALL_PERIODS[(step // 14 + mode) % len(SMALL_PERIODS)])
        left, right = (1, length) if mode & 1 else (n - length + 1, n)
    if calibration and right - left + 1 < 3:
        left = min(left, n - 2)
        right = left + 2
    return left, right


def choose_range(n: int, step: int, profile: str, rng: random.Random,
                 calibration: bool) -> tuple[int, int]:
    if profile == "noop":
        if calibration:
            return boundary_range(n, step, True) if step % 4 == 0 else (1, n)
        return 1, n
    if profile == "boundaries" or step % 16 == 0:
        return boundary_range(n, step // 16 if profile != "boundaries" else step,
                              calibration)
    if profile == "point" and (not calibration) and step % 5:
        point = 1 + ((step * 65537) % n)
        return point, point
    if profile == "offset":
        width = max(3 if calibration else 1, n - n // 3)
        left = 1 + ((step * 97 + (step // 64) * 257) % (n - width + 1))
        return left, left + width - 1
    if profile in ("nested", "gc"):
        phase = step % 128
        # Alternating prefixes/suffixes cross nested boundaries and old shared tails.
        inset = (phase // 2) * max(1, n // 256)
        inset = min(inset, max(0, n - (3 if calibration else 1)))
        left, right = (1, n - inset) if phase & 1 else (1 + inset, n)
        if profile == "gc" and phase % 7 == 0:
            center = 1 + ((step * 1009) % n)
            left, right = max(1, center - n // 8), min(n, center + n // 8)
        if calibration and right - left + 1 < 3:
            left = min(left, n - 2)
            right = left + 2
        return left, right
    mode = step % 10
    if mode == 0:
        return 1, n
    if mode == 1:
        return boundary_range(n, step // 10, calibration)
    if not calibration and mode == 2:
        point = rng.randrange(1, n + 1)
        return point, point
    if calibration:
        left = rng.randrange(1, n - 1)
        right = rng.randrange(left + 2, n + 1)
    else:
        left, right = sorted((rng.randrange(1, n + 1), rng.randrange(1, n + 1)))
    return left, right


def choose_k(length: int, step: int, profile: str, rng: random.Random) -> int:
    high = length - 2
    mode = step % 12
    if mode == 0:
        return 1
    if mode == 1:
        return high  # k+2=len: no changed term.
    if mode == 2:
        return max(1, high - 1)  # Exactly one changed term.
    if profile in ("small_k", "offset", "gc") or mode in (3, 4):
        return min(high, SMALL_PERIODS[(step // 12 + mode) % len(SMALL_PERIODS)])
    if profile == "large_k":
        return rng.randrange(max(1, high // 4), high + 1)
    return rng.randrange(1, high + 1)


def affine_parameters(step: int, rng: random.Random) -> tuple[int, int]:
    # All special multipliers recur throughout every modifying profile.
    mode = step % 8
    if mode == 0:
        return 0, EDGE_VALUES[(step // 8) % len(EDGE_VALUES)]
    if mode == 1:
        return 1, 0
    if mode == 2:
        return MOD - 1, MOD - 1
    if mode == 3:
        return 1, MOD - 1
    if mode == 4:
        return 0, 0
    if mode == 5:
        return MOD - 1, 1
    return rng.randrange(MOD), rng.randrange(MOD)


def sweep_triples(n: int) -> Iterator[tuple[int, int, int]]:
    return itertools.cycle((left, right, k)
                           for left in range(1, n - 1)
                           for right in range(left + 2, n + 1)
                           for k in range(1, right - left))


def operations(case: dict) -> Iterator[tuple[int, ...]]:
    n, m, group, profile = (case[key] for key in ("n", "m", "group", "profile"))
    rng = random.Random(case["seed"] ^ OP_MASK)
    sweep = sweep_triples(n) if profile == "sweep" else None
    affine_count = 0
    for step in range(m):
        # Certification of the group's existential condition is deterministic.
        if step == 0 and group in (3, 4, 5):
            # Use a vacuous calibration so certification preserves random seeds.
            yield (1, 1, 3, 1) if group == 3 else (1, 1, 4, 2)
            continue
        if step == m - 1:
            yield (3, 1, n)
            continue
        if profile == "preserved_seed":
            # The prefix [1, half] is never changed. Calibration's preserved
            # k+2 values end exactly at half; only the suffix is reconstructed.
            half = n // 2
            if n < 8:
                raise ValueError("preserved_seed requires n>=8")
            phase = step % 16
            if phase < 12:
                k = rng.randrange(min(max(2, n // 3), half - 2), half - 1)
                left = half - k - 1
                if step % 4 == 0:
                    right = n
                elif step % 4 == 1:
                    right = n - 1
                else:
                    right = rng.randrange(half + max(1, (n - half) // 2), n + 1)
                yield 1, left, right, k
            elif phase < 15:
                if phase == 12:
                    left, right = half + 1, n
                else:
                    left = rng.randrange(half + 1, n + 1)
                    right = left if phase == 13 else rng.randrange(left, n + 1)
                u, v = affine_parameters(affine_count, rng)
                affine_count += 1
                yield 2, left, right, u, v
            else:
                left = rng.randrange(1, half + 1)
                right = rng.randrange(half + 1, n + 1)
                yield 3, left, right
            continue
        pick = rng.randrange(100)
        if group == 2 or profile in ("affine", "point", "queries") or n < 3:
            op = 3 if profile == "queries" or pick >= 65 else 2
        elif profile == "gc":
            op = 1 if step % 16 < 12 else 2 if step % 16 < 15 else 3
        elif profile == "noop":
            op = 1 if pick < 75 else 2 if pick < 85 else 3
        elif profile == "sweep":
            op = 1 if step % 4 < 2 else 2 if step % 4 == 2 else 3
        else:
            op = 1 if pick < 58 else 2 if pick < 82 else 3
        if op == 1 and sweep is not None:
            left, right, k = next(sweep)
            yield 1, left, right, k
            continue
        left, right = choose_range(n, step, profile, rng, op == 1)
        if op == 1:
            k = 1 if group == 3 else choose_k(right - left + 1, step, profile, rng)
            yield 1, left, right, k
        elif op == 2:
            u, v = affine_parameters(affine_count, rng)
            affine_count += 1
            yield 2, left, right, u, v
        else:
            yield 3, left, right


def write_case(case: dict, destination: TextIO) -> None:
    destination.write(f"{case['n']}\n")
    values = initial_values(case)
    first_chunk = True
    while True:
        chunk = list(itertools.islice(values, 8192))
        if not chunk:
            break
        if not first_chunk:
            destination.write(" ")
        destination.write(" ".join(map(str, chunk)))
        first_chunk = False
    destination.write(f"\n{case['m']}\n")
    lines = []
    for op in operations(case):
        lines.append(" ".join(map(str, op)) + "\n")
        if len(lines) == 8192:
            destination.writelines(lines)
            lines.clear()
    destination.writelines(lines)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    selection = parser.add_mutually_exclusive_group()
    selection.add_argument("--case", action="append", metavar="ID", help="repeat to select cases")
    selection.add_argument("--group", type=int, choices=range(1, 6), help="select one scoring group")
    selection.add_argument("--all", action="store_true", help="explicitly select every case")
    parser.add_argument("--list", action="store_true", help="print resolved case metadata; write no inputs")
    parser.add_argument("--json", action="store_true", help="print metadata as JSON, implies --list")
    destination = parser.add_mutually_exclusive_group()
    destination.add_argument("--output", type=Path, help="one selected case only; otherwise stdout")
    destination.add_argument("--output-dir", type=Path, help="write ID.in and generation_manifest.json")
    parser.add_argument("--spec", type=Path, default=ROOT / "config" / "data_spec.json")
    parser.add_argument("--scoring", type=Path, default=ROOT / "config" / "scoring.json")
    parser.add_argument("--max", type=int, dest="maximum", help="override both global n,m limits")
    args = parser.parse_args()
    scoring, spec = load_json(args.scoring), load_json(args.spec)
    limits = dict(scoring["limits"])
    if args.maximum is not None:
        if args.maximum < 1:
            parser.error("--max must be positive")
        limits["n_max"] = limits["m_max"] = args.maximum
    if args.case:
        by_id = {case["id"]: case for case in spec["cases"]}
        unknown = set(args.case) - by_id.keys()
        if unknown:
            parser.error("unknown case ids: " + ", ".join(sorted(unknown)))
        if len(set(args.case)) != len(args.case):
            parser.error("duplicate --case selections")
        spec = {**spec, "cases": [by_id[case_id] for case_id in args.case]}
    elif args.group is not None:
        spec = {**spec, "cases": [case for case in spec["cases"] if case["group"] == args.group]}
    try:
        cases = resolve_cases(spec, limits)
    except ValueError as error:
        parser.error(str(error))
    if args.list or args.json or not (args.case or args.group or args.all):
        if args.json:
            print(json.dumps({"limits": limits, "cases": cases}, ensure_ascii=False, indent=2))
        else:
            for case in cases:
                print(f"{case['id']:<20} group={case['group']} n={case['n']} m={case['m']} "
                      f"seed={case['seed']} {case['array']}/{case['profile']}")
        return 0
    if not cases:
        parser.error("no cases selected")
    if args.output_dir is None and len(cases) != 1:
        parser.error("multiple cases require --output-dir")
    if args.output_dir is not None:
        args.output_dir.mkdir(parents=True, exist_ok=True)
        for case in cases:
            target = args.output_dir / f"{case['id']}.in"
            print(f"Generating {case['id']} (n={case['n']}, m={case['m']})", file=sys.stderr)
            with target.open("w", encoding="ascii", newline="\n", buffering=1 << 20) as dst:
                write_case(case, dst)
        manifest = {"generator_version": 1, "modulus": MOD, "limits": limits,
                    "python_version": sys.version.split()[0],
                    "generator_sha256": hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
                    "spec_sha256": hashlib.sha256(args.spec.read_bytes()).hexdigest(),
                    "cases": [{**case, "input": f"{case['id']}.in"} for case in cases]}
        (args.output_dir / "generation_manifest.json").write_text(
            json.dumps(manifest, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    elif args.output is not None:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        with args.output.open("w", encoding="ascii", newline="\n", buffering=1 << 20) as dst:
            write_case(cases[0], dst)
    else:
        if hasattr(sys.stdout, "reconfigure"):
            sys.stdout.reconfigure(encoding="ascii", newline="\n")
        write_case(cases[0], sys.stdout)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
