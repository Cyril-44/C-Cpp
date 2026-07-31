set -e

for ((i=1;;i++)) do
./gen > data.in
./vspirit < data.in > data.out
./bf < data.in > data.ans
diff -b data.ans data.out
echo "Doing $i..."
done
