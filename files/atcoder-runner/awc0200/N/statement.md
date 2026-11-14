# N - Currency Conversion Network

https://atcoder.jp/contests/awc0200/tasks/awc0200_n

Time Limit: 2 sec / Memory Limit: 1024 MiB

Score : $533$ pts



### Problem Statement


There are $N$ cities, and initially, there are no exchange offices between any cities.


We are given $Q$ operations in order. It is guaranteed that at any point in time, the configuration of the exchange offices forms a forest (a graph with no cycles). Therefore, there is exactly one path between any two cities belonging to the same connected component.


Each exchange office is an edge connecting two cities and has a different affine transformation ($t \mapsto at + b$) depending on the direction of traversal. All calculations are performed as integers modulo $998244353$.


Each exchange office has a direction determined when it is opened. The direction of the exchange office opened by the operation L p q a b c d is "$p$-side $\to$ $q$-side", and this direction does not change until the exchange office is closed.


Each operation is one of the following:


- L u v a b c d (Open): Open a new exchange office between city $u$ and city $v$. It is guaranteed that at this moment, $u$ and $v$ belong to different connected components. The direction of this exchange office is defined as "$u$-side $\to$ $v$-side". Exchanging money from the $u$-side to the $v$-side converts the holding money $t$ into $at + b \pmod{998244353}$, and exchanging from the $v$-side to the $u$-side converts $t$ into $ct + d \pmod{998244353}$.


- C u v (Close): Close the exchange office directly connecting city $u$ and city $v$. It is guaranteed that this exchange office exists at this moment. The order of the arguments $u, v$ may differ from when it was opened. Closing it splits $u$ and $v$ into different connected components.


- U u v a b c d (Update): Update the exchange rates of the exchange office directly connecting city $u$ and city $v$. It is guaranteed that this exchange office exists at this moment. The order of the arguments $u, v$ may differ from when it was opened.


If this exchange office was opened by an operation L p q ..., after the update, the exchange from the $p$-side to the $q$-side becomes $t \mapsto at + b \pmod{998244353}$, and the exchange from the $q$-side to the $p$-side becomes $t \mapsto ct + d \pmod{998244353}$.


- Q x y s (Query): If city $x$ and city $y$ belong to the same connected component, output the result of applying the transformations corresponding to the edges $v_0v_1, v_1v_2, \ldots, v_{k-1}v_k$ in order to the initial holding money $s$, modulo $998244353$, along the unique path $x = v_0, v_1, \ldots, v_k = y$ from $x$ to $y$.


If $x = y$, then $k = 0$, and no transformations are applied, so output $s$ as is.


If $x$ and $y$ do not belong to the same connected component, output -1.


### Constraints


- $1 \leq N \leq 5 \times 10^4$


- $1 \leq Q \leq 5 \times 10^4$


- In the operation L u v a b c d, $1 \leq u, v \leq N$, $u \neq v$, $0 \leq a, b, c, d \leq 998244352$


- In the operation C u v, $1 \leq u, v \leq N$, $u \neq v$


- In the operation U u v a b c d, $1 \leq u, v \leq N$, $u \neq v$, $0 \leq a, b, c, d \leq 998244352$


- In the operation Q x y s, $1 \leq x, y \leq N$, $0 \leq s \leq 998244352$


- At any point in time, the configuration of the exchange offices forms a forest.


- All operations are valid (satisfying the guaranteed conditions of each operation described above).


- All input values are integers.



### Input


```text

$N$ $Q$
$\text{op}_1$
$\text{op}_2$
:
$\text{op}_Q$
```


The first line contains the number of cities $N$ and the number of operations $Q$, separated by a space. The subsequent $Q$ lines each contain an operation. Each operation is in one of the following formats:


```text

L $u$ $v$ $a$ $b$ $c$ $d$
C $u$ $v$
U $u$ $v$ $a$ $b$ $c$ $d$
Q $x$ $y$ $s$
```


### Output


For each Q operation, output the required value on a single line. If $x$ and $y$ do not belong to the same connected component, output -1.



### Sample Input 1


```text

4 8
L 1 2 2 3 5 7
Q 1 2 10
L 2 3 4 1 6 0
Q 1 3 10
U 2 3 3 3 2 2
Q 3 1 5
C 1 2
Q 1 3 10
```


### Sample Output 1


```text

23
93
67
-1
```



### Sample Input 2


```text

5 8
Q 1 2 0
L 1 2 1 5 1 6
Q 2 1 10
L 3 4 2 0 3 0
Q 1 4 7
C 2 1
Q 1 2 123
Q 3 4 8
```


### Sample Output 2


```text

-1
16
-1
-1
16
```



### Sample Input 3


```text

8 20
L 1 2 2 1 3 4
L 2 3 5 0 7 2
L 4 5 1 10 1 20
Q 1 3 6
Q 3 1 100
L 3 4 4 4 2 2
Q 1 5 9
U 3 2 6 6 8 8
Q 1 5 9
C 3 4
Q 1 5 9
L 6 7 9 1 10 2
L 7 8 11 3 12 4
Q 6 8 5
L 5 6 13 5 14 6
Q 4 8 7
C 7 8
Q 6 8 5
L 3 8 15 7 16 8
Q 1 8 9
```


### Sample Output 3


```text

65
2110
394
494
-1
509
22388
-1
1807
```



### Sample Input 4


```text

12 35
L 1 2 2 3 4 5
L 2 3 3 4 5 6
L 3 4 4 5 6 7
L 5 6 5 6 7 8
L 6 7 6 7 8 9
L 8 9 7 8 9 10
Q 1 4 11
Q 4 1 22
L 4 5 8 9 10 11
Q 1 7 33
U 4 5 9 10 11 12
Q 7 1 44
C 2 3
Q 1 7 55
L 2 8 10 11 12 13
Q 1 9 66
L 9 10 11 12 13 14
L 10 11 12 13 14 15
Q 11 1 77
C 5 6
Q 3 7 88
L 5 12 13 14 15 16
Q 3 12 99
L 7 11 14 15 16 17
Q 6 1 111
U 1 2 15 16 17 18
U 10 11 16 17 18 19
Q 6 9 222
C 4 5
L 4 12 17 18 19 20
Q 3 5 333
C 8 9
Q 1 6 444
L 8 3 18 19 20 21
Q 1 5 555
```


### Sample Output 4


```text

321
2809
204073
3347809
-1
9535
6144873
-1
47061
742065513
4390335
341221
-1
533386148
```



### Sample Input 5


```text

1 5
Q 1 1 0
Q 1 1 1
Q 1 1 998244352
Q 1 1 123456789
Q 1 1 500000000
```


### Sample Output 5


```text

0
1
998244352
123456789
500000000
```
