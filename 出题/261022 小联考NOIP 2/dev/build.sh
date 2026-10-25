#!/usr/bin/env bash
set -euo pipefail

DEV_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
EVENT_DIR=$(cd -- "$DEV_DIR/.." && pwd)
STATEMENT_DIR="$DEV_DIR/statement"
DATA_DIR="$EVENT_DIR/data"
DOWN_DIR="$EVENT_DIR/down"
PDF_SOURCE="$STATEMENT_DIR/statement.pdf"

for command in pandoc python3 xelatex "${CXX:-g++}" "${CC:-gcc}"; do
  command -v -- "$command" >/dev/null 2>&1 || {
    echo "Required command not found: $command" >&2
    exit 1
  }
done
mkdir -p -- "$DATA_DIR" "$DOWN_DIR"

echo "[statement] Rendering Markdown into main.tex"
python3 "$STATEMENT_DIR/build_statement.py"
for pass in 1 2; do
  if ! (cd -- "$STATEMENT_DIR" && \
      xelatex -interaction=nonstopmode -halt-on-error -shell-escape \
        -jobname=statement main.tex >/dev/null 2>&1); then
    echo "XeLaTeX failed on pass $pass; last log lines:" >&2
    tail -n 80 "$STATEMENT_DIR/statement.log" >&2 || true
    exit 1
  fi
done

for problem in ring calibration lrud storage; do
  echo "[$problem] Generating data"
  if [[ -f "$DEV_DIR/$problem/gen/genall.py" ]]; then
    python3 "$DEV_DIR/$problem/gen/genall.py" \
      "$DATA_DIR/$problem" "$DOWN_DIR/$problem"
  else
    bash "$DEV_DIR/$problem/gen/genall.sh" \
      "$DATA_DIR/$problem" "$DOWN_DIR/$problem"
  fi
done

# LRUD genall.sh publishes its checked, obfuscated checker.c.

[[ -s "$PDF_SOURCE" ]] || {
  echo "Statement PDF was not generated: $PDF_SOURCE" >&2
  exit 1
}
cp -- "$PDF_SOURCE" "$DOWN_DIR/statement.pdf"
chmod 664 -- "$DOWN_DIR/statement.pdf"
echo "Copied the statement PDF to $DOWN_DIR/statement.pdf"
