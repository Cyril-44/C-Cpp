#!/usr/bin/env bash
set -e

echo Compiling string executables...
g++ -std=c++2a generator.cpp -o generator -O2 -I..
g++ -std=c++2a validator.cpp -o validator -O2 -I..
g++ -std=c++2a std.cpp -o std -O2 -Wno-unused-result

TOKEN=ip8MTdlE4jwqqXWvjcbaWb5l6wzzvANwLOMz4xL+---WTZAKIOI---pnbQy0uEfKomEQjkhU2ru3a/NPGfg7CdntX89UcV
echo Generating all data...
./generator $TOKEN
echo Data Generated.

for ((i=1; i<=20; i++)) do
  echo Validating data $i...
  ./validator < tests/dat$i.in;
  echo Running on data $i...
  ./std < tests/dat$i.in > tests/dat$i.ans;
done

cp -r down.default.d/* down/
for ((i=2; i<=7; i++)) do
  echo Validating down $i...
  ./validator < down/string$i.in;
  echo Running on down $i...
  ./std < down/string$i.in > down/string$i.ans;
done

rm generator validator std
