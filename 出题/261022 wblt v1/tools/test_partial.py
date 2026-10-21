#!/usr/bin/env python3
"""Reproducible differential tests for the independent partial solution.

Example: python tools/test_partial.py --exe build/partial_special.exe
The oracle generates window sums directly and does not use second differences.
"""
import argparse
import hashlib
import json
from pathlib import Path
import platform
import random
import subprocess
import sys
import time

P = 998244353
ROOT = Path(__file__).resolve().parents[1]


def oracle(text):
    tokens = iter(map(int, text.split()))
    n = next(tokens)
    a = [next(tokens) for _ in range(n)]
    m = next(tokens)
    out = []
    for _ in range(m):
        op, l, r = next(tokens), next(tokens) - 1, next(tokens) - 1
        if op == 1:
            k = next(tokens)
            windows = [sum(a[l + j:l + j + k]) % P for j in range(3)]
            c = (windows[2] - 2 * windows[1] + windows[0]) % P
            for start in range(l + 3, r - k + 2):
                new_window = (2 * windows[-1] - windows[-2] + c) % P
                a[start + k - 1] = (new_window - sum(a[start:start + k - 1])) % P
                windows.append(new_window)
        elif op == 2:
            u, v = next(tokens), next(tokens)
            for i in range(l, r + 1):
                a[i] = (u * a[i] + v) % P
        else:
            out.append(str(sum(a[l:r + 1]) % P))
    return "\n".join(out) + "\n"


def random_case(seed, k1_only, n_max, m_min, m_max):
    rng = random.Random(seed)
    n, m = rng.randint(1, n_max), rng.randint(m_min, m_max)
    def value():
        return rng.choice([0, 1, P - 1, rng.randrange(P)])
    a = [value() for _ in range(n)]
    operations = []
    for _ in range(m - 1):
        op = rng.randint(1 if n >= 3 else 2, 3)
        if op == 1:
            l = rng.randint(1, n - 2)
            r = rng.randint(l + 2, n)
            k = 1 if k1_only else rng.randint(1, r - l - 1)
            operations.append(f"1 {l} {r} {k}")
        else:
            l = rng.randint(1, n)
            r = rng.randint(l, n)
            if op == 2:
                operations.append(f"2 {l} {r} {value()} {value()}")
            else:
                operations.append(f"3 {l} {r}")
    operations.append(f"3 1 {n}")
    text = f"{n}\n" + " ".join(map(str, a)) + f"\n{m}\n" + "\n".join(operations) + "\n"
    return text, m


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--exe", type=Path, required=True)
    parser.add_argument("--seed", type=int, default=770000)
    parser.add_argument("--cases", type=int, default=600)
    parser.add_argument("--n-max", type=int, default=80)
    parser.add_argument("--m-min", type=int, default=80)
    parser.add_argument("--m-max", type=int, default=220)
    parser.add_argument("--timeout", type=float, default=5)
    parser.add_argument("--report", type=Path, default=ROOT / "audit" / "partial_fuzz.json")
    parser.add_argument("--failure-dir", type=Path, default=ROOT / "build" / "partial_failure")
    args = parser.parse_args()
    if args.cases < 1 or args.n_max < 1 or not 1 <= args.m_min <= args.m_max:
        parser.error("require cases,n-max >= 1 and 1 <= m-min <= m-max")
    executable = args.exe.resolve()
    if not executable.is_file():
        parser.error(f"executable does not exist: {executable}")
    started = time.perf_counter()
    report = {"status": "running", "platform": platform.platform(), "python": platform.python_version(),
              "seed": args.seed, "cases": args.cases, "n_max": args.n_max,
              "m_min": args.m_min, "m_max": args.m_max,
              "oracle": "direct recurrence of window sums; no difference-array data structure",
              "executable_name": executable.name,
              "executable_sha256": hashlib.sha256(executable.read_bytes()).hexdigest(),
              "samples": {}, "random_passed": 0, "operations": 0, "answers": 0}
    def save():
        args.report.parent.mkdir(parents=True, exist_ok=True)
        args.report.write_text(json.dumps(report, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    def run(text, expected, case_id):
        try:
            proc = subprocess.run([str(executable)], input=text.encode(), stdout=subprocess.PIPE,
                                  stderr=subprocess.PIPE, timeout=args.timeout, check=True)
            got = proc.stdout.decode().replace("\r\n", "\n")
            if got != expected:
                raise AssertionError("output mismatch")
        except (subprocess.SubprocessError, UnicodeError, AssertionError) as exc:
            args.failure_dir.mkdir(parents=True, exist_ok=True)
            (args.failure_dir / f"{case_id}.in").write_text(text, encoding="utf-8")
            (args.failure_dir / f"{case_id}.out").write_text(expected, encoding="utf-8")
            report.update(status="failed", failure_case=case_id, error=str(exc))
            save()
            raise
        report["answers"] += len(expected.splitlines())
    for path in sorted((ROOT / "samples").glob("*.in")):
        text = path.read_text(encoding="utf-8")
        expected = path.with_suffix(".out").read_text(encoding="utf-8")
        assert oracle(text) == expected, f"sample oracle disagreement: {path.name}"
        run(text, expected, path.stem)
        report["samples"][path.stem] = "passed"
    split = (args.cases + 1) // 2
    for index in range(args.cases):
        text, m = random_case(args.seed + index, index >= split, args.n_max, args.m_min, args.m_max)
        run(text, oracle(text), f"random_{args.seed + index}")
        report["random_passed"] += 1
        report["operations"] += m
    report.update(status="passed", random_general_k_cases=split,
                  random_k1_only_cases=args.cases - split,
                  elapsed_seconds=round(time.perf_counter() - started, 3))
    save()
    print(f"PASS: {len(report['samples'])} samples, {args.cases} random cases; report={args.report}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
