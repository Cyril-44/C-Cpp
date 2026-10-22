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
printf 'file\tsubtask\ttype\tn\tseed\tleaves_parameter\n' > "$STAGE/data/manifest.tsv"
printf 'file\ttype\tn\tseed\tleaves_parameter\n' > "$STAGE/down/manifest.tsv"
make_case() {
  local group=$1 number=$2 type=$3 n=$4 seed=$5 leaves=${6:-}
  local name input
  printf -v name '%02d_%02d' "$group" "$number"
  input="$STAGE/data/$name.in"
  local -a args=("--type=$type" "--n=$n" "--seed=$seed")
  [[ -z "$leaves" ]] || args+=("--leaves=$leaves")
  "$BUILD/gen" "${args[@]}" > "$input"
  "$BUILD/val" "--subtask=$group" < "$input"
  "$BUILD/std" < "$input" > "${input%.in}.ans"
  echo "Validated $name ($type, n=$n)"
}
make_sample() {
  local number=$1 type=$2 n=$3 seed=$4 leaves=${5:-}
  local input="$STAGE/down/ring$number.in"
  local -a args=("--type=$type" "--n=$n" "--seed=$seed")
  [[ -z "$leaves" ]] || args+=("--leaves=$leaves")
  "$BUILD/gen" "${args[@]}" > "$input"
  "$BUILD/val" < "$input"
  "$BUILD/std" < "$input" > "${input%.in}.ans"
}
make_case 1 1 chain 4 101
make_case 1 2 two 4 102
make_case 1 3 star 4 103
make_case 1 4 star 20 104
make_case 1 5 balanced 20 105
make_case 1 6 few 20 106 8
make_case 1 7 random 20 107
make_case 1 8 binary_random 19 108
make_case 2 1 chain 100000 201
make_case 2 2 two 100000 202
make_case 2 3 few 100000 203 3
make_case 2 4 few 100000 204 8
make_case 2 5 few_random 100000 205 8
make_case 2 6 few_random 99999 206 7
make_case 2 7 few_random 99999 207 2
make_case 2 8 few_random 15 208 8
make_case 3 1 star 4 301
make_case 3 2 star 99999 302
make_case 3 3 star 100000 303
make_case 3 4 star 1000000 304
make_case 4 1 chain 1000000 401
make_case 4 2 two 1000000 402
make_case 4 3 balanced 1000000 403
make_case 4 4 comb 1000000 404
make_case 4 5 broom 1000000 405
make_case 4 6 random 1000000 406
make_case 4 7 binary_random 1000000 407
make_case 4 8 random 999999 408
make_case 4 9 few_random 1000000 409 8
make_case 4 10 balanced 999999 410
make_sample 1 chain 100000 501
make_sample 2 few_random 100000 502 8
make_sample 3 star 1000000 503
make_sample 4 random 1000000 504
make_sample 5 binary_random 1000000 505
cp -- "$STAGE/data/"* "$DATA/"
cp -- "$STAGE/down/"* "$DOWN/"
for file in "$STAGE/down/"*; do
  chmod 664 -- "$DOWN/$(basename -- "$file")"
done
echo "Generated 30 official cases in $DATA and 5 public cases in $DOWN"
