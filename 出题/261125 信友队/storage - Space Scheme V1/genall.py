#!/usr/bin/env python3
"""Build, generate and verify the v1 data with C++17 and Python 3.8+."""

import argparse
from collections import Counter
from concurrent.futures import ThreadPoolExecutor
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import tempfile
import time

ROOT = Path(__file__).resolve().parent
MOD = 998244353


def run(program, args=(), data=None):
    return subprocess.run(
        [str(program), *map(str, args)], input=data, capture_output=True, timeout=30
    )


def checked(program, args=(), data=None):
    result = run(program, args, data)
    if result.returncode:
        raise RuntimeError(f"{program.name}: {result.stderr.decode(errors='replace')}")
    return result


def build(cxx, directory):
    sources = [ROOT / "gen.cpp", ROOT / "val.cpp", ROOT / "verify/snapshot.cpp"]
    sources += sorted((ROOT / "std").glob("*.cpp"))
    sources += sorted((ROOT / "wrong").glob("*.cpp"))

    def compile_source(source):
        name = source.stem
        target = directory / (name + (".exe" if os.name == "nt" else ""))
        flags = ["-std=c++17", "-O2", "-Wall", "-Wextra"]
        if name == "val":
            flags.append("-DFOR_LINUX")  # Data files consistently use LF.
        result = run(cxx, [*flags, str(source), "-o", str(target)])
        if result.returncode:
            raise RuntimeError(f"{source}: {result.stderr.decode(errors='replace')}")
        return name, target

    with ThreadPoolExecutor(max_workers=4) as pool:
        return dict(pool.map(compile_source, sources))


def validate_rejections(val):
    cases = [
        (1, b"1 2 1\n+ 1 1\n"),
        (2, b"2 9 1\n+ 1 1\n"),
        (3, b"3 100000 21\n" + b"+ 1 1\n- 1 1\n" * 10 + b"+ 1 1\n"),
        (4, b"4 2 1\n+ 1 2\n"),
        (5, b"5 4 2\n+ 1 2\n+ 2 3\n"),
        (6, b"6 3 1\n+ 1 2\n"),
        (7, b"7 3 1\n+ 1 3\n"),
        (8, b"8 1 2\n+ 1 1\n- 1 1\n"),
        (9, b"9 3 1\n+ 2 3\n"),
    ]
    for group, data in cases:
        checked(val, ["--subtask=10"], data)
        if run(val, [f"--subtask={group}"], data).returncode == 0:
            raise ValueError(f"Validator accepted a subtask {group} counterexample")
    invalid = [
        b"10 1 1\n- 1 1\n", b"10 1 2\n+ 1 1\n+ 1 1\n", b"10 2 1\n+ 2 1\n",
        b"10 2 1\n+ 1 2\nextra\n", b"10 2 2\n+ 1 1\n", b"10 2 1\nx 1 2\n",
        b"10 2 1\n+ 1 2", b"10 100001 1\n+ 1 1\n", b"10 2 1\n+ 0 2\n",
        b"11 2 1\n+ 1 2\n",
    ]
    for data in invalid:
        if run(val, data=data).returncode == 0:
            raise ValueError(f"Validator accepted invalid input: {data!r}")
    return len(cases) + len(invalid)


def reference_programs(case):
    n, q, group = (case[k] for k in ("n", "q", "subtask"))
    if n <= 8 and q <= 100:
        yield "partial_brute"
    if n * q <= 2000000:
        yield "partial_recompute"
    if group in (1, 4, 5):
        yield "partial_disjoint"
    if group == 6:
        yield "partial_whole_singleton"
    if group == 7:
        yield "partial_short"


def assert_answer(data, answer, n, q):
    lines = answer.splitlines()
    if len(lines) != q:
        raise ValueError(f"Expected {q} answer lines, got {len(lines)}")
    for step, line in enumerate(lines, start=1):
        fields = line.split()
        if len(fields) != 2 or any(not token.isdigit() for token in fields):
            raise ValueError(f"Invalid output at operation {step}: {line!r}")
        cost, count = map(int, fields)
        if not (0 <= cost <= n and 0 <= count < MOD):
            raise ValueError(f"Answer outside valid range at operation {step}")
    if len(data.splitlines()) != q + 1:
        raise ValueError("Wrong number of input lines")


