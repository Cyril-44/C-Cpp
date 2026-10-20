#!/usr/bin/env python3
"""Run independent-subtask scoring for one local solution; not a security sandbox."""
import argparse
import json
from pathlib import Path
import subprocess
import tempfile
import time
from common import ROOT, compile_cpp, executable


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--name", required=True)
    ap.add_argument("--source", type=Path)
    ap.add_argument("--program", type=Path)
    ap.add_argument("--build-dir", type=Path, default=ROOT / "build")
    ap.add_argument("--timeout", type=float)
    ap.add_argument("--subtasks", nargs="*", type=int)
    ap.add_argument("--cxx")
    args = ap.parse_args()
    config = json.loads((ROOT / "config/subtasks.json").read_text(encoding="utf-8"))
    timeout = args.timeout or config["time_limit_seconds"]
    if args.program:
        program = args.program.resolve()
    else:
        source = args.source or ROOT / "solutions" / (args.name + ".cpp")
        program = executable(args.build_dir, args.name)
        compile_cpp(source, program, args.cxx)
    cases = json.loads((ROOT / "config/cases.json").read_text(encoding="utf-8"))["cases"]
    results, score = [], 0
    with tempfile.TemporaryDirectory(prefix="flexible_judge_") as temp:
        for subtask in config["subtasks"]:
            sid = subtask["id"]
            if args.subtasks and sid not in args.subtasks:
                continue
            tests, passed = [], True
            for case in (x for x in cases if x["subtask"] == sid):
                inp, ans = ROOT / case["input"], ROOT / case["answer"]
                output = Path(temp) / "answer"
                begin = time.perf_counter()
                status, error = "AC", ""
                try:
                    with inp.open("rb") as fin, output.open("wb") as fout:
                        p = subprocess.run([str(program)], stdin=fin, stdout=fout,
                                           stderr=subprocess.PIPE, timeout=timeout)
                    if p.returncode:
                        status, error = "RE", p.stderr.decode(errors="replace")[:300]
                    elif output.read_bytes().split() != ans.read_bytes().split():
                        status = "WA"
                except subprocess.TimeoutExpired:
                    status = "TLE"
                tests.append({"id": case["id"], "status": status,
                              "seconds": time.perf_counter() - begin, "detail": error})
                if status != "AC":
                    passed = False
                    break
            awarded = subtask["score"] if passed and tests else 0
            score += awarded
            results.append({"subtask": sid, "score": awarded, "tests": tests})
            print(f"{args.name}: subtask {sid} = {awarded}; "
                  + ", ".join(t["id"] + ":" + t["status"] for t in tests), flush=True)
    report = {"solution": args.name, "score": score, "time_limit_seconds": timeout,
              "memory_limit_enforced": False, "subtasks": results}
    (ROOT / "reports" / ("score_" + args.name + ".json")).write_text(
        json.dumps(report, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    print(f"{args.name}: {score} points", flush=True)


if __name__ == "__main__":
    main()
