#!/usr/bin/env python3
"""Rebuild all private tests and public large samples from fixed seeds."""
import argparse
import json
from pathlib import Path
import subprocess
import time

from common import ROOT, compile_cpp, executable, run_solution, sha256
from gen import generate_case


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--build-dir", type=Path, default=ROOT / "build")
    ap.add_argument("--cxx")
    ap.add_argument("--skip-compile", action="store_true")
    ap.add_argument("--resume", action="store_true",
                    help="Reuse a case only when its parameters and both hashes match.")
    ap.add_argument("--only", nargs="*", help="Generate only these case IDs.")
    args = ap.parse_args()
    args.build_dir.mkdir(parents=True, exist_ok=True)
    for folder in ("data", "samples/large", "reports", "config"):
        (ROOT / folder).mkdir(parents=True, exist_ok=True)
    compilation = {}
    programs = {name: ROOT / "solutions" / (name + ".cpp")
                for name in ("std", "brute", "recompute", "bounded_height", "short")}
    programs.update({name: ROOT / "tools" / (name + ".cpp")
                     for name in ("val", "checker", "snapshot_oracle")})
    if not args.skip_compile:
        for name, source in programs.items():
            compilation[name] = compile_cpp(source, executable(args.build_dir, name), args.cxx)
        (ROOT / "reports/compilation.json").write_text(
            json.dumps(compilation, indent=2) + "\n", encoding="utf-8")
    plan = json.loads((ROOT / "tools/case_plan.json").read_text(encoding="utf-8"))
    all_cases = [dict(case, public=False) for case in plan["cases"]]
    all_cases += [dict(case, public=True) for case in plan["large_samples"]]
    manifest_path = ROOT / "manifest.json"
    previous = {}
    if args.resume and manifest_path.exists():
        previous = {x["id"]: x for x in json.loads(manifest_path.read_text(encoding="utf-8"))["cases"]}
    records = []
    started = time.perf_counter()
    generator_hash = sha256(ROOT / "tools/gen.py")
    standard_hash = sha256(ROOT / "solutions/std.cpp")
    for index, case in enumerate(all_cases):
        params = {key: case[key] for key in plan["cli_fields"]}
        inp, ans = ROOT / case["input"], ROOT / case["answer"]
        old = previous.get(case["id"])
        if args.only and case["id"] not in args.only:
            if old:
                records.append(old)
            continue
        reusable = (old and old.get("parameters") == params and
                    old.get("generator_sha256") == generator_hash and
                    old.get("std_sha256") == standard_hash and inp.exists() and ans.exists() and
                    sha256(inp) == old.get("input_sha256") and sha256(ans) == old.get("answer_sha256"))
        if reusable:
            records.append(old)
            print(f"{case['id']}: reused verified files", flush=True)
            continue
        rows, operations, metadata = generate_case(**params)
        inp.parent.mkdir(parents=True, exist_ok=True)
        ans.parent.mkdir(parents=True, exist_ok=True)
        with inp.open("w", encoding="ascii", newline="\n") as out:
            out.write(f"{case['n']} {case['q']}\n")
            for a, b, w in rows:
                out.write(f"{a} {b} {w}\n")
            for op, l, r in operations:
                out.write(f"{op} {l} {r}\n")
        del rows, operations
        subtask = case.get("subtask", 10)
        if subtask is None:
            subtask = 10
        validation = subprocess.run(
            [str(executable(args.build_dir, "val")), "--subtask", str(subtask),
             "--input", str(inp)], capture_output=True, text=True)
        if validation.returncode:
            raise RuntimeError(case["id"] + ": " + validation.stderr)
        elapsed = run_solution(executable(args.build_dir, "std"), inp, ans, timeout=120)
        # Stable answer hashes across Windows/Linux C++ text streams.
        ans.write_bytes(ans.read_bytes().replace(b"\r\n", b"\n"))
        checking = subprocess.run([str(executable(args.build_dir, "checker")),
                                   str(inp), str(ans), str(ans)], capture_output=True, text=True)
        if checking.returncode:
            raise RuntimeError(case["id"] + ": " + checking.stderr)
        record = dict(case, parameters=params, max_leaf_depth=metadata["max_leaf_depth"],
                      input_sha256=sha256(inp), answer_sha256=sha256(ans),
                      input_bytes=inp.stat().st_size, answer_bytes=ans.stat().st_size,
                      std_seconds=elapsed, generator_sha256=generator_hash,
                      std_sha256=standard_hash)
        records.append(record)
        manifest_path.write_text(json.dumps({
            "status": "generating", "cases": records,
            "elapsed_seconds": time.perf_counter() - started
        }, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
        print(f"{index+1}/{len(all_cases)} {case['id']}: "
              f"n={case['n']} q={case['q']} std={elapsed:.3f}s", flush=True)
    complete = len(records) == len(all_cases)
    manifest_path.write_text(json.dumps({
        "title": "自由分割，按价存储", "version": "v2", "date": "2026-10-21",
        "status": "generated" if complete else "partial", "cases": records,
        "private_cases": sum(not x["public"] for x in records),
        "large_samples": sum(x["public"] for x in records),
        "operations": sum(x["q"] for x in records),
        "elapsed_seconds": time.perf_counter() - started
    }, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    (ROOT / "config/cases.json").write_text(json.dumps({
        "cases": [{"id": x["id"], "subtask": x["subtask"],
                   "input": x["input"], "answer": x["answer"]}
                  for x in plan["cases"]]
    }, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")


if __name__ == "__main__":
    main()
