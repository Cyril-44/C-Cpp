g++ chain.gen.cpp -O2 -o gen
# g++ chain.cpp -O2 -o chain

for i in {1..1000}
do
    echo "Testing Round $i"
    ./gen > chain.in
    ./chainbf; cp chain.out chain.ans;
    ./chain > chain.err
    diff chain.out chain.ans || break
done

