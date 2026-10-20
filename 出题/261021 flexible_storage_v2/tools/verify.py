#!/usr/bin/env python3
"""Differential checks for the complete standard solution. Python 3.10+."""
import argparse
import json
from pathlib import Path
import random
import subprocess
import time

from common import ROOT, compile_cpp, executable
from gen import generate_case, SHAPES
from verify_tools import gray_operations


def encode(n, rows, operations):
    return (f"{n} {len(operations)}\n" +
            "".join(f"{a} {b} {w}\n" for a, b, w in rows) +
            "".join(f"{op} {l} {r}\n" for op, l, r in operations)).encode()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--build-dir", type=Path, default=ROOT / "build")
    parser.add_argument("--skip-compile", action="store_true")
    parser.add_argument("--cxx")
    args = parser.parse_args()
    args.build_dir.mkdir(parents=True, exist_ok=True)
    programs = ["std", "brute", "recompute", "bounded_height"]
    if not args.skip_compile:
        for name in programs:
            compile_cpp(ROOT / "solutions" / (name + ".cpp"),
                        executable(args.build_dir, name), args.cxx)
    records = []
    start = time.perf_counter()

    def check(label, n, rows, ops, brute=False):
        data = encode(n, rows, ops)
        outputs = {}
        names = ["std", "recompute", "bounded_height"] + (["brute"] if brute else [])
        for name in names:
            p = subprocess.run([str(executable(args.build_dir, name))],
                               input=data, capture_output=True, timeout=45)
            if p.returncode:
                raise RuntimeError(f"{label} {name}: {p.stderr.decode(errors='replace')}")
            outputs[name] = p.stdout.split()
        expected = outputs["recompute"]
        if len(expected) != 2 * len(ops) or any(out != expected for out in outputs.values()):
            (args.build_dir / "failure.in").write_bytes(data)
            for name, output in outputs.items():
                (args.build_dir / ("failure." + name)).write_bytes(b" ".join(output))
            raise AssertionError(label)
        records.append({"id": label, "n": n, "q": len(ops), "solvers": names})

    for n in range(1, 6):
        operations = [tuple(line.split()) for line in gray_operations(n)]
        operations = [(op, int(l), int(r)) for op, l, r in operations]
        for shape in ("mid", "left_comb", "right_comb", "zigzag_comb", "random"):
            for weights in ("unit", "tie", "large"):
                rows, _, _ = generate_case(n, 1, shape, "mixed", weights, n * 73 + len(records))
                check(f"gray-{n}-{shape}-{weights}", n, rows, operations, brute=True)
        print(f"exhaustive n={n} complete", flush=True)
    for seed in range(180):
        n = 6 + seed % 3 if seed < 36 else 9 + seed * 17 % 400
        q = 350 if n <= 8 else 700
        shape = SHAPES[seed % len(SHAPES)]
        mode = ("mixed", "adversarial", "alternating", "node_only", "short",
                "random")[seed % 6]
        rows, ops, _ = generate_case(n, q, shape, mode,
                                    ("unit", "tie", "large", "random")[seed % 4],
                                    20261021000 + seed)
        check(f"random-{seed}", n, rows, ops, brute=n <= 8)
        if seed % 30 == 29:
            print(f"random {seed+1}/180 complete", flush=True)
    for seed, shape in enumerate(SHAPES):
        rows, ops, _ = generate_case(1500, 2500, shape, "mixed", "tie", 71234 + seed)
        check("medium-" + shape, 1500, rows, ops)
    report = {"status": "passed", "cases": len(records),
              "operations": sum(x["q"] for x in records),
              "brute_operations": sum(x["q"] for x in records if "brute" in x["solvers"]),
              "elapsed_seconds": time.perf_counter() - start,
              "records": records}
    (ROOT / "reports").mkdir(exist_ok=True)
    (ROOT / "reports/differential.json").write_text(
        json.dumps(report, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({k: v for k, v in report.items() if k != "records"}), flush=True)


if __name__ == "__main__":
    main()
