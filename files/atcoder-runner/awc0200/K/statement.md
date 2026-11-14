# K - Fence Poster

https://atcoder.jp/contests/awc0200/tasks/awc0200_k

Time Limit: 4 sec / Memory Limit: 1024 MiB

Score : $466$ pts



### Problem Statement


In Takahashi's garden, there is a fence consisting of $N$ boards arranged in a row from left to right without any gaps, with their bottom edges aligned.


The $i$-th board from the left $(1 \le i \le N)$ has a width of $1$ and a height of $A_i$.


Takahashi wants to put up a single rectangular poster without it extending beyond the fence.


Specifically, he decides to place the poster such that the following conditions are satisfied:


- Choose a contiguous range of boards $[L, R]$ $(1 \le L \le R \le N)$ and a positive integer $H$ $(1 \le H \le \min_{L \le i \le R} A_i)$.


- Place a rectangular poster of width $R - L + 1$ and height $H$ over the chosen range. The area of the poster will be $(R - L + 1) \times H$.


Takahashi wants to place a poster with an area of at least $S$.


Determine whether there is a way to place a poster that satisfies the conditions. If it exists, print the minimum possible width $R - L + 1$ of the poster.


If no such placement exists, print $-1$ instead.


### Constraints


- $1 \le N \le 5 \times 10^5$


- $1 \le S \le 5 \times 10^{14}$


- $1 \le A_i \le 10^9$


- All input values are integers.



### Input


The input is given from Standard Input in the following format:


```text

$N$ $S$
$A_1$ $A_2$ $\dots$ $A_N$
```


### Output


Print the minimum possible width $R - L + 1$ of a poster with an area of at least $S$ in a single line. If no such placement exists, print $-1$.



### Sample Input 1


```text

5 8
2 4 4 3 1
```


### Sample Output 1


```text

2
```



### Sample Input 2


```text

4 20
3 2 4 2
```


### Sample Output 2


```text

-1
```



### Sample Input 3


```text

10 15
3 5 2 7 6 8 4 2 1 5
```


### Sample Output 3


```text

3
```



### Sample Input 4


```text

24 2500000000
300000000 450000000 200000000 800000000 750000000 700000000 720000000 500000000 100000000 600000000 650000000 600000000 400000000 350000000 800000000 700000000 200000000 500000000 550000000 500000000 600000000 400000000 300000000 100000000
```


### Sample Output 4


```text

4
```



### Sample Input 5


```text

1 1000000000
1000000000
```


### Sample Output 5


```text

1
```
