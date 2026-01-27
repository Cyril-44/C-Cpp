for ((i=1; i<=6; i++)) do
#   ./P2740_gen $i $RANDOM > P2740_$i.in
  ./P2740_bot < P2740_$i.in > P2740_$i.ans
  echo Finished $i.
done
