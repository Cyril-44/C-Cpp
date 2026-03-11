#!/usr/bin/env bash
set -e

for ((;!$?;)) do
./gen > sale.in
./std < sale.in > sale.ans
/home/cyril/C-Cpp/.vscode/a.out < sale.in > sale.out
diff -b sale.out sale.ans
done