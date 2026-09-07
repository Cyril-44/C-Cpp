#!/bin/bash

# 编译所有程序
g++ std.cpp -o std -O2
g++ gen.cpp -o gen -O2
g++ val.cpp -o val -O2

mkdir -p data samples

echo "Generating 20 regular test cases..."
for i in {1..20}
do
    # 模拟不同梯度的分布
    if [ $i -le 4 ]; then TYPE=1; # 10%
    elif [ $i -le 10 ]; then TYPE=2; # 30%
    elif [ $i -le 12 ]; then TYPE=4; # zero case
    else TYPE=3; # 100%
    fi
    
    ./gen $TYPE $RANDOM > data/test$i.in
    ./val < data/test$i.in
    if [ $? -eq 0 ]; then
        ./std < data/test$i.in > data/test$i.out
        echo "Case $i finished."
    else
        echo "Case $i validation failed!"
    fi
done

echo "Generating 3 specialized large samples..."
./gen 1 999 > samples/noise1.in && ./std < samples/noise1.in > samples/noise1.ans
./gen 2 888 > samples/noise2.in && ./std < samples/noise2.in > samples/noise2.ans
./gen 3 777 > samples/noise3.in && ./std < samples/noise3.in > samples/noise3.ans
./gen 4 666 > samples/noise4.in && ./std < samples/noise4.in > samples/noise4.ans
./gen 5 555 > samples/noise5.in && ./std < samples/noise5.in > samples/noise5.ans

echo "All data generated in /data folder."

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
