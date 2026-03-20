#!/bin/bash
set -e

mkdir -p tests
cd tests

# 生成一个测试点
# 参数: SID CID n amax mode
gen_one() {
    local SID=$1
    local CID=$2
    local n=$3
    local amax=$4
    local mode=$5

    printf -v fname "data%d%02d" $SID $CID
    ../gen $n $amax $mode > ${fname}.in
    touch ${fname}.ans
    echo "Generated ${fname}.in / ${fname}.ans  (SID=$SID, CID=$CID, n=$n, amax=$amax, mode=$mode)"
}

########################################
# Subtask 1: 2 cases
########################################
SID=1
CID=1
gen_one $SID $CID 1 1 0; CID=$((CID+1))
gen_one $SID $CID 1 1 0

########################################
# Subtask 2: 12 cases
########################################
SID=2
CID=1
for i in {1..12}; do
    n=$(shuf -i1-10 -n1)
    gen_one $SID $CID $n 100 0
    CID=$((CID+1))
done

########################################
# Subtask 3: 14 cases
########################################
SID=3
CID=1
for i in {1..14}; do
    n=$(shuf -i50-100 -n1)
    gen_one $SID $CID $n 2000 0
    CID=$((CID+1))
done

########################################
# Subtask 4: 16 cases
########################################
SID=4
CID=1
for i in {1..16}; do
    n=$(shuf -i200-500 -n1)
    gen_one $SID $CID $n 200000 0
    CID=$((CID+1))
done

########################################
# Subtask 5: 20 cases
########################################
SID=5
CID=1
for i in {1..20}; do
    n=$(shuf -i1000-2022 -n1)
    gen_one $SID $CID $n 200000 0
    CID=$((CID+1))
done

########################################
# Subtask 6: 18 cases (sum close to 515n)
########################################
SID=6
CID=1
for i in {1..18}; do
    n=$(shuf -i1000-2022 -n1)
    gen_one $SID $CID $n 200000 1
    CID=$((CID+1))
done

########################################
# Subtask 7: 18 cases (sum very small)
########################################
SID=7
CID=1
for i in {1..18}; do
    n=$(shuf -i1000-2022 -n1)
    gen_one $SID $CID $n 200000 2
    CID=$((CID+1))
done

echo "All testcases generated."
