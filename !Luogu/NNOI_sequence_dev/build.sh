g++ gen1.cpp -O2 -I../../testlib -o gen1
g++ gen2.cpp -O2 -I../../testlib -o gen2
mkdir -p sequence1
mkdir -p sequence2
cd sequence1
../gen1 WTZ AK IOI APIO
cd ../sequence2
../gen2 WTZ AK IOI APIO
cd ..
rm gen1 gen2
cp interactive_lib1.cpp sequence1/interactive_lib.cpp
cp interactive_lib2.cpp sequence2/interactive_lib.cpp
cp checker1.cpp sequence1/checker.cpp
cp checker2.cpp sequence2/checker.cpp
cp config1.yml sequence1/config.yml
cp config2.yml sequence2/config.yml
cd sequence1
touch sequence{1..20}.out
zip ../sequence1.zip * >/dev/null
cd ../sequence2
touch sequence{1..32}.out
zip ../sequence2.zip * >/dev/null
cd ..
rm -r sequence1 sequence2
