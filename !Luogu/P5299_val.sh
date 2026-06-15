#!/bin/bash
set -e
for ((i=1; i<=100; i++)); do
    echo "Running test $i..."
    ./P5299_gen > P5299.in
    ./P5299 < P5299.in > P5299.out 2>P5299.dbg
    ./P5299_std < P5299.in > P5299.ans 2>P5299.std
    if diff -q P5299.out P5299.ans > /dev/null; then
        echo "Test $i passed."
    else
        echo "Test $i failed."
        echo "Input:"
        cat P5299.in
        echo "Output:"
        cat P5299.out
        echo "Expected:"
        cat P5299.ans
        exit 1
    fi
done
echo "All tests passed."