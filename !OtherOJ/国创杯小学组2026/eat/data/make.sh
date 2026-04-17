#!/usr/bin/env bash
set -e

TOKEN=raBfcfOwRyCN9gyLt9QhMiJNx6y81rRyDeWf1C7y---WTZAKIOI---mfeSrPEVQlEr6jAC3qG89x7SxNzLe5iL9wbqSV6x
echo Generating all data...
./generator $TOKEN
echo Data Generated.

for ((i=1; i<=10; i++)) do
  echo Validating data $i...
  ./validator --ans=tests/dat$i.ans < tests/dat$i.in;
  echo Running on data $i...
  ./std < tests/dat$i.in > tests/dat.out;
  diff -b tests/dat$i.ans tests/dat.out;
done
rm tests/dat.out;

for ((i=2; i<=5; i++)) do
  echo Validating down $i...
  ./validator --ans=down/eat$i.ans < down/eat$i.in;
  echo Running on down $i...
  ./std < down/eat$i.in > down/eat.out;
done
rm down/eat.out;
