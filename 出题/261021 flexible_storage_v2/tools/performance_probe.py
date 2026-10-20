#!/usr/bin/env python3
"""Test a correct variant that propagates after every chain update separately."""
import argparse
import json
from pathlib import Path
import subprocess
import tempfile
import time
from common import ROOT, compile_cpp, executable, sha256
from gen import generate_case
from verify import encode


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--build-dir", type=Path, default=ROOT / "build/performance_probe")
    ap.add_argument("--timeout", type=float, default=10)
    args = ap.parse_args()
    args.build_dir.mkdir(parents=True, exist_ok=True)
    source = (ROOT / "solutions/std.cpp").read_text(encoding="utf-8")
    before = "        if (l <= r) rangeAdd(1, 1, N, l, r, direction, delta);"
    after = """        if (l <= r) {
            rangeAdd(1, 1, N, l, r, direction, delta);
            vector<int> immediately;
            collect(at[l], immediately);
            finish(immediately);
        }"""
    assert source.count(before) == 1
    source = source.replace(before, after)
    before = """        if (before != (heavyCount[p] != 0))
            pointRemake(1, 1, N, dfn[p]);"""
    after = """        if (before != (heavyCount[p] != 0)) {
            pointRemake(1, 1, N, dfn[p]);
            vector<int> immediately;
            collect(p, immediately);
            finish(immediately);
        }"""
    assert source.count(before) == 1
    source = source.replace(before, after)
    source_path = args.build_dir / "unbatched.cpp"
    source_path.write_text(source, encoding="utf-8")
    slow = executable(args.build_dir, "unbatched")
    reference = executable(args.build_dir, "recompute")
    compile_cpp(source_path, slow)
    compile_cpp(ROOT / "solutions/recompute.cpp", reference)
    for seed in range(24):
        n = 1 + seed * 7 % 50
        rows, ops, _ = generate_case(n, 300,
                                    ("mid", "random", "zigzag_comb")[seed % 3],
                                    "mixed", ("unit", "tie", "random")[seed % 3],
                                    812345 + seed)
        content = encode(n, rows, ops)
        expected = subprocess.check_output([str(reference)], input=content, timeout=15)
        actual = subprocess.check_output([str(slow)], input=content, timeout=15)
        assert expected.split() == actual.split()
    manifest = json.loads((ROOT / "manifest.json").read_text(encoding="utf-8"))
    by_id = {c["id"]: c for c in manifest["cases"]}
    results = []
    with tempfile.TemporaryDirectory(prefix="unbatched_probe_") as temp:
        for name in ("s04_04", "s10_08", "s10_09"):
            case = by_id[name]
            start = time.perf_counter()
            status = "AC"
            try:
                with (ROOT / case["input"]).open("rb") as inp, (Path(temp) / "out").open("wb") as out:
                    subprocess.run([str(slow)], stdin=inp, stdout=out,
                                   timeout=args.timeout, check=True)
                assert (Path(temp) / "out").read_bytes().split() == (
                    ROOT / case["answer"]).read_bytes().split()
            except subprocess.TimeoutExpired:
                status = "TLE"
            results.append({"id": name, "status": status,
                            "seconds": time.perf_counter() - start})
            print(results[-1], flush=True)
    report = {"small_cases_passed": 24, "small_operations": 7200,
              "standard_sha256": sha256(ROOT / "solutions/std.cpp"),
              "variant_sha256": sha256(source_path), "timeout_seconds": args.timeout,
              "results": results,
              "note": "This tests one correct unbatched implementation; it does not prove that "
                      "every O(log^3 n) implementation exceeds the time limit."}
    (ROOT / "reports/unbatched_performance.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")


if __name__ == "__main__":
    main()
