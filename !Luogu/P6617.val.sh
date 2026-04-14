#!/usr/bin/env bash
set -e

for ((;;)) do
./P6617.val
./P6617 < P6617.in > P6617.out
diff -b P6617.out P6617.ans
done
