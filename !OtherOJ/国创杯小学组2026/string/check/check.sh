#!/bin/bash

IN=check.in
OUT1=check.ans
OUT2=check.out
HACK=check.hack.in

check() {
if ! diff -q $OUT1 $OUT2 >/dev/null; then
    echo "===================="
    echo "Mismatch found $1!"
    # echo "=========in========="
    # cat $IN
    echo "=========out========"
    cat $OUT2
    echo "=========ans========"
    cat $OUT1
    echo "===================="
    exit 1
fi;
}

# n=20
# for ((s=1;s<=1;s++)) do
#     echo $n > $IN
#     # echo 5000000 >> $IN
#     for ((i=1;i<=n;i++)) do
#         tr -dc 'a-z' </dev/urandom | head -c 5000000 >> $IN;
#         # echo 5000000 >> $IN;
#         # for ((j=1;j<=5000000;j++)) do
#         #     echo $(($RANDOM%26+1)) >> $IN
#         # done;
#         echo >> $IN;
#     done
#     # ./std_gpt < $IN > $OUT1
#     # ./std < $IN > $OUT2; check "zsy";
#     # ./std_szs < $IN > $OUT2; check "szs";
#     # ./std_wsy < $IN > $OUT2; check "wsy";
#     # ./std_zzy < $IN > $OUT2; check "zzy";
#     echo "Pass #@"
# done

# exit 0





echo 1 > $IN;
echo $((5*5*5*5*5*5)) >> $IN

# 枚举所有 a[1..8] ∈ [1..8]
for a1 in {a..e}; do
for a2 in {a..e}; do
for a3 in {a..e}; do
for a4 in {a..e}; do
for a5 in {a..e}; do
for a6 in {a..e}; do
# for a7 in {a..f}; do
# for a8 in {1..5}; do
# for a9 in {1..5}; do
# for a10 in {1..5}; do
    echo 1 > $IN;
    echo "$a1$a2$a3$a4$a5$a6" >> $IN
    ./bf < $IN > $OUT1
    ./std < $IN > $OUT2
    if ! diff -q $OUT1 $OUT2 >/dev/null; then
        echo "$a1$a2$a3$a4$a5$a6" >> $HACK
    fi
#     echo "10 $a1 $a2 $a3 $a4 $a5 $a6 $a7 $a8 $a9 $a10" >> $IN

# done; done; done;
# done;
done; done; done; done; done; done
# 比较
echo "All $cnt tests passed!"
