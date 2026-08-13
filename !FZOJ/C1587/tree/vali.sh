#!/usr/bin/bash
set -e
for ((i=1;;i++)) do
./tree_gen > tree.in;
./tree_bf < tree.in > tree.ans
./tree < tree.in > tree.out
diff -Z tree.ans tree.out
echo Round $i Finished.;
done