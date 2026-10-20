#!/usr/bin/env python3
"""Compile and test the independent solvers, validator, and checker.

Usage: python tools/verify_tools.py --build-dir PATH [--cxx g++] [--skip-compile]
The report and temporary cases are written only below --build-dir (default:
./work/verify_tools). The package's test data and solutions are never modified.
"""

import argparse
import json
import os
import random
import subprocess
import time
from pathlib import Path

PACKAGE = Path(__file__).resolve().parents[1]
MOD = 998244353


def make_tree(n, shape="mid", rng=None, weights="unit", shuffle=False):
    """Return 1-based rows (left, right, weight), with root fixed at 1."""
    rng = rng or random.Random(0)
    rows = [None]

    def build(size):
        vertex = len(rows)
        rows.append(None)
        if size == 1:
            left = right = 0
        else:
            if shape == "mid":
                left_size = (size + 1) // 2
            elif shape == "floor":
                left_size = size // 2
            elif shape == "random":
                left_size = rng.randrange(1, size)
            else:
                raise ValueError(shape)
            left = build(left_size)
            right = build(size - left_size)
        weight = {"unit": 1, "size": size, "large": 10**9}.get(weights)
        if weight is None:
            weight = rng.randrange(1, 9)
        rows[vertex] = (left, right, weight)
        return vertex

    build(n)
    if shuffle:
        permutation = [0, 1] + rng.sample(range(2, len(rows)), len(rows) - 2)
        shuffled = [None] * len(rows)
        for old in range(1, len(rows)):
            left, right, weight = rows[old]
            shuffled[permutation[old]] = (permutation[left], permutation[right], weight)
        rows = shuffled
    return rows


def comb_tree(n, rightward=True, weight=1):
    rows = [None] + [(0, 0, weight)] * (2 * n - 1)
    for index in range(n - 1):
        vertex = 2 * index + 1
        left, right = vertex + 1, vertex + 2
        if not rightward:
            left, right = right, left
        rows[vertex] = (left, right, weight)
    return rows


def encode(rows, operations):
    n = len(rows) // 2
    return f"{n} {len(operations)}\n" + "".join(
        f"{a} {b} {w}\n" for a, b, w in rows[1:]
    ) + "\n".join(operations) + "\n"


def random_operations(n, q, rng):
    active = set()
    result = []
    for _ in range(q):
        l = rng.randrange(1, n + 1)
        r = rng.randrange(l, n + 1)
        interval = l, r
        if interval in active:
            active.remove(interval)
            result.append(f"- {l} {r}")
        else:
            active.add(interval)
            result.append(f"+ {l} {r}")
    return result


