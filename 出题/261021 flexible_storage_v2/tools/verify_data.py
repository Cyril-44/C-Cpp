#!/usr/bin/env python3
"""Validate all files/hashes and independently check generated answers."""
import argparse
import json
from pathlib import Path
import subprocess
import tempfile
import time

from common import ROOT, executable, run_solution, sha256


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--build-dir", type=Path, default=ROOT / "build")
    ap.add_argument("--snapshots", type=int, default=20)
    args = ap.parse_args()
    manifest = json.loads((ROOT / "manifest.json").read_text(encoding="utf-8"))
    if manifest["status"] != "generated":
        raise RuntimeError("Data generation is not complete")
    records = []
    started = time.perf_counter()
    with tempfile.TemporaryDirectory(prefix="flexible_verify_") as temp:
        for case in manifest["cases"]:
            inp, ans = ROOT / case["input"], ROOT / case["answer"]
            for kind, path in (("input", inp), ("answer", ans)):
                if sha256(path) != case[kind + "_sha256"]:
                    raise AssertionError(case["id"] + ": hash mismatch")
            subtask = case.get("subtask") or 10
            p = subprocess.run([str(executable(args.build_dir, "val")),
                                "--input", str(inp), "--subtask", str(subtask)],
                               capture_output=True, text=True)
            if p.returncode:
                raise AssertionError(case["id"] + ": " + p.stderr)
            p = subprocess.run([str(executable(args.build_dir, "snapshot_oracle")),
                                str(inp), str(ans), str(args.snapshots)],
                               capture_output=True, text=True, timeout=120)
            if p.returncode:
                raise AssertionError(case["id"] + ": " + p.stderr)
            solvers = []
            if case["n"] * case["q"] <= 2_000_000:
                solvers.append("recompute")
            if case["n"] <= 7 and case["q"] <= 100:
                solvers.append("brute")
            if case["max_leaf_depth"] <= 30:
                solvers.append("bounded_height")
            if subtask == 9:
                solvers.append("short")
            expected = ans.read_bytes().split() if solvers else None
            for solver in solvers:
                result = Path(temp) / (solver + ".out")
                run_solution(executable(args.build_dir, solver), inp, result, 60)
                if result.read_bytes().split() != expected:
                    raise AssertionError(case["id"] + ": " + solver + " disagrees")
            records.append({"id": case["id"], "status": "passed",
                            "snapshot_result": p.stderr.strip(),
                            "input_sha256": case["input_sha256"],
                            "answer_sha256": case["answer_sha256"],
                            "full_output_references": solvers})
            print(case["id"] + ": " + p.stderr.strip() +
                  ("; full " + ",".join(solvers) if solvers else ""), flush=True)
    report = {"status": "passed", "cases": len(records),
              "elapsed_seconds": time.perf_counter() - started, "records": records}
    (ROOT / "reports/data_verification.json").write_text(
        json.dumps(report, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")


if __name__ == "__main__":
    main()
