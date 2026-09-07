set -e
for ((i=1;;i++)) do
./gen > move.in
./move_bf < move.in > move.ans
./move < move.in > move.out
diff -b move.out move.ans
echo Round $i Finished.
done