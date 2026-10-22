#!/usr/bin/env bash
# bash /path/to/dev/calibration/gen/genall.sh [data_directory] [large_sample_directory]
set -euo pipefail
ROOT=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
REPO=$(git -C "$ROOT" rev-parse --show-toplevel)
[[ -f "$REPO/testlib.h" ]] || { echo "Missing Git-root testlib.h: $REPO/testlib.h" >&2; exit 1; }
DATA=${1:-"$ROOT/../../data/calibration"}
DOWN=${2:-"$ROOT/../../down/calibration"}
mkdir -p -- "$DATA" "$DOWN"
DATA=$(cd -- "$DATA" && pwd)
DOWN=$(cd -- "$DOWN" && pwd)
BUILD=$(mktemp -d "$ROOT/.genall.XXXXXX")
trap 'rm -rf -- "$BUILD"' EXIT
CXX=${CXX:-g++}
"$CXX" -std=c++17 -O2 -I "$REPO" "$ROOT/gen/gen.cpp" -o "$BUILD/gen"
"$CXX" -std=c++17 -O2 -I "$REPO" "$ROOT/gen/val.cpp" -o "$BUILD/val"
"$CXX" -std=c++17 -O2 -DNDEBUG "$ROOT/sol/std_wblt.cpp" -o "$BUILD/std"
"$CXX" -std=c++17 -O2 -DNDEBUG "$ROOT/sol/std_avl.cpp" -o "$BUILD/avl"
"$CXX" -std=c++17 -O2 -DNDEBUG "$ROOT/sol/partial_brute.cpp" -o "$BUILD/brute"
python3 "$ROOT/review/generate_data.py" --build "$BUILD" --data "$DATA" --down "$DOWN"
