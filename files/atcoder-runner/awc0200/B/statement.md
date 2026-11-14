# B - Market Price Movements

https://atcoder.jp/contests/awc0200/tasks/awc0200_b

Time Limit: 2 sec / Memory Limit: 1024 MiB

Score : $233$ pts



### Problem Statement


Takahashi recorded the market price of a certain product for $N$ days. The price on the $i$-th day was $A_i$ yen.


For each day from day 2 to day $N$, we call a day an *up day* if the price increased compared to the previous day, a *down day* if it decreased, and a *flat day* if it stayed the same.


Find the number of up days, down days, and flat days.


### Constraints


- $2 \leq N \leq 100$


- $1 \leq A_i \leq 10000$


- All input values are integers.



### Input


The input is given from standard input in the following format:


```text

$N$
$A_1$ $A_2$ $\ldots$ $A_N$
```


- The first line contains the number of recorded days $N$.


- The second line contains the prices $A_1, A_2, \ldots, A_N$ for each day, separated by spaces.


### Output


Print the number of up days, the number of down days, and the number of flat days in this order, separated by spaces, on a single line.



### Sample Input 1


```text

3
100 120 90
```


### Sample Output 1


```text

1 1 0
```



### Sample Input 2


```text

4
10 20 30 40
```


### Sample Output 2


```text

3 0 0
```



### Sample Input 3


```text

3
50 40 30
```


### Sample Output 3


```text

0 2 0
```



### Sample Input 4


```text

3
1301 2362 8372
```


### Sample Output 4


```text

2 0 0
```



### Sample Input 5


```text

2
2 5
```


### Sample Output 5


```text

1 0 0
```
