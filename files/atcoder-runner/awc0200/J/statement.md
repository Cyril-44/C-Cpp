# J - Moving the Piece

https://atcoder.jp/contests/awc0200/tasks/awc0200_j

Time Limit: 2 sec / Memory Limit: 1024 MiB

Score : $433$ pts



### Problem Statement


Takahashi is playing a game with a piece on a number line. Initially, the piece is placed at coordinate $0$ on the number line, and there is not even $1$ obstacle on the number line.


The following events occur in order for $i = 1, 2, \dots, N$:


- An obstacle is placed at coordinate $X_i$. The placed obstacle remains there indefinitely (if an obstacle is placed at a coordinate that already has an obstacle, the state does not change).


- If the current coordinate of the piece is $X_i$, move the piece to the integer coordinate $Y$ determined by the following conditions. Otherwise, do not move the piece.


- Choose a coordinate where an obstacle is not currently placed, that is, a coordinate satisfying $Y \notin \{X_1, X_2, \dots, X_i\}$.


- Among such coordinates, choose one that minimizes the distance $|Y - X_i|$ to the piece's current coordinate $X_i$.


- Letting $d$ be the minimum distance, if there are $2$ coordinates satisfying the conditions, $X_i - d$ and $X_i + d$, choose the larger one, $X_i + d$.


This always uniquely determines the destination $Y$.


Find the coordinate where the piece is located after all events have finished.


### Constraints


- $1 \leq N \leq 10^5$


- $-10^9 \leq X_i \leq 10^9$ ($1 \leq i \leq N$)


- All input values are integers.



### Input


The input is given from Standard Input in the following format:


```text

$N$
$X_1$
$X_2$
$\vdots$
$X_N$
```


### Output


Print the coordinate where the piece is located after all events have finished in $1$ line.



### Sample Input 1


```text

4
0
1
5
2
```


### Sample Output 1


```text

3
```



### Sample Input 2


```text

2
10
-5
```


### Sample Output 2


```text

0
```



### Sample Input 3


```text

10
0
2
3
1
-2
-1
10
-3
-4
0
```


### Sample Output 3


```text

-5
```



### Sample Input 4


```text

25
1
2
3
4
5
-1
-2
-3
-4
0
-6
-7
-5
1000000000
-1000000000
999999999
-8
-9
-10
500
-500
-11
1
2
-12
```


### Sample Output 4


```text

-13
```



### Sample Input 5


```text

1
0
```


### Sample Output 5


```text

1
```
