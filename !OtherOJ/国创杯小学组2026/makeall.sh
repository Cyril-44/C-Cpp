#!/usr/bin/env bash
set -e
probs=("candy" "year" "eat" "string")
rm -rf Final/data/ Final/down/;
mkdir Final/data Final/down;
for prob in "${probs[@]}"
do
  echo Generating Problem $prob...
  cd $prob;
  rm -rf tests/ down/;
  mkdir tests down;
  ./make.sh;
  cp -r tests/ ../Final/data/$prob/;
  cp -r down/ ../Final/down/$prob/;
  rm -rf tests/ down/;
  cd ..
done
cp statement.pdf Final/down/
cp solution.pdf Final/std/
