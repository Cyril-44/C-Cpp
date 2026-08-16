#!/bin/bash
set -e
for ((i=1;;i++)) do
    ./gen > coin.in
    ./bf < coin.in > coin.ans
    ./coin < coin.in > coin.out
    diff -b coin.out coin.ans
    if ((i%100==0)); then
        echo Round $i Finished
    fi
done