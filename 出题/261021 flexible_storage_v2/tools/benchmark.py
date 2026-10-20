#!/usr/bin/env python3
"""Benchmark the standard solution on representative private tests."""
import argparse
import ctypes
import json
import os
from pathlib import Path
import platform
import statistics
import subprocess
import tempfile
import time
from common import ROOT, executable


def peak_memory(process):
    if os.name != "nt":
        return None
    from ctypes import wintypes
    class Counters(ctypes.Structure):
        _fields_ = [("cb", wintypes.DWORD), ("PageFaultCount", wintypes.DWORD)] + [
            (name, ctypes.c_size_t) for name in ("PeakWorkingSetSize", "WorkingSetSize",
             "QuotaPeakPagedPoolUsage", "QuotaPagedPoolUsage", "QuotaPeakNonPagedPoolUsage",
             "QuotaNonPagedPoolUsage", "PagefileUsage", "PeakPagefileUsage")]
    counter = Counters()
    counter.cb = ctypes.sizeof(counter)
    function = ctypes.windll.psapi.GetProcessMemoryInfo
    function.argtypes = [wintypes.HANDLE, ctypes.POINTER(Counters), wintypes.DWORD]
    function.restype = wintypes.BOOL
    if function(int(process._handle), ctypes.byref(counter), counter.cb):
        return int(counter.PeakWorkingSetSize)
    return None


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--build-dir", type=Path, default=ROOT / "build")
    ap.add_argument("--repeat", type=int, default=3)
    ap.add_argument("--cases", nargs="+", default=["s04_04", "s07_03", "s10_01", "s10_09"])
    args = ap.parse_args()
    manifest = json.loads((ROOT / "manifest.json").read_text(encoding="utf-8"))
    cases = {x["id"]: x for x in manifest["cases"]}
    results = []
    with tempfile.TemporaryDirectory(prefix="flexible_bench_") as temp:
        for name in args.cases:
            seconds, memory = [], []
            case = cases[name]
            for repeat in range(args.repeat):
                with (ROOT / case["input"]).open("rb") as inp, (Path(temp) / "answer").open("wb") as out:
                    begin = time.perf_counter()
                    p = subprocess.Popen([str(executable(args.build_dir, "std"))],
                                         stdin=inp, stdout=out)
                    peak = 0
                    while p.poll() is None:
                        peak = max(peak, peak_memory(p) or 0)
                        time.sleep(0.004)
                    if p.returncode:
                        raise RuntimeError(name + ": nonzero exit")
                    seconds.append(time.perf_counter() - begin)
                    memory.append(peak or None)
                if (Path(temp) / "answer").read_bytes().split() != (ROOT / case["answer"]).read_bytes().split():
                    raise AssertionError(name + ": wrong output")
            record = {"id": name, "seconds": seconds,
                      "median_seconds": statistics.median(seconds),
                      "peak_working_set_bytes": max((x for x in memory if x), default=None)}
            results.append(record)
            print(json.dumps(record), flush=True)
    report = {"status": "passed", "platform": platform.platform(),
              "python": platform.python_version(), "processor": platform.processor(),
              "compiler": subprocess.check_output(["g++", "--version"], text=True).splitlines()[0],
              "cases": results, "time_limit_seconds": 10, "memory_limit_mib": 256,
              "note": "Local wall-clock measurements including startup. Memory is sampled Windows "
                      "peak working set; unavailable on other platforms. Rebenchmark on the judge host."}
    (ROOT / "reports/benchmark.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8")


if __name__ == "__main__":
    main()
