#!/usr/bin/env bash
set -euo pipefail
HERE=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
ROOT=$(cd -- "$HERE/.." && pwd)
REPO=$(git -C "$ROOT" rev-parse --show-toplevel)
DATA=${1:-"$ROOT/../../data/ring"}
DOWN=${2:-"$ROOT/../../down/ring"}
[[ $# -le 2 ]] || { echo "Usage: $0 [data-directory [sample-directory]]" >&2; exit 2; }
BUILD="$ROOT/review/.build"
mkdir -p "$BUILD" "$DATA" "$DOWN"
DATA=$(cd -- "$DATA" && pwd)
DOWN=$(cd -- "$DOWN" && pwd)
[[ "$DATA" != "$DOWN" ]] || { echo "Data and sample directories must differ" >&2; exit 2; }
CXX=${CXX:-g++}
"$CXX" -std=c++17 -O2 -pipe -I"$REPO" "$HERE/gen.cpp" -o "$BUILD/gen"
"$CXX" -std=c++17 -O2 -pipe -I"$REPO" "$HERE/val.cpp" -o "$BUILD/val"
"$CXX" -std=c++17 -O2 -pipe "$ROOT/sol/std_boundary_dp.cpp" -o "$BUILD/std"
# Stage everything first. Existing unrelated files in either destination stay intact.
STAGE=$(mktemp -d "$BUILD/generate.XXXXXX")
trap 'rm -rf -- "$STAGE"' EXIT
mkdir -p "$STAGE/data" "$STAGE/down"
statement_sample() {
  awk -v target="$1" '
    /^## 样例$/ { sample = 1; next }
    sample && /^## / { exit }
    sample && /^```text$/ { ++block; inside = 1; next }
    sample && /^```$/ { inside = 0; next }
    sample && inside && block == target { print; found = 1 }
    END { if (!found) exit 1 }
  ' "$ROOT/statement.md"
}
make_case() {
  local destination=$1 filename=$2 group=$3 type=$4 n=$5 seed=$6 leaves=$7
  local input="$STAGE/$destination/$filename"
  local answer="${input%.in}.ans"
  local -a options=()
  [[ "$group" == - ]] || options+=("--subtask=$group")
  if [[ "$type" == statement ]]; then
    statement_sample 1 > "$input"
    [[ "$(head -n 1 -- "$input")" == "$n" ]]
  else
    local -a args=("--type=$type" "--n=$n" "--seed=$seed")
    [[ "$leaves" == - ]] || args+=("--leaves=$leaves")
    "$BUILD/gen" "${args[@]}" > "$input"
  fi
  "$BUILD/val" "${options[@]}" < "$input"
  "$BUILD/std" < "$input" > "$answer"
  if [[ "$type" == statement ]]; then
    statement_sample 2 > "$STAGE/statement.ans"
    cmp -- "$answer" "$STAGE/statement.ans"
  fi
  echo "Validated $destination/$filename ($type, n=$n)"
}
while IFS=$'\t' read -r destination filename group type n seed leaves; do
  [[ "$destination" == destination ]] && continue
  make_case "$destination" "$filename" "$group" "$type" "$n" "$seed" "$leaves"
done < "$HERE/cases.tsv"
cp -- "$STAGE/data/"*.in "$STAGE/data/"*.ans "$DATA/"
cp -- "$STAGE/down/"*.in "$STAGE/down/"*.ans "$DOWN/"
for file in "$STAGE/down/"*.in "$STAGE/down/"*.ans; do
  chmod 664 -- "$DOWN/$(basename -- "$file")"
done
echo "Generated 30 official cases in $DATA and 6 public cases in $DOWN"
