set -e
for ((i=1;;i++)) do
  ./gen > count.in
  ./std < count.in > count.ans
  ./count < count.in > count.out
  diff -b count.out count.ans
  echo Test Case $i: AC
done