#!/usr/bin/env python3
"""Final package audit: hashes, scoring, sample, source versions and seed replay."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import tempfile
from common import ROOT, executable, sha256
from gen import generate_case


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--build-dir", type=Path, default=ROOT / "build")
    ap.add_argument("--replay", nargs="*", default=["s01_03", "s07_02", "s10_09", "large_sample_02"])
    args = ap.parse_args()
    manifest = json.loads((ROOT / "manifest.json").read_text(encoding="utf-8"))
    plan = json.loads((ROOT / "tools/case_plan.json").read_text(encoding="utf-8"))
    config = json.loads((ROOT / "config/subtasks.json").read_text(encoding="utf-8"))
    assert manifest["status"] == "generated"
    assert len(config["subtasks"]) == 10
    assert sum(s["score"] for s in config["subtasks"]) == 100
    assert manifest["private_cases"] == len(plan["cases"]) == 45
    assert manifest["large_samples"] == len(plan["large_samples"]) == 2
    planned = {c["id"]: c for c in plan["cases"] + plan["large_samples"]}
    verified = {}
    total_bytes = 0
    for case in manifest["cases"]:
        assert case["std_sha256"] == sha256(ROOT / "solutions/std.cpp")
        assert case["generator_sha256"] == sha256(ROOT / "tools/gen.py")
        for key in plan["cli_fields"]:
            assert case["parameters"][key] == planned[case["id"]][key]
        for kind in ("input", "answer"):
            path = ROOT / case[kind]
            assert sha256(path) == case[kind + "_sha256"]
            assert path.stat().st_size == case[kind + "_bytes"]
            total_bytes += path.stat().st_size
        verified[case["id"]] = case
    replayed = []
    for name in args.replay:
        case = verified[name]
        rows, operations, _ = generate_case(**case["parameters"])
        h = hashlib.sha256()
        h.update(f"{case['n']} {case['q']}\n".encode())
        for a, b, w in rows:
            h.update(f"{a} {b} {w}\n".encode())
        for op, l, r in operations:
            h.update(f"{op} {l} {r}\n".encode())
        assert h.hexdigest() == case["input_sha256"], name
        replayed.append(name)
    sample_input = ROOT / "samples/sample1.in"
    sample_answer = ROOT / "samples/sample1.ans"
    statement = (ROOT / "statement.md").read_text(encoding="utf-8")
    blocks = re.findall(r"```text\s*\n(.*?)```", statement, re.S)
    assert blocks[0].split() == sample_input.read_text().split()
    assert blocks[1].split() == sample_answer.read_text().split()
    for name in ("std", "recompute", "brute"):
        p = subprocess.run([str(executable(args.build_dir, name))],
                           input=sample_input.read_bytes(), capture_output=True, timeout=20)
        assert p.returncode == 0 and p.stdout.split() == sample_answer.read_bytes().split()
    local_links = 0
    for path in ROOT.rglob("*.md"):
        if any(x in ("build", "work", "__pycache__") for x in path.relative_to(ROOT).parts):
            continue
        for match in re.findall(r"\[[^\]]*\]\(([^)]+)\)", path.read_text(encoding="utf-8")):
            target = match.split("#", 1)[0].strip("<>")
            if not target or "://" in target:
                continue
            if not (path.parent / target).exists():
                raise AssertionError(f"Broken link in {path.name}: {target}")
            local_links += 1
    mutation = json.loads((ROOT / "reports/mutations.json").read_text(encoding="utf-8"))
    assert mutation["standard_sha256"] == sha256(ROOT / "solutions/std.cpp")
    assert mutation["generated_wrong_output_kills"] == mutation["mutants"]
    assert mutation["official_wrong_output_kills"] == mutation["mutants"]
    data_report = json.loads((ROOT / "reports/data_verification.json").read_text(encoding="utf-8"))
    for record in data_report["records"]:
        for kind in ("input", "answer"):
            assert record[kind + "_sha256"] == verified[record["id"]][kind + "_sha256"]
    report = {"status": "passed", "data_files": len(verified) * 2,
              "data_bytes": total_bytes, "private_cases": 45, "large_samples": 2,
              "subtasks": 10, "score_sum": 100, "sample_solvers": ["std", "recompute", "brute"],
              "seed_replays": replayed, "checked_local_links": local_links}
    (ROOT / "reports/final_audit.json").write_text(
        json.dumps(report, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(report, ensure_ascii=False))


if __name__ == "__main__":
    main()
