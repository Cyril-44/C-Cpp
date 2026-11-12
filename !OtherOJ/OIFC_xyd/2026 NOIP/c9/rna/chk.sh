set -e

for ((i=1;;i++)) do
  echo Testcase $i...
  ./gen > rna.in
  ./rna.bf < rna.in > rna.ans
  ./rna
  diff -Z rna.ans rna.out || break
done