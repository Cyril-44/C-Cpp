#!/usr/bin/env python3
"""Independent statement oracle, formula audit, and reproducible executable fuzzing.

Input layout: n, then n array values, then m, then m operations.
Example (run from the package root):
  python3 review/oracle_fuzz.py --std ../../work/linear/bin/std.exe --cases 3000
  python3 review/oracle_fuzz.py --self-check-only --self-check 10000

Expected values come only from direct array operations and arithmetic window sums.
The difference-based implementation below is confined to the formula self-check.
Every failing input, expected output, stdout, stderr, and metadata is saved.
"""

from __future__ import annotations

import argparse
import itertools
import json
import random
import subprocess
import sys
import time
from dataclasses import dataclass
from pathlib import Path

P = 998244353
EDGE_VALUES = (0, 1, 2, P - 1, P - 2, P // 2, (P + 1) // 2)


@dataclass
class Case:
    name: str
    array: list[int]
    operations: list[tuple[int, ...]]

    def text(self) -> str:
        return (f"{len(self.array)}\n" + " ".join(map(str, self.array)) + "\n"
                + f"{len(self.operations)}\n"
                + "".join(" ".join(map(str, op)) + "\n" for op in self.operations))


def statement_update(a: list[int], operation: tuple[int, ...]) -> int | None:
    """Apply the literal operation, with no difference array or periodicity."""
    kind, left, right, *args = operation
    l, r = left - 1, right - 1
    if kind == 1:
        k, = args
        windows = [sum(a[l + t:l + t + k]) % P for t in range(2)]
        first_step = (windows[1] - windows[0]) % P
        # Each successive target sum contains exactly one unknown element.
        # Re-summing the known k-1 terms also separates this oracle from brute.cpp.
        target = windows[1]
        for end in range(l + k + 1, r + 1):
            target = (target + first_step) % P
            a[end] = (target - sum(a[end - k + 1:end])) % P
    elif kind == 2:
        u, v = args
        for i in range(l, r + 1):
            a[i] = (u * a[i] + v) % P
    elif kind == 3:
        return sum(a[l:r + 1]) % P
    else:
        raise ValueError(f"Unknown operation: {operation}")
    return None


def expected_output(case: Case) -> list[int]:
    a = [x % P for x in case.array]
    answers = []
    for operation in case.operations:
        answer = statement_update(a, operation)
        if answer is not None:
            answers.append(answer)
    return answers


def parse_case(path: Path) -> Case:
    values = iter(map(int, path.read_text(encoding="utf-8").split()))
    try:
        n = next(values)
        a = [next(values) for _ in range(n)]
        m = next(values)
        operations = []
        for _ in range(m):
            kind = next(values)
            count = {1: 3, 2: 4, 3: 2}[kind]
            fields = [next(values) for _ in range(count)]
            operations.append((kind, *fields))
    except (StopIteration, KeyError) as exc:
        raise ValueError(f"Malformed input: {path}") from exc
    if next(values, None) is not None:
        raise ValueError(f"Trailing tokens in {path}")
    case = Case(path.stem, a, operations)
    validate_case(case)
    return case


def validate_case(case: Case) -> None:
    n = len(case.array)
    if n < 1:
        raise ValueError("n must be positive")
    for op in case.operations:
        kind, l, r, *args = op
        if not 1 <= l <= r <= n:
            raise ValueError(f"Bad interval: {op}")
        if kind == 1 and not (len(args) == 1 and 1 <= args[0] <= r - l):
            raise ValueError(f"Bad k: {op}")
        if kind not in (1, 2, 3) or len(args) != {1: 1, 2: 2, 3: 0}[kind]:
            raise ValueError(f"Bad operation: {op}")


def observe_all(n: int) -> list[tuple[int, ...]]:
    return [(3, 1, n)] + [(3, i, i) for i in range(1, n + 1)]


def fixed_cases() -> list[Case]:
    cases = []
    for n in (1, 2, 3):
        operations = observe_all(n)
        for l in range(1, n + 1):
            for r in range(l, n + 1):
                for u, v in ((0, 0), (0, P - 1), (1, 1), (P - 1, P - 1), (2, 3)):
                    operations += [(2, l, r, u, v)] + observe_all(n)
        if n >= 2:
            operations += [(1, 1, n, 1)] + observe_all(n)
        cases.append(Case(f"tiny_{n}", [P - 1, 0, 1][:n], operations))

    for n in (4, 5, 8, 13, 24):
        a = [(pow(3, i + 1, P) + i * i * 17) % P for i in range(n)]
        operations = observe_all(n)
        intervals = [(1, n), (1, n - 1), (2, n), (2, n - 1), (n - 2, n)]
        for l, r in intervals:
            if r - l < 1:
                continue
            for k in sorted({1, max(1, (r - l) // 2), r - l}):
                operations += [(1, l, r, k)] + observe_all(n)
                operations += [(1, l, r, k)] + observe_all(n)  # Idempotence.
                operations += [(2, l, r, 0, P - 1), (1, l, r, k)] + observe_all(n)
                operations += [(2, max(l, r - 1), r, P - 1, P - 1)]
        cases.append(Case(f"boundaries_{n}", a, operations))

    # W=(3, 6, ...), so the continued windows are 9, 12, 15, 18.
    cases.append(Case("arithmetic_windows_k2", [1, 2, 4, 8, 99, 77, 66],
                      [(1, 1, 7, 2)] + observe_all(7)))
    # Repeatedly overwrite both inside and outside the protected prefix.
    operations = []
    for t in range(60):
        operations += [(1, 1, 17, 3), (2, 1 + t % 17, 1 + t % 17, t % 3, P - 1),
                       (1, 2, 16, 1 + t % 13), (3, 1, 17), (3, 2, 16)]
    cases.append(Case("repeated_mixed", list(range(17)), operations))
    return cases


def pick_value(rng: random.Random) -> int:
    return rng.choice(EDGE_VALUES) if rng.random() < 0.55 else rng.randrange(P)


def pick_interval(rng: random.Random, n: int, minimum: int = 1) -> tuple[int, int]:
    if n < minimum:
        raise ValueError("Interval longer than array")
    mode = rng.randrange(6)
    if mode == 0:
        return 1, n
    if mode == 1:
        return 1, rng.randint(minimum, n)
    if mode == 2:
        return rng.randint(1, n - minimum + 1), n
    l = rng.randint(1, n - minimum + 1)
    return l, rng.randint(l + minimum - 1, n)


def random_case(seed: int, index: int, n_max: int, m_max: int) -> Case:
    # A case depends on its seed only, so one failing test is independently replayable.
    rng = random.Random(seed)
    n = rng.randint(1, n_max)
    a = [pick_value(rng) for _ in range(n)]
    if rng.randrange(6) == 0:
        a = [rng.choice(EDGE_VALUES)] * n
    operations = []
    for _ in range(rng.randint(1, m_max)):
        kind = rng.choices((1, 2, 3), (42 if n >= 2 else 0, 38, 20))[0]
        l, r = pick_interval(rng, n, 2 if kind == 1 else 1)
        if kind == 1:
            maximum = r - l
            k = rng.choice((1, maximum, rng.randint(1, maximum)))
            operations.append((1, l, r, k))
        elif kind == 2:
            operations.append((2, l, r, pick_value(rng), pick_value(rng)))
        else:
            operations.append((3, l, r))
        # Observe local and global consequences without depending on later updates.
        if rng.random() < 0.20:
            operations += [(3, 1, n), (3, l, r)]
        if rng.random() < 0.06:
            operations += [(3, i, i) for i in range(1, n + 1)]
    operations += observe_all(n)
    return Case(f"random_{index:06d}_seed_{seed}", a, operations)


def difference_array(a: list[int]) -> list[int]:
    return [(a[i] - (a[i - 1] if i else 0)) % P for i in range(len(a))]


def formula_update(a: list[int], operation: tuple[int, ...]) -> list[int]:
    """Proposed difference formulas, used for self-audit, never as the oracle."""
    e = difference_array(a)
    kind, left, right, *args = operation
    l, r, n = left - 1, right - 1, len(a)

    def add_at(i: int, delta: int) -> None:
        if i < n:
            e[i] = (e[i] + delta) % P

    if kind == 1:
        k, = args
        old = e[l + 1:r + 1]
        new = [old[j % k] for j in range(len(old))]
        ds = (sum(new) - sum(old)) % P
        e[l + 1:r + 1] = new
        add_at(r + 1, -ds)
    elif kind == 2:
        u, v = args
        old_am1 = a[l - 1] if l >= 1 else 0
        old_ar = a[r]
        for i in range(l, r + 1):
            e[i] = u * e[i] % P
        add_at(l, (u - 1) * old_am1 + v)
        add_at(r + 1, -(u - 1) * old_ar - v)
    return e


def verify_operation(a: list[int], operation: tuple[int, ...]) -> None:
    direct = a.copy()
    statement_update(direct, operation)
    candidate_e = formula_update(a, operation)
    if candidate_e != difference_array(direct):
        raise AssertionError(f"Difference formula mismatch: a={a}, op={operation}, "
                             f"direct={direct}, e={candidate_e}")
    if operation[0] == 1:
        _, left, right, k = operation
        l, r = left - 1, right - 1
        if direct[:l + k + 1] != a[:l + k + 1] or direct[r + 1:] != a[r + 1:]:
            raise AssertionError("Protected elements changed")
        w = [sum(direct[s:s + k]) % P for s in range(l, r - k + 2)]
        if any((w[i + 2] - 2 * w[i + 1] + w[i]) % P
               for i in range(len(w) - 2)):
            raise AssertionError("Window sequence has nonzero second difference")
        repeated = direct.copy()
        statement_update(repeated, operation)
        if repeated != direct:
            raise AssertionError("Calibration is not idempotent")
        step = (a[l + k] - a[l]) % P
        if any((direct[i] - direct[i - k]) % P != step
               for i in range(l + k, r + 1)):
            raise AssertionError("Independent direct-array block formula mismatch")
    # Independently check both moment reconstruction identities for every prefix.
    m0 = m1 = actual_prefix = 0
    for j, e in enumerate(candidate_e):
        m0 = (m0 + e) % P
        m1 = (m1 + j * e) % P
        t = j + 1
        actual_prefix = (actual_prefix + direct[j]) % P
        if m0 != direct[j]:
            raise AssertionError("Point moment reconstruction mismatch")
        if (t * m0 - m1) % P != actual_prefix:
            raise AssertionError("Prefix moment reconstruction mismatch")


def self_check(seed: int, random_checks: int) -> dict[str, int]:
    exhaustive = 0
    for n in range(2, 7):
        for values in itertools.product((0, 1, P - 1), repeat=n):
            a = list(values)
            for l in range(1, n):
                for r in range(l + 1, n + 1):
                    for k in range(1, r - l + 1):
                        verify_operation(a, (1, l, r, k))
                        exhaustive += 1
    exhaustive_affine = 0
    for n in range(1, 6):
        for values in itertools.product((0, 1, P - 1), repeat=n):
            a = list(values)
            for l in range(1, n + 1):
                for r in range(l, n + 1):
                    for u, v in itertools.product((0, 1, P - 1), repeat=2):
                        verify_operation(a, (2, l, r, u, v))
                        exhaustive_affine += 1
    rng = random.Random(seed ^ 0xA11D17)
    for _ in range(random_checks):
        n = rng.randint(1, 24)
        a = [pick_value(rng) for _ in range(n)]
        if n >= 2 and rng.randrange(2):
            l, r = pick_interval(rng, n, 2)
            operation = (1, l, r, rng.randint(1, r - l))
        else:
            l, r = pick_interval(rng, n)
            operation = (2, l, r, pick_value(rng), pick_value(rng))
        verify_operation(a, operation)
    return {"exhaustive_operation1": exhaustive, "exhaustive_affine": exhaustive_affine,
            "random_formula_checks": random_checks}


def save_failure(case: Case, answers: list[int], candidate: str, stdout: str,
                 stderr: str, reason: str, options: argparse.Namespace,
                 command: list[str]) -> Path:
    folder = options.failure_dir / case.name
    folder.mkdir(parents=True, exist_ok=True)
    input_path = folder / "input.in"
    input_path.write_text(case.text(), encoding="utf-8")
    (folder / "expected.out").write_text("".join(f"{x}\n" for x in answers), encoding="utf-8")
    (folder / "actual.out").write_text(stdout, encoding="utf-8")
    (folder / "stderr.txt").write_text(stderr, encoding="utf-8")
    metadata = {"case": case.name, "master_seed": options.seed,
                "candidate": candidate, "command": command, "reason": reason,
                "replay": [sys.executable, str(Path(__file__).resolve()), "--std", command[0],
                           *(f"--std-arg={arg}" for arg in command[1:]),
                           "--input", str(input_path.resolve()), "--self-check", "0"]}
    (folder / "metadata.json").write_text(json.dumps(metadata, indent=2), encoding="utf-8")
    return input_path


def run_candidate(case: Case, answers: list[int], candidate: str, command: list[str],
                  options: argparse.Namespace) -> None:
    stdout = stderr = ""
    reason = ""
    try:
        result = subprocess.run(command, input=case.text(), text=True, encoding="utf-8", errors="replace",
                                stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                timeout=options.timeout, check=False)
        stdout, stderr = result.stdout, result.stderr
        if result.returncode:
            reason = f"exit code {result.returncode}"
        else:
            try:
                actual = list(map(int, stdout.split()))
            except ValueError:
                reason = "non-integer output"
            else:
                if actual != answers:
                    mismatch = next((i for i, (x, y) in enumerate(zip(actual, answers))
                                     if x != y), min(len(actual), len(answers)))
                    reason = (f"wrong answer at answer {mismatch + 1}; "
                              f"expected {len(answers)} answers, got {len(actual)}")
    except subprocess.TimeoutExpired as exc:
        stdout = exc.stdout or ""
        stderr = exc.stderr or ""
        if isinstance(stdout, bytes):
            stdout = stdout.decode("utf-8", errors="replace")
        if isinstance(stderr, bytes):
            stderr = stderr.decode("utf-8", errors="replace")
        reason = f"timeout after {options.timeout}s"
    except OSError as exc:
        reason = str(exc)
    if reason:
        saved = save_failure(case, answers, candidate, stdout, stderr, reason, options, command)
        raise RuntimeError(f"{candidate}: {case.name}: {reason}; input saved to {saved}")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--std", type=Path, help="candidate executable")
    parser.add_argument("--std-arg", action="append", default=[], help="extra candidate argument; repeatable")
    parser.add_argument("--brute", type=Path, help="optional independently compiled C++ brute")
    parser.add_argument("--reference", type=Path, action="append", default=[],
                        help="additional executable; repeatable for independent full references")
    parser.add_argument("--input", type=Path, help="replay one input instead of generating tests")
    parser.add_argument("--seed", type=int, default=20261022)
    parser.add_argument("--case-seed", type=int, help="generate exactly one random case with this seed")
    parser.add_argument("--cases", type=int, default=1000)
    parser.add_argument("--n-max", type=int, default=48)
    parser.add_argument("--m-max", type=int, default=120)
    parser.add_argument("--timeout", type=float, default=5.0)
    parser.add_argument("--self-check", type=int, default=2000,
                        help="random formula audits before fuzzing; 0 skips all formula audits")
    parser.add_argument("--self-check-only", action="store_true")
    parser.add_argument("--failure-dir", type=Path, default=Path(__file__).resolve().parent / "oracle_failures")
    parser.add_argument("--report", type=Path, help="save final JSON summary to this file")
    parser.add_argument("--progress-every", type=int, default=100)
    options = parser.parse_args()
    if options.n_max < 1 or options.m_max < 1 or options.cases < 0 or options.self_check < 0:
        parser.error("n-max and m-max must be positive; cases and self-check must be nonnegative")
    if not options.self_check_only and options.std is None:
        parser.error("provide --std, or use --self-check-only")
    started = time.perf_counter()
    audit = self_check(options.seed, options.self_check) if options.self_check else {}
    if options.self_check_only:
        summary = {"status": "ok", **audit,
                   "seconds": round(time.perf_counter() - started, 3)}
        if options.report:
            options.report.parent.mkdir(parents=True, exist_ok=True)
            options.report.write_text(json.dumps(summary, indent=2) + "\n", encoding="utf-8")
        print(json.dumps(summary))
        return 0
    candidates = [("std", [str(options.std.resolve()), *options.std_arg])]
    if options.brute:
        candidates.append(("brute", [str(options.brute.resolve())]))
    for index, path in enumerate(options.reference, 1):
        candidates.append((f"reference{index}", [str(path.resolve())]))
    if options.input:
        cases = iter([parse_case(options.input)])
    elif options.case_seed is not None:
        cases = iter([random_case(options.case_seed, 0, options.n_max, options.m_max)])
    else:
        rng = random.Random(options.seed)
        cases = itertools.chain(fixed_cases(),
                                (random_case(rng.getrandbits(64), i, options.n_max, options.m_max)
                                 for i in range(options.cases)))
    checked = operation_count = query_count = 0
    for case in cases:
        validate_case(case)
        answers = expected_output(case)
        for name, command in candidates:
            run_candidate(case, answers, name, command, options)
        checked += 1
        operation_count += len(case.operations)
        query_count += len(answers)
        if options.progress_every and checked % options.progress_every == 0:
            print(f"checked {checked} cases, {operation_count} operations, {query_count} answers", flush=True)
    summary = {"status": "ok", "seed": options.seed, "cases": checked,
                      "operations": operation_count, "answers": query_count,
                      "candidates": [name for name, _ in candidates], **audit,
                      "seconds": round(time.perf_counter() - started, 3)}
    if options.report:
        options.report.parent.mkdir(parents=True, exist_ok=True)
        options.report.write_text(json.dumps(summary, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(summary))
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (AssertionError, RuntimeError, ValueError) as error:
        print(f"FAIL: {error}", file=sys.stderr)
        raise SystemExit(1)
