#!/bin/bash
set -e
for ((i=1;1;i++)) do
  ./P5163_gen 5 5 5 6 > P5163_chk.in
  ./P5163_mzx < P5163_chk.in > P5163_chk.ans
  ./P5163 < P5163_chk.in > P5163_chk.out 2> P5163_chk.dbg
  diff -b P5163_chk.ans P5163_chk.out && echo AC $i || exit 2
done