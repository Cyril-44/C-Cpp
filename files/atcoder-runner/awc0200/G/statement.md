# G - Local Specialty Products

https://atcoder.jp/contests/awc0200/tasks/awc0200_g

Time Limit: 2 sec / Memory Limit: 1024 MiB

Score : $366$ pts



### Problem Statement


In the Kingdom of AtCoder, there are $N$ towns numbered $1$ to $N$.


Town $i$ ($1 \le i \le N$) has a local specialty with a value of $A_i$.


There are $M$ roads in the kingdom numbered $1$ to $M$, and road $j$ ($1 \le j \le M$) connects town $U_j$ and town $V_j$ bidirectionally.


Town $v$ is said to be "reachable" from town $u$ if one can reach town $v$ from town $u$ by traversing zero or more roads (thus, every town is reachable from itself).


For each town $i$ ($1 \le i \le N$), find the maximum value of a specialty among all towns reachable from town $i$ (including town $i$ itself).


### Constraints


- $1 \le N \le 2 \times 10^5$


- $0 \le M \le 2 \times 10^5$


- $N + M + 2 \le 2 \times 10^5$ (The total number of lines in the input and output is at most $2 \times 10^5$)


- $1 \le A_i \le 10^9$ ($1 \le i \le N$)


- $1 \le U_j, V_j \le N$ ($1 \le j \le M$)


- There may be multiple roads connecting the same pair of towns, or roads connecting a town to itself (the graph may contain multi-edges and self-loops).


- All input values are integers.



### Input


The input is given from Standard Input in the following format:


```text

$N$ $M$
$A_1$ $A_2$ $\dots$ $A_N$
$U_1$ $V_1$
$U_2$ $V_2$
$\vdots$
$U_M$ $V_M$
```


If $M = 0$, the lines representing roads are not given.


### Output


Print $N$ lines.


The $i$-th line ($1 \le i \le N$) should contain the maximum value of a specialty in a town reachable from town $i$.



### Sample Input 1


```text

4 2
10 30 20 5
1 2
3 4
```


### Sample Output 1


```text

30
30
20
20
```



### Sample Input 2


```text

5 4
50 100 200 40 40
1 1
1 2
2 1
4 5
```


### Sample Output 2


```text

100
100
200
40
40
```



### Sample Input 3


```text

10 9
15 28 999999999 42 15 1000000000 8 300 42 77
1 3
3 5
2 4
4 6
6 8
8 2
7 9
9 10
7 10
```


### Sample Output 3


```text

999999999
1000000000
999999999
1000000000
999999999
1000000000
77
1000000000
77
77
```



### Sample Input 4


```text

25 26
72 15 88 43 91 105 2 34 999 12 56 78 90 23 45 67 89 12 345 678 901 234 567 890 123
1 2
2 3
3 4
4 5
5 1
2 6
6 7
8 9
9 10
10 11
11 12
12 13
13 9
14 15
15 16
16 17
17 18
18 19
19 20
14 18
15 19
21 22
22 23
23 24
24 21
21 23
```


### Sample Output 4


```text

105
105
105
105
105
105
105
999
999
999
999
999
999
678
678
678
678
678
678
678
901
901
901
901
123
```



### Sample Input 5


```text

1 0
1000000000
```


### Sample Output 5


```text

1000000000
```
