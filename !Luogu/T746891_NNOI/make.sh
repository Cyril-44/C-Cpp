#!/bin/bash

# 编译
g++ std.cpp -o std -O2
g++ gen.cpp -o gen -O2
g++ val.cpp -o val -O2 -I/home/cyril/code/testlib

mkdir -p data samples

generate() {
    local n=$1
    local q=$2
    local type=$3
    local id=$4
    echo "Generating Case $id: n=$n, q=$q, type=$type"
    ./gen $n $q $RANDOM $type > $id.in
    ./val < $id.in
    ./std < $id.in > $id.out
}

# Subtask 1: n,q <= 100 (4 cases)
for i in {1..4}; do generate 100 100 0 "data/data$i"; done

# Subtask 2: n,q <= 3000 (4 cases)
for i in {5..8}; do generate 3000 3000 0 "data/data$i"; done

# Subtask 3: k = 1 (6 cases)
for i in {9..14}; do generate 1000000 1000000 1 "data/data$i"; done

# Subtask 4: n,q <= 10^6 (6 cases)
for i in {15..20}; do generate 1000000 1000000 2 "data/data$i"; done

# 大样例 (3 samples)
generate 100 100 0 "samples/ex1"
generate 3000 3000 0 "samples/ex2"
generate 1000000 1000000 2 "samples/ex3"

echo "All data generated successfully."

rm std gen val
cp ../tuackCodeTheme.tmTheme .
cp ../NNOI_Default_template.typ .default.temp.typ
echo "#include \"statement.typ\"" >> .default.temp.typ
pandoc statement.typ -o statement.tex --highlight-style=none
typst compile .default.temp.typ statement.pdf
cp ../NNOI_Default_template.typ .default.temp.typ
echo "#include \"solution.typ\"" >> .default.temp.typ
pandoc solution.typ -o solution.tex --highlight-style=none
typst compile .default.temp.typ solution.pdf

name=$(basename "$PWD")
clean=${name%_NNOI}
rm -f ${clean}_data.zip
zip -r ${clean}_data.zip data/ samples/ statement.tex statement.pdf solution.tex solution.pdf

rm statement.pdf statement.tex solution.pdf solution.tex tuackCodeTheme.tmTheme .default.temp.typ
rm -rf data/ samples/