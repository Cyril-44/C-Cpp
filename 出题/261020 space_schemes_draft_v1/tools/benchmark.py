#!/usr/bin/env python3
"""Measure representative cases. Windows reports peak process working set."""
import argparse
import ctypes
import json
import os
import platform
import shlex
import statistics
import subprocess
import tempfile
import time
from pathlib import Path

from common import ROOT, executable


def peak_memory(process):
    if os.name != 'nt': return None
    from ctypes import wintypes
    class Counters(ctypes.Structure):
        _fields_ = [('cb', wintypes.DWORD), ('PageFaultCount', wintypes.DWORD)] + [
            (name, ctypes.c_size_t) for name in ['PeakWorkingSetSize', 'WorkingSetSize',
            'QuotaPeakPagedPoolUsage', 'QuotaPagedPoolUsage', 'QuotaPeakNonPagedPoolUsage',
            'QuotaNonPagedPoolUsage', 'PagefileUsage', 'PeakPagefileUsage']]
    counters = Counters()
    counters.cb = ctypes.sizeof(counters)
    function = ctypes.windll.psapi.GetProcessMemoryInfo
    function.argtypes = [wintypes.HANDLE, ctypes.POINTER(Counters), wintypes.DWORD]
    function.restype = wintypes.BOOL
    if function(int(process._handle), ctypes.byref(counters), counters.cb):
        return int(counters.PeakWorkingSetSize)
    return None


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--build-dir', type=Path, default=ROOT / 'build')
    parser.add_argument('--cxx', default=None)
    args = parser.parse_args()
    records = []
    with tempfile.TemporaryDirectory(prefix='benchmark_') as temp:
        for name in ['06_01', '09_01', '10_01', '10_02', '10_05']:
            times, peaks = [], []
            for repeat in range(3):
                with (ROOT / 'data' / (name + '.in')).open('rb') as fin, (Path(temp) / 'out').open('wb') as fout:
                    start = time.perf_counter()
                    process = subprocess.Popen([str(executable(args.build_dir, 'std'))], stdin=fin, stdout=fout)
                    peak = 0
                    while process.poll() is None:
                        peak = max(peak, peak_memory(process) or 0)
                        time.sleep(0.002)
                    if process.returncode: raise RuntimeError('std benchmark failed')
                    times.append(time.perf_counter()-start)
                    peaks.append(peak or None)
            records.append({'id': name, 'seconds': times, 'median_seconds': statistics.median(times),
                            'peak_working_set_bytes': max((x for x in peaks if x), default=None)})
            print(f'{name}: median {statistics.median(times):.3f}s, peak {records[-1]["peak_working_set_bytes"]}', flush=True)
        # Deliberately test a correct full-tree recomputation against the proposed time limit.
        slow_timed_out = False
        start = time.perf_counter()
        try:
            with (ROOT / 'data/10_01.in').open('rb') as fin, (Path(temp) / 'slow').open('wb') as fout:
                subprocess.run([str(executable(args.build_dir, 'recompute'))], stdin=fin, stdout=fout,
                               timeout=3, check=True)
        except subprocess.TimeoutExpired:
            slow_timed_out = True
    compiler_command = shlex.split(args.cxx or os.environ.get('CXX', 'g++'))
    compiler = subprocess.check_output(compiler_command + ['--version'], text=True).splitlines()[0]
    report = {'platform': platform.platform(), 'processor': platform.processor(), 'compiler': compiler,
              'python': platform.python_version(), 'cases': records,
              'proposed_time_limit_seconds': 3, 'proposed_memory_limit_mib': 256,
              'recompute_10_01_timed_out_at_3_seconds': slow_timed_out,
              'recompute_elapsed_seconds': time.perf_counter()-start,
              'notes': 'Wall clock includes process startup. Windows peak working set is sampled; '
                       'this is a local benchmark, not a guarantee about another judge host.'}
    (ROOT / 'reports/benchmark.json').write_text(json.dumps(report, ensure_ascii=False, indent=2) + '\n', encoding='utf-8')


if __name__ == '__main__':
    main()
