set -e
for ((i=1;;i++)) do
  ./gen > nodes_chk.in
  timeout 3s ./nodes < nodes_chk.in > /dev/null
done