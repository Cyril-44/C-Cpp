#!/usr/bin/env bash
set -e
for ((i=1; ;i++)) do
echo Round $i;
./gen > test_match.in
./bf < test_match.in > test_match.ans
./match < test_match.in > test_match.out
if ! diff test_match.ans test_match.out; then
    echo "WA on test_match.in"
    exit 1
fi
done