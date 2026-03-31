IN="bf_check.in"

echo 0 $((15*15*14*14*15)) > $IN
for m in {1..15}; do
for a in {1..15}; do
for b in {2..15}; do
for c in {2..15}; do
for n in {1..15}; do

echo $m $a $((a*b)) $((a*b*c)) $n >> $IN;

done;done;done;done;done
