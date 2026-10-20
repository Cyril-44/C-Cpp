#!/usr/bin/env python3
"""Score supplied partial solutions with the configured independent subtasks."""
import argparse
from pathlib import Path
import subprocess
import sys
from common import ROOT


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--build-dir", type=Path, default=ROOT / "build")
    args = ap.parse_args()
    for name in ("brute", "short", "recompute", "bounded_height"):
        subprocess.run([sys.executable, str(ROOT / "tools/judge.py"),
                        "--name", name, "--build-dir", str(args.build_dir)], check=True)


if __name__ == "__main__":
    main()
