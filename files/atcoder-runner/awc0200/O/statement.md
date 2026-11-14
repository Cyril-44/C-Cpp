# O - Watering Plants

https://atcoder.jp/contests/awc0200/tasks/awc0200_o

Time Limit: 2 sec / Memory Limit: 1024 MiB

Score : $533$ pts



### Problem Statement


Takahashi is growing $N$ plants in a greenhouse. Each plant has an associated current water level, and the water level of the $i$-th plant is $T_i$. A plant with a water level of $0$ or less will wither.


There is only one watering can in the greenhouse. Takahashi can restore the water levels of the plants by watering them with this can. Specifically, at the beginning of each second, Takahashi performs the following operation:


- First, Takahashi chooses exactly one of the $N$ plants and waters it with the watering can. The water level of the watered plant increases by $P$.


- After that, the water level of all plants decreases by $1$ (including the plant that was just watered). This represents evaporation due to the high temperature in the greenhouse.


Takahashi wants to keep all plants from withering (i.e., keep the water level of all plants at $1$ or more) for as long as possible.


Assuming Takahashi acts optimally, if the number of seconds he can keep the water level of all plants at $1$ or more is finite, output the maximum such number of seconds. If he can keep them alive forever, output -1.


Here, the number of seconds refers to the number of operations performed. That is, if the water level of any plant is $0$ or less before performing any operations, the answer is $0$. Find the maximum $k$ such that the water level of all plants is $1$ or more immediately after performing the operation $k$ times (or -1 if it can be kept alive forever).


### Constraints


- $1 \leq N \leq 2 \times 10^5$


- $1 \leq P \leq 10^4$


- $1 \leq T_i \leq 10^9$


- All input values are integers.



### Input


```text

$N$ $P$
$T_1$ $T_2$ $\ldots$ $T_N$
```


- The first line contains $N$, the number of plants, and $P$, the amount of water level increase per watering, separated by a space.


- The second line contains the initial water levels of each plant, $T_1, T_2, \ldots, T_N$, separated by spaces.


### Output


Print the maximum number of operations for which the water level of all plants can be kept at $1$ or more. If they can be kept alive forever, print -1.



### Sample Input 1


```text

2 1
2 3
```


### Sample Output 1


```text

3
```



### Sample Input 2


```text

1 1
1
```


### Sample Output 2


```text

-1
```



### Sample Input 3


```text

6 3
4 8 2 10 5 7
```


### Sample Output 3


```text

7
```



### Sample Input 4


```text

12 5
15 3 22 8 19 6 11 30 4 17 25 9
```


### Sample Output 4


```text

17
```



### Sample Input 5


```text

1 10000
1000000000
```


### Sample Output 5


```text

-1
```
