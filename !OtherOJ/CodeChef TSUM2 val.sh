#!/bin/bash

set -e

for ((i=1;;i++)) do
  printf "Round %d\n" $i
  ./"CodeChef TSUM2 gen" > "CodeChef TSUM2.val.in"
  ./"CodeChef TSUM2 std" < "CodeChef TSUM2.val.in" > "CodeChef TSUM2.val.ans"
  ./"CodeChef TSUM2 树上求和 点分治+李超"  < "CodeChef TSUM2.val.in" > "CodeChef TSUM2.val.out"
  diff -b "CodeChef TSUM2.val.out" "CodeChef TSUM2.val.ans"
done