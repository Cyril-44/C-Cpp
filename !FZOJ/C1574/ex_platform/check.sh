#!/bin/bash
set -e

for ((i=1; ;i++)) do
    echo Validating on Test $i....
    ./gen > .checker.in
    ./platform_bf < .checker.in > .checker.ans
    ./platform < .checker.in > .checker.out
    diff -b .checker.ans .checker.out
done
