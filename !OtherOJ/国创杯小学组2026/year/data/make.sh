#!/usr/bin/env bash
set -e

TOKEN=K1aTMmr6cy/jPicJuuW+aqo2KzycRlvNj9efmye---WTZAKIOI---+rlCmHdoEOrMJi88wTLKYR3bRjxa6zFBL6lnG/Sr
echo Generating all data...
./generator $TOKEN
echo Data Generated.

for ((i=1; i<=20; i++)) do
  echo Validating data $i...
  ./validator < tests/dat$i.in;
  echo Running on data $i...
  ./std < tests/dat$i.in > tests/dat$i.ans;
done

for ((i=2; i<=6; i++)) do
  echo Validating down $i...
  ./validator < down/year$i.in;
  echo Running on down $i...
  ./std < down/year$i.in > down/year$i.ans;
done