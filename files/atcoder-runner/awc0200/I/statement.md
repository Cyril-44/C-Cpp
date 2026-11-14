# I - Drone Delivery and Altitude Control

https://atcoder.jp/contests/awc0200/tasks/awc0200_i

Time Limit: 2 sec / Memory Limit: 1024 MiB

Score : $433$ pts



### Problem Statement


Takahashi is delivering a package from base $1$ to base $N$ using a drone.


The flight altitude of the drone is represented by an integer between $0$ and $K$, inclusive. Initially, the drone is at base $1$ at an altitude of $S$. He wants to eventually arrive at base $N$ with an altitude of exactly $T$.


The drone is equipped with an altitude limiter device, and an allowed altitude range must be configured before departure. Takahashi chooses a pair of integers $U, V$ ($0 \leq U \leq V \leq K$) before departure. Setting the altitude range to $[U, V]$ takes $G \times (V - U)$ time to adjust the device.


Throughout the entire trip, the drone's altitude must always be at least $U$ and at most $V$. Therefore, $U \leq S \leq V$ and $U \leq T \leq V$ are required. The configured altitude range cannot be changed en route.


There are $M$ one-way flight paths connecting bases. There may be multiple flight paths between the same pair of bases. Flight path $j$ goes from base $A_j$ to base $B_j$.


When the altitude is $x$, flight path $j$ can be traversed if and only if $L_j \leq x \leq R_j$, and the time required to traverse it is


$C_j + D_j \times |x - H_j|$


Here, $H_j$ is the reference altitude of flight path $j$, and is not necessarily within the range $[L_j, R_j]$. Traversing a flight path does not change the altitude.


While staying at each base $i$ (including base $1$ and base $N$), the following operations can each be performed any number of times in any order:


- Increase the altitude by $1$. This operation takes $P_i$ time.


- Decrease the altitude by $1$. This operation takes $Q_i$ time.


However, the operations must not cause the altitude to become less than $U$ or exceed $V$.


The drone may visit the same base multiple times and traverse the same flight path multiple times.


Takahashi wants to minimize the total required time by freely choosing the altitude range $[U, V]$, the altitude adjustments at bases, and the flight paths to take. Here, the total required time is the sum of the time to adjust the altitude range $G \times (V - U)$, the total time spent adjusting altitude at bases, and the total time spent traversing flight paths.


Find the minimum total required time to start from base $1$ at altitude $S$ and reach base $N$ at altitude $T$. If it is impossible to achieve this, print NA.


### Constraints


- $2 \leq N \leq 1000$


- $0 \leq M \leq 5000$


- $1 \leq K \leq 10$


- $0 \leq G \leq 10^6$


- $0 \leq S \leq K$


- $0 \leq T \leq K$


- $0 \leq P_i \leq 10^6$ $(1 \leq i \leq N)$


- $0 \leq Q_i \leq 10^6$ $(1 \leq i \leq N)$


- $1 \leq A_j \leq N,\ 1 \leq B_j \leq N,\ A_j \neq B_j$ $(1 \leq j \leq M)$


- $0 \leq L_j \leq R_j \leq K$ $(1 \leq j \leq M)$


- $0 \leq H_j \leq K$ $(1 \leq j \leq M)$


- $0 \leq C_j \leq 10^6$ $(1 \leq j \leq M)$


- $0 \leq D_j \leq 10^6$ $(1 \leq j \leq M)$


- All input values are integers.



### Input


```text

$N$ $M$ $K$ $G$
$S$ $T$
$P_1$ $Q_1$
$P_2$ $Q_2$
$\vdots$
$P_N$ $Q_N$
$A_1$ $B_1$ $L_1$ $R_1$ $C_1$ $H_1$ $D_1$
$A_2$ $B_2$ $L_2$ $R_2$ $C_2$ $H_2$ $D_2$
$\vdots$
$A_M$ $B_M$ $L_M$ $R_M$ $C_M$ $H_M$ $D_M$
```


- The first line contains the number of bases $N$, the number of flight paths $M$, the maximum altitude $K$, and the adjustment time $G$ per unit width of the altitude range, separated by spaces.


- The second line contains the initial altitude $S$ and the target altitude $T$, separated by spaces.


- The following $N$ lines give the time required for altitude adjustment at each base.


- The $(2+i)$-th line contains the time $P_i$ to increase altitude by $1$ and the time $Q_i$ to decrease altitude by $1$ at base $i$, separated by spaces.


- The following $M$ lines give the information of each flight path.


- The $(2+N+j)$-th line contains the start base $A_j$, the end base $B_j$, the lower bound $L_j$ and upper bound $R_j$ of the passable altitude range, the base time $C_j$, the reference altitude $H_j$, and the additional time coefficient for altitude deviation $D_j$ of flight path $j$, separated by spaces.


- If $M=0$, no lines representing flight path information are given.


### Output


Print in one line the minimum total required time to start from base $1$ at altitude $S$ and reach base $N$ at altitude $T$. If it is impossible to achieve this, print NA.



### Sample Input 1


```text

3 3 4 5
1 3
10 10
2 2
10 10
1 2 1 3 20 2 3
2 3 2 4 15 3 2
1 3 1 4 100 2 1
```


### Sample Output 1


```text

52
```



### Sample Input 2


```text

3 2 3 10
1 2
5 5
5 5
5 5
1 2 0 3 10 1 1
3 2 0 3 10 1 1
```


### Sample Output 2


```text

NA
```



### Sample Input 3


```text

6 7 6 15
0 6
20 5
3 30
25 25
4 4
15 15
10 10
1 2 0 2 50 1 10
1 3 0 4 80 2 5
2 4 1 5 40 3 8
3 4 2 6 30 4 2
4 5 3 6 45 5 6
2 5 0 3 120 2 1
5 6 4 6 25 6 3
```


### Sample Output 3


```text

287
```



### Sample Input 4


```text

15 22 10 100
3 7
50 30
20 20
15 40
35 10
25 25
45 15
10 50
30 30
20 15
40 25
15 35
25 20
30 40
10 10
50 50
1 2 2 5 120 3 15
1 3 0 4 200 2 10
2 4 3 6 85 4 20
2 5 1 5 150 2 12
3 5 2 7 110 5 8
3 6 0 3 180 1 25
4 7 4 8 90 6 15
4 8 2 6 130 3 10
5 8 3 7 75 5 18
5 9 1 6 160 4 14
6 9 0 5 140 2 22
7 10 5 9 105 7 12
8 10 4 8 80 6 16
8 11 3 7 115 4 20
9 11 2 6 95 5 15
9 12 1 8 170 3 10
10 13 6 10 130 8 25
10 14 5 9 85 7 14
11 14 4 8 100 6 18
12 14 2 7 125 5 12
13 15 7 10 110 8 20
14 15 5 10 90 7 15
```


### Sample Output 4


```text

1055
```



### Sample Input 5


```text

2 2 3 0
0 3
0 100
100 0
1 2 2 3 50 0 10
1 2 1 1 0 3 5
```


### Sample Output 5


```text

80
```
