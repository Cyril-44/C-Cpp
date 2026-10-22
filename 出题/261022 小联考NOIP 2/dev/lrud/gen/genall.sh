#!/usr/bin/env bash
# Usage: bash gen/genall.sh [final_data_directory]
# Defaults are relative to dev/lrud, independent of the current directory.
set -euo pipefail
[[ $# -le 1 ]] || { echo "Usage: $0 [data_dir]" >&2; exit 2; }
ROOT=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
REPO=$(git -C "$ROOT" rev-parse --show-toplevel)
[[ -f "$REPO/testlib.h" ]] || { echo "Missing $REPO/testlib.h" >&2; exit 1; }
DATA=${1:-"$ROOT/../../data/lrud"}
mkdir -p -- "$DATA"
DATA=$(cd -- "$DATA" && pwd)
BUILD=$(mktemp -d "$ROOT/.genall.XXXXXX")
trap 'rm -rf -- "$BUILD"' EXIT
CXX=${CXX:-g++}
"$CXX" -std=c++17 -O2 -I "$REPO" "$ROOT/gen/gen.cpp" -o "$BUILD/gen"
"$CXX" -std=c++17 -O2 -I "$REPO" "$ROOT/gen/val.cpp" -o "$BUILD/val"
"$CXX" -std=c++17 -O2 "$ROOT/sol/std_maximum.cpp" -o "$BUILD/std"
"$CXX" -std=c++17 -O2 -I "$REPO" "$ROOT/checker.cpp" -o "$BUILD/checker"
generate() {
  local target=$1 group=$2 values=$3
  "$BUILD/gen" --subtask="$group" --mode=list --values="$values" --seed=20261022 > "$BUILD/case.in"
  "$BUILD/val" --subtask="$group" < "$BUILD/case.in"
  timeout 1s "$BUILD/std" < "$BUILD/case.in" > "$BUILD/case.ans"
  "$BUILD/checker" "$BUILD/case.in" "$BUILD/case.ans" "$BUILD/case.ans" > "$BUILD/check.log" 2>&1 || { cat "$BUILD/check.log" >&2; exit 1; }
  cp -- "$BUILD/case.in" "$target.in"
  cp -- "$BUILD/case.ans" "$target.ans"
}
count=0
while IFS=$'\t' read -r id group values purpose; do
  [[ "$id" == \#* || -z "$id" ]] && continue
  printf 'Generating %s: %s\n' "$id" "$purpose"
  generate "$DATA/$id" "$group" "$values"
  count=$((count + 1))
done < "$ROOT/gen/cases.tsv"
printf 'Done: %d final cases in %s; no large samples are generated.\n' "$count" "$DATA"
