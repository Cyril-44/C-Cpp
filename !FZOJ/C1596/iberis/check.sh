set -e
for ((i=1; ;i++)) do
./gen > iberis.in;
./iberis < iberis.in > iberis.out;
./iberis_bf < iberis.in > iberis.ans;
diff -b iberis.out iberis.ans;
done
