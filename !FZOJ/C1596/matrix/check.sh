set -e
for ((i=1;;i++)) do
./gen > matrix.in
./matrix_xyx < matrix.in > matrix.out
./checker matrix.in matrix.out
echo Round $i Finished.;
done