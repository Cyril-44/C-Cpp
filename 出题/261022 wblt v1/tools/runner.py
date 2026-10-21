#!/usr/bin/env python3
"""Measured, bounded subprocess runner (Windows/Linux; no third-party modules)."""
import ctypes
import json
import os
from pathlib import Path
import subprocess
import time

if os.name == 'nt':
    from ctypes import wintypes
    class MemoryCounters(ctypes.Structure):
        _fields_ = [('cb', wintypes.DWORD), ('PageFaultCount', wintypes.DWORD)] + [
            (name, ctypes.c_size_t) for name in ('PeakWorkingSetSize', 'WorkingSetSize',
             'QuotaPeakPagedPoolUsage', 'QuotaPagedPoolUsage', 'QuotaPeakNonPagedPoolUsage',
             'QuotaNonPagedPoolUsage', 'PagefileUsage', 'PeakPagefileUsage')]
    get_memory = ctypes.WinDLL('psapi').GetProcessMemoryInfo
    get_memory.argtypes = [wintypes.HANDLE, ctypes.POINTER(MemoryCounters), wintypes.DWORD]
    get_memory.restype = wintypes.BOOL

def memory_bytes(proc):
    if os.name == 'nt':
        info = MemoryCounters(); info.cb = ctypes.sizeof(info)
        if get_memory(int(proc._handle), ctypes.byref(info), info.cb):
            return info.PeakWorkingSetSize
    else:
        try:
            lines = Path(f'/proc/{proc.pid}/status').read_text().splitlines()
            return max(int(line.split()[1])*1024 for line in lines if line.startswith(('VmHWM:', 'VmRSS:')))
        except (OSError, ValueError):
            pass
    return 0

def run_measured(command, input_path, output_path, timeout=120, memory_mb=None, env=None):
    output_path = Path(output_path); output_path.parent.mkdir(parents=True, exist_ok=True)
    stderr_path = output_path.with_suffix(output_path.suffix+'.stderr')
    start = time.perf_counter(); peak = 0; verdict = 'OK'
    with open(input_path, 'rb') as src, open(output_path, 'wb') as dst, open(stderr_path, 'wb') as err:
        proc = subprocess.Popen([str(x) for x in command], stdin=src, stdout=dst, stderr=err, env=env)
        while True:
            peak = max(peak, memory_bytes(proc))
            code = proc.poll()
            if code is not None:
                break
            elapsed = time.perf_counter()-start
            if elapsed > timeout:
                verdict = 'TLE'; proc.kill(); proc.wait(); break
            if memory_mb and peak > memory_mb*1024*1024:
                verdict = 'MLE'; proc.kill(); proc.wait(); break
            time.sleep(0.01)
        elapsed = time.perf_counter()-start
        peak = max(peak, memory_bytes(proc))
        code = proc.returncode
    if verdict == 'OK' and code:
        verdict = 'RE'
    text = stderr_path.read_text(encoding='utf-8', errors='replace')
    profile = None
    for line in text.splitlines():
        try:
            obj = json.loads(line)
            if isinstance(obj, dict) and 'node_bytes' in obj:
                profile = obj
        except ValueError:
            pass
    return {'status': verdict, 'returncode': code, 'wall_seconds': round(elapsed, 6),
            'peak_working_set_bytes': peak, 'profile': profile,
            'stderr': text[:4000] if verdict != 'OK' else ''}

if __name__ == '__main__':
    import argparse
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--exe', required=True); ap.add_argument('--input', required=True)
    ap.add_argument('--output', required=True); ap.add_argument('--timeout', type=float, default=120)
    ap.add_argument('--memory-mb', type=int)
    a = ap.parse_args()
    print(json.dumps(run_measured([a.exe], a.input, a.output, a.timeout, a.memory_mb), ensure_ascii=False))
