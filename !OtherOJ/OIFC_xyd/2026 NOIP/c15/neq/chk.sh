set -e
for ((i=1;;i++)) do
  echo Testcase $i...
  ./gen 100 5 20 > neq.in
  ./neq
done
  