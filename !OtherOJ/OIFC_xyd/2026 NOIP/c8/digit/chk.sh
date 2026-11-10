set -e

for ((i=1;;i++)) do
  echo "Test $i"
  ./gen > digit.in
  ./digitbf < digit.in > digit.ans
  ./digit
  diff -Z digit.ans digit.out
done