#!/bin/bash

set -e

for ((i=1;;i++)) do
    echo Round $i
    ./gen > check.in
    ./simu < check.in > check.ans
    ./goldenfield.grok < check.in > check.out
    diff -b check.ans check.out || break
done