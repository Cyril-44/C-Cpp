for ((i=1;i<=6;i++)) do
  cp rna$i.in rna.in
  ./rna
  diff -Z rna.out rna$i.out > /dev/null && echo "Testcase $i: AC" || echo "Testcase $i: WA"
done