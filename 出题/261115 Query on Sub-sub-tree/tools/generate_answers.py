#!/usr/bin/env python3
"""Run a compiled reference solution on every generated .in file."""

from pathlib import Path
import resource
import subprocess
import sys


ROOT = Path(__file__).resolve().parents[1]
DATA = ROOT / "data"


def unlimited_stack():
    resource.setrlimit(resource.RLIMIT_STACK,
                       (resource.RLIM_INFINITY, resource.RLIM_INFINITY))


def main():
    if len(sys.argv) != 2:
        raise SystemExit("usage: python3 tools/generate_answers.py /path/to/std")
    executable = sys.argv[1]
    inputs = sorted(DATA.glob("subtask*/*.in"))
    if not inputs:
        raise SystemExit("no input files found; run generate_data.py first")
    for input_path in inputs:
        result = subprocess.run(
            [executable], input=input_path.read_bytes(), capture_output=True,
            timeout=120, preexec_fn=unlimited_stack,
        )
        if result.returncode != 0:
            sys.stderr.write(f"reference failed on {input_path}: exit {result.returncode}\n")
            sys.stderr.buffer.write(result.stderr)
            raise SystemExit(1)
        answer_path = input_path.with_suffix(".ans")
        answer_path.write_bytes(result.stdout)
        print(f"{input_path.relative_to(ROOT)} -> {answer_path.relative_to(ROOT)}")


if __name__ == "__main__":
    main()
