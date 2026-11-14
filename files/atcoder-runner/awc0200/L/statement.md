# L - Satisfaction of the Mountain Trail

https://atcoder.jp/contests/awc0200/tasks/awc0200_l

Time Limit: 2 sec / Memory Limit: 1024 MiB

Score : $500$ pts



### Problem Statement


Takahashi is going to hike along a trail consisting of $N$ sections.


The $i$-th section ($1 \leq i \leq N$) has a scenic beauty value $S_i$ and a tranquility value $B_i$.


Takahashi will select one or more consecutive sections to hike through.


Specifically, he chooses integers $l, r$ ($1 \leq l \leq r \leq N$) and walks from section $l$ to section $r$.


The satisfaction of the hike is defined as follows:


$S_l + S_{l+1} + \cdots + S_r + \min(B_l, B_{l+1}, \ldots, B_r)$


That is, the sum of the scenic beauty values of all sections walked, plus the minimum tranquility value among all sections walked.


Takahashi wants to choose a range to walk such that the satisfaction is at least $K$.


Find the number of pairs $(l, r)$ for which the satisfaction is at least $K$.


### Constraints


- $1 \leq N \leq 2 \times 10^5$


- $1 \leq K \leq 2 \times 10^{14}$


- $1 \leq S_i \leq 10^9$


- $1 \leq B_i \leq 10^9$


- All inputs are integers



### Input


```text

$N$ $K$
$S_1$ $B_1$
$S_2$ $B_2$
$\vdots$
$S_N$ $B_N$
```


The first line contains the number of sections $N$ and the required lower bound on satisfaction $K$, separated by a space.


The following $N$ lines each contain, on the $i$-th line ($1 \leq i \leq N$), the scenic beauty $S_i$ and tranquility $B_i$ of the $i$-th section, separated by a space.


### Output


Output in one line the number of pairs $(l, r)$ for which the satisfaction is at least $K$.



### Sample Input 1


```text

5 10
3 4
2 5
6 1
1 7
4 3
```


### Sample Output 1


```text

6
```



### Sample Input 2


```text

4 50
5 3
4 2
3 5
2 1
```


### Sample Output 2


```text

0
```



### Sample Input 3


```text

12 25
8 6
3 10
5 4
7 8
2 3
9 5
4 12
6 2
1 9
10 7
3 6
5 1
```


### Sample Output 3


```text

38
```



### Sample Input 4


```text

30 250
12 80
25 60
7 100
18 45
30 70
5 20
22 90
14 35
40 55
11 75
9 15
33 65
16 85
28 25
6 95
21 50
13 40
37 30
19 110
8 10
45 58
24 72
10 38
31 88
17 22
26 68
4 120
35 44
15 78
29 18
```


### Sample Output 4


```text

191
```



### Sample Input 5


```text

1 200000000000000
1000000000 1000000000
```


### Sample Output 5


```text

0
```