def gray_operations(n):
    intervals = [(l, r) for l in range(1, n + 1) for r in range(l, n + 1)]
    previous = 0
    result = []
    for step in range(1, 1 << len(intervals)):
        current = step ^ (step >> 1)
        bit = current ^ previous
        l, r = intervals[bit.bit_length() - 1]
        result.append(f"{'+' if current & bit else '-'} {l} {r}")
        previous = current
    l, r = intervals[previous.bit_length() - 1]
    result.append(f"- {l} {r}")
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cxx", default=os.environ.get("CXX", "g++"))
    parser.add_argument("--build-dir", type=Path, default=Path.cwd() / "work" / "verify_tools")
    parser.add_argument("--skip-compile", action="store_true")
    args = parser.parse_args()
    directory = args.build_dir.resolve()
    directory.mkdir(parents=True, exist_ok=True)
    started = time.perf_counter()
    suffix = ".exe" if os.name == "nt" else ""
    executables = {name: directory / (name + suffix) for name in ("brute", "recompute", "short", "val", "checker", "snapshot_oracle")}
    if not args.skip_compile:
        for name, executable in executables.items():
            folder = "solutions" if name in ("brute", "recompute", "short") else "tools"
            subprocess.run([
                args.cxx, "-std=c++17", "-O2", "-Wall", "-Wextra", "-Wshadow",
                str(PACKAGE / folder / (name + ".cpp")), "-o", str(executable),
            ], check=True)

    records = []

    def run(name, data=None, arguments=(), expected=0, label=None):
        result = subprocess.run(
            [str(executables[name])] + list(arguments), input=data,
            text=True, capture_output=True, timeout=90,
        )
        if result.returncode != expected:
            if data is not None:
                (directory / "failure.in").write_text(data, encoding="ascii")
            raise AssertionError(f"{label or name}: expected exit {expected}, got {result.returncode}: {result.stderr}")
        if label:
            records.append({"label": label, "exit_code": result.returncode, "diagnostic": result.stderr.strip()})
        return result.stdout

    def validate(label, data, arguments=(), expected=0):
        return run("val", data, arguments, expected, "val-" + label)

    def snapshot(label, data, answer, checks=None, expected=0):
        input_path = directory / "snapshot.in"
        answer_path = directory / "snapshot.ans"
        input_path.write_text(data, encoding="ascii")
        answer_path.write_text(answer, encoding="ascii")
        arguments = [str(input_path), str(answer_path)]
        if checks is not None:
            arguments.append(str(checks))
        run("snapshot_oracle", arguments=arguments, expected=expected, label="snapshot-" + label)

    def compare(label, rows, operations):
        data = encode(rows, operations)
        validate(label, data)
        first = run("brute", data)
        second = run("recompute", data)
        if first != second:
            (directory / "failure.in").write_text(data, encoding="ascii")
            (directory / "failure.brute").write_text(first, encoding="ascii")
            (directory / "failure.recompute").write_text(second, encoding="ascii")
            raise AssertionError(f"solver mismatch in {label}")
        assert len(first.splitlines()) == len(operations)
        records.append({"label": "solvers-" + label, "operations": len(operations), "status": "passed"})
        snapshot(label, data, first, checks=len(operations))
        return first

    sample_operations = ["+ 1 2", "+ 1 1", "+ 2 2", "- 1 1", "- 1 2", "- 2 2"]
    sample_tree = make_tree(2)
    sample_answer = compare("six-step-unit-example", sample_tree, sample_operations)
    assert sample_answer.splitlines() == ["1 1", "2 2", "2 1", "2 2", "1 1", "0 1"]
    tied_tree = [None, (2, 3, 2), (0, 0, 1), (0, 0, 1)]
    tied_answer = compare("weighted-tie", tied_tree, sample_operations)
    assert tied_answer.splitlines() == ["2 2", "2 1", "2 1", "2 1", "1 1", "0 1"]

    rng = random.Random(20261020)
    differential_operations = 0
    for n in range(1, 6):
        operations = gray_operations(n)
        for shape in ("mid", "random"):
            for weights in ("unit", "size", "random", "large"):
                rows = make_tree(n, shape, rng, weights, shuffle=True)
                compare(f"all-subsets-n{n}-{shape}-{weights}", rows, operations)
                differential_operations += len(operations)
    for n in (6, 7, 8):
        for weights in ("unit", "size", "random", "large"):
            rows = make_tree(n, "random", rng, weights, shuffle=True)
            operations = random_operations(n, 1000, rng)
            compare(f"random-n{n}-{weights}", rows, operations)
            differential_operations += len(operations)

    mid5 = make_tree(5)
    small = encode(mid5, ["+ 1 5"])
    bad_trees = [
        ("one-child", [None, (2, 0, 1), (0, 0, 1), (0, 0, 1)]),
        ("duplicate-child", [None, (2, 2, 1), (0, 0, 1), (0, 0, 1)]),
        ("out-of-range-child", [None, (2, 4, 1), (0, 0, 1), (0, 0, 1)]),
        ("negative-child", [None, (-1, 2, 1), (0, 0, 1), (0, 0, 1)]),
        ("self-loop", [None, (1, 2, 1), (0, 0, 1), (0, 0, 1)]),
        ("multiple-parents", [None, (2, 3, 1), (4, 5, 1), (4, 5, 1), (0, 0, 1), (0, 0, 1)]),
        ("disconnected-cycle", [None, (0, 0, 1), (3, 5, 1), (4, 6, 1), (2, 7, 1), (0, 0, 1), (0, 0, 1), (0, 0, 1)]),
        ("wrong-leaf-count", [None, (0, 0, 1), (0, 0, 1), (0, 0, 1)]),
        ("zero-weight", [None, (0, 0, 0)]),
        ("negative-weight", [None, (0, 0, -1)]),
        ("excessive-weight", [None, (0, 0, 1000000001)]),
    ]
    for label, rows in bad_trees:
        validate(label, encode(rows, ["+ 1 1"]), expected=1)
    for label, operations in [
        ("duplicate-insertion", ["+ 1 3", "+ 1 3"]),
        ("inactive-deletion", ["- 1 3"]),
        ("endpoint-too-small", ["+ 0 3"]),
        ("endpoint-too-large", ["+ 1 6"]),
        ("reversed-endpoints", ["+ 3 2"]),
        ("invalid-operation", ["* 1 3"]),
        ("non-integer-endpoint", ["+ 1.0 3"]),
        ("overflow-endpoint", ["+ 1 999999999999999999999999"]),
        ("missing-endpoint", ["+ 1"]),
    ]:
        validate(label, encode(mid5, operations), expected=1)
    validate("trailing-token", small + "extra\n", expected=1)
    validate("missing-operation", small.rsplit("+", 1)[0], expected=1)
    validate("n-zero", "0 1\n", expected=1)
    validate("n-excessive", "100001 1\n", expected=1)
    validate("q-zero", "1 0\n", expected=1)
    validate("q-excessive", "1 100001\n", expected=1)
    validate("overflow-header", "999999999999999999999 1\n", expected=1)
    validate("non-integer-weight", "1 1\n0 0 1.0\n+ 1 1\n", expected=1)
    validate("subtask1-valid", small, ["--subtask", "1"])
    validate("subtask1-n-limit", encode(make_tree(8), ["+ 1 1"]), ["--subtask", "1"], 1)
    validate("subtask1-q-limit", "1 101\n", ["--subtask", "1"], 1)
    validate("subtask2-valid", small, ["--subtask", "2"])
    validate("subtask2-product-limit", "100000 21\n", ["--subtask", "2"], 1)
    validate("subtask3-valid", small, ["--subtask", "3"])
    validate("subtask3-balanced-but-wrong-split", encode(make_tree(5, "floor"), ["+ 1 5"]), ["--subtask", "3"], 1)
    validate("subtask4-depth30-valid", encode(comb_tree(31), ["+ 1 31"]), ["--subtask", "4"])
    validate("subtask4-depth31-invalid", encode(comb_tree(32), ["+ 1 32"]), ["--subtask", "4"], 1)
    validate("subtask5-node-valid", encode(mid5, ["+ 1 3", "+ 4 5", "- 1 3"]), ["--subtask", "5"])
    validate("subtask5-nonnode-invalid", encode(mid5, ["+ 2 3"]), ["--subtask", "5"], 1)
    validate("subtask6-comb-valid", encode(comb_tree(5), ["+ 2 4"]), ["--subtask", "6"])
    validate("subtask6-noncomb-invalid", small, ["--subtask", "6"], 1)
    validate("subtask7-valid", encode(comb_tree(5), ["+ 2 4"]), ["--subtask", "7"])
    validate("subtask7-invalid", small, ["--subtask", "7"], 1)
    validate("subtask8-valid", encode(mid5, ["+ 1 3", "+ 4 5"]), ["--subtask", "8"])
    validate("subtask8-invalid", encode(mid5, ["+ 1 3", "- 1 3"]), ["--subtask", "8"], 1)
    validate("subtask9-valid", encode(mid5, ["+ 2 3", "+ 3 3"]), ["--subtask", "9"])
    validate("subtask9-invalid", encode(mid5, ["+ 2 4"]), ["--subtask", "9"], 1)
    validate("subtask10-valid", small, ["--subtask", "10"])
    validate("subtask-intersection", small, ["--subtask", "6", "--subtask", "10"], 1)
    validate("unknown-option", small, ["--unknown", "1"], 1)
    validate("missing-option-value", small, ["--subtask"], 1)
    validate("invalid-subtask", small, ["--subtask", "11"], 1)

    # Overlapping length-two requests can force the same leaf several times.
    for n in range(1, 9):
        universe = [(l, r) for l in range(1, n + 1) for r in range(l, min(n, l + 1) + 1)]
        for weights in ("unit", "size", "random", "large"):
            rows = make_tree(n, "random", rng, weights, shuffle=True)
            operations, active = [], set()
            for _ in range(500):
                l, r = rng.choice(universe)
                if (l, r) in active:
                    active.remove((l, r))
                    operations.append(f"- {l} {r}")
                else:
                    active.add((l, r))
                    operations.append(f"+ {l} {r}")
            data = encode(rows, operations)
            validate(f"short-n{n}-{weights}", data, ["--subtask", "9"])
            expected = run("brute", data)
            assert run("short", data) == expected, (n, weights)
            records.append({"label": f"short-brute-n{n}-{weights}", "operations": len(operations), "status": "passed"})
            differential_operations += len(operations)

    # This checks both iterative traversal and sums exceeding signed 32 bits.
    for rightward in (True, False):
        rows = comb_tree(100000, rightward, 10**9)
        l, r = (1, 99999) if rightward else (2, 100000)
        data = encode(rows, [f"+ {l} {r}", f"- {l} {r}"])
        validate(f"large-{'right' if rightward else 'left'}-comb", data)
        answer = run("recompute", data)
        assert answer.splitlines() == ["99999000000000 1", "0 1"]
        records.append({"label": f"solver-large-{'right' if rightward else 'left'}-comb", "status": "passed"})
        snapshot(f"large-{'right' if rightward else 'left'}-comb", data, answer)

    # Every requested singleton is forced, independently of tree shape. This
    # gives an exact large oracle without invoking any tree dynamic programming.
    large_rows = comb_tree(100000, weight=10**9)
    large_operations = [f"+ {leaf} {leaf}" for leaf in range(1, 100001)]
    large_data = encode(large_rows, large_operations)
    large_answers = [f"{leaf * 10**9} 1" for leaf in range(1, 100001)]
    large_answer = "\n".join(large_answers) + "\n"
    validate("large-n-and-q-short", large_data, ["--subtask", "9"])
    assert run("short", large_data) == large_answer
    records.append({"label": "short-large-exact-singletons", "status": "passed"})
    snapshot("large-exact-singletons", large_data, large_answer)
    for step in (1, 8, 50000, 100000):
        corrupted = large_answers.copy()
        corrupted[step - 1] = f"{step * 10**9} 2"
        snapshot(f"wrong-at-{step}", large_data, "\n".join(corrupted) + "\n", expected=1)
    sample_data = encode(sample_tree, sample_operations)
    snapshot("extra-answer-token", sample_data, sample_answer + "0\n", expected=1)
    snapshot("missing-answer-token", sample_data, "1\n", expected=1)
    snapshot("extra-input-token", sample_data + "extra\n", sample_answer, expected=1)
    snapshot("zero-uniform-checks", sample_data, sample_answer, checks=0)
    snapshot("invalid-check-count", sample_data, sample_answer, checks=-1, expected=1)

    input_path, answer_path, output_path = [directory / filename for filename in ("checker.in", "checker.ans", "checker.out")]
    input_path.write_text(encode(sample_tree, sample_operations), encoding="ascii")
    answer_path.write_text(sample_answer, encoding="ascii")
    run("val", arguments=["--input", str(input_path)], label="val-input-file")
    checker_arguments = [str(input_path), str(output_path), str(answer_path)]
    outputs = [
        ("valid", sample_answer, 0),
        ("arbitrary-whitespace", " \t" + "\t".join(sample_answer.split()) + " \n", 0),
        ("empty", "", 1),
        ("missing-token", " ".join(sample_answer.split()[:-1]), 1),
        ("extra-token", sample_answer + "0\n", 1),
        ("extra-word", sample_answer + "hello\n", 1),
        ("wrong-answer", sample_answer.replace("2 2", "2 1", 1), 1),
        ("decimal", sample_answer.replace("1 1", "1.0 1", 1), 1),
        ("nan", sample_answer.replace("1 1", "nan 1", 1), 1),
        ("negative-cost", sample_answer.replace("1 1", "-1 1", 1), 1),
        ("excessive-cost", sample_answer.replace("1 1", "2000000001 1", 1), 1),
        ("negative-count", sample_answer.replace("1 1", "1 -1", 1), 1),
        ("excessive-count", sample_answer.replace("1 1", f"1 {MOD}", 1), 1),
        ("overflow", sample_answer.replace("1 1", "999999999999999999999999 1", 1), 1),
    ]
    for label, output, expected in outputs:
        output_path.write_text(output, encoding="ascii")
        run("checker", arguments=checker_arguments, expected=expected, label="checker-" + label)
    output_path.write_text(sample_answer, encoding="ascii")
    for label, bad_answer in [
        ("missing-answer-token", "1"),
        ("extra-answer-token", sample_answer + "0\n"),
        ("non-integer-answer", sample_answer.replace("1 1", "oops 1", 1)),
        ("out-of-range-answer", sample_answer.replace("1 1", "-1 1", 1)),
    ]:
        answer_path.write_text(bad_answer, encoding="ascii")
        run("checker", arguments=checker_arguments, expected=3, label="checker-" + label)
    run("checker", arguments=[], expected=3, label="checker-missing-arguments")
    large_rows = make_tree(3, weights="large")
    large_data = encode(large_rows, ["+ 1 1", "+ 2 2", "+ 3 3"])
    input_path.write_text(large_data, encoding="ascii")
    large_answer = "1000000000 1\n2000000000 1\n3000000000 1\n"
    answer_path.write_text(large_answer, encoding="ascii")
    output_path.write_text(large_answer, encoding="ascii")
    run("checker", arguments=checker_arguments, label="checker-int64-answer")

    report = {
        "status": "passed", "checks": len(records),
        "differential_operations": differential_operations + 12,
        "elapsed_seconds": round(time.perf_counter() - started, 3), "results": records,
    }
    (directory / "verification.json").write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"PASS: {len(records)} checks; {report['differential_operations']} differential operations")
    print(f"Report: {directory / 'verification.json'}")


if __name__ == "__main__":
    main()
