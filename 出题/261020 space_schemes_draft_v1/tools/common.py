"""Shared build and subprocess helpers; no third-party dependencies."""
import hashlib
import os
from pathlib import Path
import shlex
import subprocess
import time

ROOT = Path(__file__).resolve().parents[1]


def executable(build_dir, name):
    return Path(build_dir).resolve() / (name + ('.exe' if os.name == 'nt' else ''))


def compile_cpp(source, target, cxx=None):
    source, target = Path(source), Path(target)
    target.parent.mkdir(parents=True, exist_ok=True)
    command = shlex.split(cxx or os.environ.get('CXX', 'g++'))
    command += ['-std=c++17', '-O2', '-DNDEBUG', '-Wall', '-Wextra',
                str(source), '-o', str(target)]
    result = subprocess.run(command, capture_output=True, text=True)
    if result.returncode:
        raise RuntimeError('Compilation failed:\n' + result.stdout + result.stderr)
    return result.stderr.strip()


def run_solution(program, input_file, output_file, timeout=30):
    start = time.perf_counter()
    with Path(input_file).open('rb') as fin, Path(output_file).open('wb') as fout:
        result = subprocess.run([str(Path(program).resolve())], stdin=fin, stdout=fout,
                                stderr=subprocess.PIPE, timeout=timeout)
    elapsed = time.perf_counter() - start
    if result.returncode:
        raise RuntimeError(f'{program} exited {result.returncode}: ' +
                           result.stderr.decode(errors='replace')[:2000])
    return elapsed


def sha256(path):
    digest = hashlib.sha256()
    with Path(path).open('rb') as f:
        for block in iter(lambda: f.read(1 << 20), b''):
            digest.update(block)
    return digest.hexdigest()
