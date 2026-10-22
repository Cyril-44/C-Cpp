#!/usr/bin/env bash
# Usage: bash gen/genall.sh [final_data_directory] [sample_directory]
# Defaults are relative to dev/lrud, independent of the current directory.
set -euo pipefail
[[ $# -le 2 ]] || { echo "Usage: $0 [data_dir] [down_dir]" >&2; exit 2; }
ROOT=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
REPO=$(git -C "$ROOT" rev-parse --show-toplevel)
[[ -f "$REPO/testlib.h" ]] || { echo "Missing $REPO/testlib.h" >&2; exit 1; }
DATA=${1:-"$ROOT/../../data/lrud"}
DOWN=${2:-"$ROOT/../../down/lrud"}
mkdir -p -- "$DATA" "$DOWN"
DATA=$(cd -- "$DATA" && pwd)
DOWN=$(cd -- "$DOWN" && pwd)
[[ "$DATA" != "$DOWN" ]] || { echo "Data and sample directories must differ." >&2; exit 2; }
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
# Extract the current statement's examples; keep this entry point pure Bash/AWK.
mkdir -p -- "$BUILD/public"
awk -v out="$BUILD/public" -v count_file="$BUILD/sample-count" '
  /^## 样例/ { sample++; active=1; mode=""; next }
  /^## / { active=0; mode="" }
  active && /^### 输入/ { mode="in"; next }
  active && /^### 输出/ { mode="ans"; next }
  active && /^### / { mode="" }
  active && /^```/ {
    if (inside) { inside=0; mode="" }
    else if (mode != "") { inside=1; seen[sample,mode]=1 }
    next
  }
  inside { print > (out "/lrud" sample "." mode) }
  END {
    if (sample == 0 || inside) exit 1
    for (i=1; i<=sample; i++)
      if (!seen[i,"in"] || !seen[i,"ans"]) exit 1
    print sample > count_file
  }
' "$ROOT/statement.md"
samples=$(cat "$BUILD/sample-count")
for ((i=1; i<=samples; i++)); do
  input="$BUILD/public/lrud$i.in"
  answer="$BUILD/public/lrud$i.ans"
  "$BUILD/val" --subtask=5 < "$input"
  timeout 1s "$BUILD/std" < "$input" > "$BUILD/reference.ans"
  "$BUILD/checker" "$input" "$answer" "$BUILD/reference.ans" > "$BUILD/check.log" 2>&1 || { cat "$BUILD/check.log" >&2; exit 1; }
done
cp -- "$BUILD/public/"* "$DOWN/"
for file in "$BUILD/public/"*; do
  chmod 664 -- "$DOWN/$(basename -- "$file")"
done
printf 'Done: %d final cases in %s; %d statement samples in %s\n' "$count" "$DATA" "$samples" "$DOWN"