def gen_args(case):
    return [f"--tid={case['subtask']}",
            *(f"--{k}={case[k]}" for k in ("n", "q", "mode", "seed"))]


def verify_sample(programs):
    blocks = re.findall(r'\x60\x60\x60text\n(.*?)\x60\x60\x60', (ROOT / "statement.md").read_text("utf-8"), re.S)
    inp, ans = [block.encode() for block in blocks]
    checked(programs["val"], ["--subtask=2"], inp)
    for name in ("std", "partial_brute"):
        if checked(programs[name], data=inp).stdout.split() != ans.split():
            raise ValueError(f"{name} disagrees with the statement sample")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cxx", default=os.environ.get("CXX", "g++"))
    parser.add_argument("--data-dir", type=Path, default=ROOT / "data")
    args = parser.parse_args()
    data_dir = args.data_dir.resolve()
    data_dir.mkdir(parents=True, exist_ok=True)
    plan = json.loads((ROOT / "cases.json").read_text("utf-8"))
    cases = plan["cases"]
    group_counts = Counter()
    for case in cases:
        group = case["subtask"]
        group_counts[group] += 1
        if case["id"] != f"{group}_{group_counts[group]}":
            raise ValueError("Each case ID must be subtask_number, starting from 1 in each subtask")
    compiler = shutil.which(args.cxx)
    if not compiler:
        parser.error(f"Compiler not found: {args.cxx}")

    with tempfile.TemporaryDirectory(prefix="space-v1-build-") as build_dir, \
            tempfile.TemporaryDirectory(prefix=".space-v1-data-", dir=data_dir.parent) as stage_dir:
        programs = build(compiler, Path(build_dir))
        rejected = validate_rejections(programs["val"])
        verify_sample(programs)
        stage = Path(stage_dir)
        wrong_names = {p.stem for p in (ROOT / "wrong").glob("*.cpp")}
        killed, records = {}, []
        for index, c in enumerate(cases, start=1):
            case, n, q, group = (c[k] for k in ("id", "n", "q", "subtask"))
            data = checked(programs["gen"], gen_args(c)).stdout.replace(b"\r\n", b"\n")
            if index in (1, 11, len(cases)):
                repeat = checked(programs["gen"], gen_args(c)).stdout.replace(b"\r\n", b"\n")
                if repeat != data:
                    raise ValueError(f"Generator is not deterministic for case {case}")
            checked(programs["val"], [f"--subtask={group}"], data)
            start = time.perf_counter()
            answer = checked(programs["std"], data=data).stdout.replace(b"\r\n", b"\n")
            elapsed = time.perf_counter() - start
            assert_answer(data, answer, n, q)
            inp, ans = stage / f"{case}.in", stage / f"{case}.ans"
            inp.write_bytes(data)
            ans.write_bytes(answer)

            result = checked(programs["snapshot"], [inp, ans])
            snapshots = int(re.search(rb"(\d+) snapshots", result.stderr).group(1))
            expected = answer.split()
            full_checks = list(reference_programs(c))
            for name in full_checks:
                if checked(programs[name], data=data).stdout.split() != expected:
                    raise ValueError(f"Case {case}: {name} disagrees with std")
            for name in sorted(wrong_names - killed.keys()):
                wrong = checked(programs[name], data=data).stdout.split()
                if wrong != expected:
                    killed[name] = case
            records.append({"case": case, "subtask": group, "snapshots": snapshots,
                            "full_checks": full_checks, "std_seconds": round(elapsed, 3)})
            print(f"{index:2d}/{len(cases)}  {case:>4}  n={n:6d}  q={q:6d}  verified", flush=True)
        if wrong_names != killed.keys():
            raise ValueError(f"Surviving wrong solutions: {sorted(wrong_names - killed.keys())}")
        for c in cases:
            for suffix in ("in", "ans"):
                name = f"{c['id']}.{suffix}"
                os.replace(stage / name, data_dir / name)

    report = {"validator_rejections": rejected, "operations": sum(c["q"] for c in cases),
              "wrong_first_mismatch": killed, "cases": records}
    (ROOT / "verify/results.json").write_text(
        json.dumps(report, indent=2, ensure_ascii=False) + "\n", encoding="utf-8")
    print(f"Generated {len(cases)} pairs in {data_dir}. All {len(killed)} wrong solutions rejected.")


if __name__ == "__main__":
    main()
