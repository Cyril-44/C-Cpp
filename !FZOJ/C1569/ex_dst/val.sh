#!/usr/bin/env bash
set -e
for ((i=1;;i++)) do
echo Testing $i...
./dst.gen > dst.test.in;
./dst < dst.test.in > dst.test.out;
./dst.bf < dst.test.in > dst.test.ans 2>dst.test.dbg;
diff -b dst.test.ans dst.test.out;
done
