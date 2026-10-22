#!/usr/bin/env bash
set -euo pipefail

if [[ $# -ne 2 ]]; then
  echo "Usage: $0 <executable> <data-directory>" >&2
  exit 2
fi

program=$1
data_dir=$2
time_limit=${TIME_LIMIT:-5}
if [[ ! -x "$program" ]]; then
  echo "Program is not executable: $program" >&2
  exit 2
fi
if [[ ! -d "$data_dir" ]]; then
  echo "Data directory does not exist: $data_dir" >&2
  exit 2
fi

tmp_dir=$(mktemp -d)
trap 'rm -rf "$tmp_dir"' EXIT

normalize_output() {
  # awk emits a newline for every record, including an unterminated last one.
  awk '{ sub(/[[:space:]]+$/, ""); print }' "$1" > "$2"
}

mapfile -d '' input_files < <(find "$data_dir" -type f -name '*.in' -print0 | sort -z)
if [[ ${#input_files[@]} -eq 0 ]]; then
  echo "No .in files found under: $data_dir" >&2
  exit 2
fi

passed=0
for input in "${input_files[@]}"; do
  answer=${input%.in}.ans
  if [[ ! -f "$answer" && -f "${input%.in}.out" ]]; then
    answer=${input%.in}.out
  fi
  if [[ ! -f "$answer" ]]; then
    echo "Missing answer file: $answer" >&2
    exit 2
  fi

  actual_raw="$tmp_dir/actual.raw"
  actual_norm="$tmp_dir/actual.norm"
  answer_norm="$tmp_dir/answer.norm"
  if timeout "${time_limit}s" "$program" < "$input" > "$actual_raw"; then
    :
  else
    status=$?
    echo "Runtime failure or time limit exceeded (status $status): $input" >&2
    exit 1
  fi

  normalize_output "$actual_raw" "$actual_norm"
  normalize_output "$answer" "$answer_norm"
  if ! diff -u "$answer_norm" "$actual_norm"; then
    echo "Wrong answer: $input" >&2
    exit 1
  fi
  ((passed += 1))
  echo "PASS: $input"
done

echo "Accepted: $passed test file(s)."
