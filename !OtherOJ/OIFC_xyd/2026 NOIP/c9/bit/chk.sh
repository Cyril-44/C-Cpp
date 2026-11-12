set -e

for ((i=1;;i++)) do
  ./gen > bit.in
  ./bit_bf < bit.in > bit.ans
  ./bit
  diff -Z bit.ans bit.out || break
done