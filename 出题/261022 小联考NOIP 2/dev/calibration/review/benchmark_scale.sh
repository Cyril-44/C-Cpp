#!/usr/bin/env bash
# Rebuild and benchmark every full-size operation strategy from any working directory.
set -euo pipefail
ROOT=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
REPO=$(git -C "$ROOT" rev-parse --show-toplevel)
BUILD=$(mktemp -d "$ROOT/.genall.benchmark.XXXXXX")
trap 'rm -rf -- "$BUILD"' EXIT
CXX=${CXX:-g++}
"$CXX" -std=c++17 -O2 -I "$REPO" "$ROOT/gen/gen.cpp" -o "$BUILD/gen"
"$CXX" -std=c++17 -O2 -I "$REPO" "$ROOT/gen/val.cpp" -o "$BUILD/val"
"$CXX" -std=c++17 -O2 -DNDEBUG "$ROOT/sol/std_wblt.cpp" -o "$BUILD/std"
"$CXX" -std=c++17 -O2 -DNDEBUG "$ROOT/sol/std_avl.cpp" -o "$BUILD/avl"
printf 'profile\tprogram\tseconds\trss_kib\n' > "$ROOT/review/scale_benchmark.tsv"
for profile in mixed gc offset nested small_k large_k preserved_seed; do
  "$BUILD/gen" 500000 500000 5 random "$profile" 20261022 > "$BUILD/scale.in"
  "$BUILD/val" --group 5 < "$BUILD/scale.in" > /dev/null 2> "$BUILD/val.log"
  for program in std avl; do
    /usr/bin/time -f '%e\t%M' -o "$BUILD/time" timeout 4s "$BUILD/$program" < "$BUILD/scale.in" > "$BUILD/$program.out"
    printf '%s\t%s\t' "$profile" "$program" >> "$ROOT/review/scale_benchmark.tsv"
    cat "$BUILD/time" >> "$ROOT/review/scale_benchmark.tsv"
  done
  python3 "$ROOT/review/check_output.py" "$BUILD/scale.in" "$BUILD/std.out" "$BUILD/avl.out"
done
cat "$ROOT/review/scale_benchmark.tsv"
