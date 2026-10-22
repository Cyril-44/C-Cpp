#!/usr/bin/env bash
set -euo pipefail

DEV_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
EVENT_DIR=$(cd -- "$DEV_DIR/.." && pwd)

mkdir -p -- "$EVENT_DIR/sol"
: > "$EVENT_DIR/sol/solution.md"
RESOURCE_PATH="$EVENT_DIR/sol"

for problem in ring calibration lrud storage; do
  echo "[$problem] Concatenating..."
  # Keep image links valid in both the combined Markdown and the PDF.
  sed "s|](figures/|](../dev/$problem/sol/figures/|g" \
    "$DEV_DIR/$problem/sol/solution.md" >> "$EVENT_DIR/sol/solution.md"
  [[ "$problem" == storage ]] || printf '\n\n' >> "$EVENT_DIR/sol/solution.md"
  RESOURCE_PATH+=":$DEV_DIR/$problem/sol"
done

cd "$EVENT_DIR/sol"

echo Building solution.pdf...
TYPST=$(command -v typst || true)
if [[ -z "$TYPST" ]]; then
  REPO_DIR=$(git -C "$DEV_DIR" rev-parse --show-toplevel)
  TYPST="$REPO_DIR/typst-x86_64-unknown-linux-musl/typst"
fi
[[ -x "$TYPST" ]] || { echo 'Typst executable not found.' >&2; exit 1; }
BUILD=$(mktemp -d "$EVENT_DIR/sol/.buildsol.XXXXXX")
trap 'rm -rf -- "$BUILD"' EXIT
cat > "$BUILD/layout.typ" <<'EOF'
#show heading.where(level: 1): it => {
  pagebreak(weak: true)
  it
}
#show raw.where(block: true): set block(breakable: false)
EOF
cat > "$BUILD/layout.yaml" <<'EOF'
papersize: a4
margin:
  x: 20mm
  y: 18mm
EOF
pandoc solution.md -s -t typst -o "$BUILD/solution.typ" \
  --extract-media="$BUILD/media" --resource-path="$RESOURCE_PATH" \
  --include-in-header="$BUILD/layout.typ" --metadata-file="$BUILD/layout.yaml" \
  -V mainfont="Noto Serif CJK SC" -V lang=zh -V page-numbering=1
# Pandoc emits the older .circle spelling; this Typst uses .o for circled signs.
sed -i -e 's/times\.circle/times.o/g' -e 's/plus\.circle/plus.o/g' "$BUILD/solution.typ"
"$TYPST" compile --root / "$BUILD/solution.typ" solution.pdf
