probs=("candy" "eat" "string" "year")
rm -rf data/ down/;
mkdir data down;
for prob in "${probs[@]}"
do
  echo Generating Problem $prob...
  cd $prob/data;
  rm -rf tests/;
  mkdir tests;
  ./make.sh;
  cp tests/* ../../data/$prob/;
  cp down/* ../../down/$prob/;
  cd ../..
done
