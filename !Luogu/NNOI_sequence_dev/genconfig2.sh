#!/bin/bash

output="config2.yml"
> "$output"

# 每个子任务的测试点数量
declare -A subtask_ranges=(
  [1]="1 4"
  [2]="5 10"
  [3]="11 14"
  [4]="15 20"
  [5]="21 26"
  [6]="27 50"
)
declare -A subtask_scores=(
  [1]=10
  [2]=15
  [3]=10
  [4]=20
  [5]=10
  [6]=35
)

# 默认资源限制
timeLimit=1000
memoryLimit="524288"

# 遍历每个子任务范围
for subtask in "${!subtask_ranges[@]}"; do
  range=(${subtask_ranges[$subtask]})
  start=${range[0]}
  end=${range[1]}
  score=${subtask_scores[$subtask]}
  for ((i=start; i<=end; i++)); do
    echo "sequence$i.in:" >> "$output"
    echo "  timeLimit: $timeLimit" >> "$output"
    echo "  memoryLimit: $memoryLimit" >> "$output"
    echo "  score: $score" >> "$output"
    echo "  subtaskId: $subtask" >> "$output"
    echo "" >> "$output"
  done
done
