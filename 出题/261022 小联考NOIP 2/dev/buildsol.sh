#!/usr/bin/env bash
set -euo pipefail

DEV_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
EVENT_DIR=$(cd -- "$DEV_DIR/.." && pwd)

echo > "$EVENT_DIR/sol/solution.md"

for problem in ring calibration lrud storage; do
  echo "[$problem] Concatenating..."
  cat "$DEV_DIR/$problem/sol/solution.md" >> "$EVENT_DIR/sol/solution.md"
done

cd "$EVENT_DIR/sol"

echo Building solution.pdf...
pandoc solution.md -t typst -o solution.pdf --pdf-engine=typst -V mainfont="Noto Serif CJK SC"
