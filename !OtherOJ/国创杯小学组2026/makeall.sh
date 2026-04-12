#!/usr/bin/env bash
set -e
probs=("candy" "year" "eat" "string")
echo "You may need to recompile data/generator.cpp to data/generator, data/validator.cpp to data/validator and std/std.cpp to data/std. Or there could be compatibility issues."
echo "Press the Enter key to make the Final test data folder."
read;
rm -rf Final/data/ Final/down/;
mkdir Final/data Final/down;
for prob in "${probs[@]}"
do
  echo Generating Problem $prob...
  cd $prob/data;
  rm -rf tests/;
  mkdir tests;
  ./make.sh;
  cp -r tests/ ../../Final/data/$prob/;
  cp -r down/ ../../Final/down/$prob/;
  rm -rf tests/;
  cd ../..
done
cp statement.pdf Final/down/