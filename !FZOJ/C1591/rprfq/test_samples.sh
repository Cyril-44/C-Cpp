#!/usr/bin/env bash

name="rprfq"

for id in ${TC_LIST}; do
  input=${name}${id}.in
  output=${name}${id}.out
  answer=${name}${id}.ans
  log=${name}${id}.log

  ${PROG} < ${input} > ${output}

  if diff ${output} ${answer} > ${log}; then
    echo "Test Case #${id}: accepted"
    rm -f ${output} ${log}
  else
    echo "Test Case #${id}: wrong answer"
  fi
done
