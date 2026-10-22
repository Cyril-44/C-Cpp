#!/usr/bin/env bash
set -euo pipefail

ROOT=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
BUILD="$ROOT/build"
TESTLIB_INCLUDE_DIR=${TESTLIB_INCLUDE_DIR:-"$HOME/C-Cpp"}

if [[ ! -f "$TESTLIB_INCLUDE_DIR/testlib.h" ]]; then
  echo "Cannot find testlib.h in $TESTLIB_INCLUDE_DIR" >&2
  echo "Set TESTLIB_INCLUDE_DIR to the directory containing testlib.h." >&2
  exit 2
fi

mkdir -p "$BUILD" "$ROOT/data/subtask1" "$ROOT/data/subtask2" \
  "$ROOT/data/subtask3" "$ROOT/data/subtask4" "$ROOT/samples"

g++ -std=c++17 -O2 -pipe -I"$TESTLIB_INCLUDE_DIR" "$ROOT/gen.cpp" -o "$BUILD/gen"
g++ -std=c++17 -O2 -pipe -I"$TESTLIB_INCLUDE_DIR" "$ROOT/validator.cpp" -o "$BUILD/validator"
g++ -std=c++17 -O2 -pipe "$ROOT/solution/std.cpp" -o "$BUILD/std"

make_case() {
  local group=$1 name=$2 type=$3 n=$4 seed=$5 leaves=${6:-}
  local input="$ROOT/data/subtask${group}/${name}.in"
  local answer="${input%.in}.ans"
  local -a args=("--type=$type" "--n=$n" "--seed=$seed")
  if [[ -n "$leaves" ]]; then args+=("--leaves=$leaves"); fi
  "$BUILD/gen" "${args[@]}" > "$input"
  "$BUILD/validator" "--subtask=$group" < "$input"
  "$BUILD/std" < "$input" > "$answer"
}

make_sample() {
  local name=$1 type=$2 n=$3 seed=$4 leaves=${5:-}
  local input="$ROOT/samples/${name}.in"
  local answer="$ROOT/samples/${name}.out"
  local -a args=("--type=$type" "--n=$n" "--seed=$seed")
  if [[ -n "$leaves" ]]; then args+=("--leaves=$leaves"); fi
  "$BUILD/gen" "${args[@]}" > "$input"
  "$BUILD/validator" < "$input"
  "$BUILD/std" < "$input" > "$answer"
}

# Subtask 1: exhaustive reference cases.
make_case 1 01_chain chain 4 101
make_case 1 02_two_leaves two 4 102
make_case 1 03_star star 20 103
make_case 1 04_balanced balanced 20 104
make_case 1 05_few_leaves few 20 105 8
make_case 1 06_random random 20 106

# Subtask 2: long trees with at most eight leaves.
make_case 2 01_one_leaf chain 100000 201
make_case 2 02_two_leaves two 100000 202
make_case 2 03_three_leaves few 100000 203 3
make_case 2 04_eight_leaves few 100000 204 8
make_case 2 05_small_spine few 99999 205 7

# Subtask 3: stars, including the maximum size.
make_case 3 01_min_star star 4 301
make_case 3 02_star star 100000 302
make_case 3 03_max_star star 1000000 303

# Subtask 4: unrestricted shapes and boundary cases.
make_case 4 01_max_chain chain 1000000 401
make_case 4 02_max_two_leaves two 1000000 402
make_case 4 03_max_balanced balanced 1000000 403
make_case 4 04_max_comb comb 1000000 404
make_case 4 05_max_broom broom 1000000 405
make_case 4 06_max_random random 1000000 406
make_case 4 07_random_medium random 500000 407

# Public statement and diagnostic samples.
make_sample statement_sample balanced 7 501
make_sample long_chain chain 100000 502
make_sample few_leaves few 100000 503 8
make_sample star star 200000 504
make_sample general_tree random 200000 505

echo "Generated data and samples under $ROOT/data and $ROOT/samples"
