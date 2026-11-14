# D - Outstanding Score

https://atcoder.jp/contests/awc0200/tasks/awc0200_d

Time Limit: 2 sec / Memory Limit: 1024 MiB

Score : $300$ pts



### Problem Statement


In Takahashi's class, $N$ students took a test. The test score of each student $i$ ($1 \leq i \leq N$) is $A_i$.


The teacher wants to recognize students who achieved particularly excellent results compared to the other students. Specifically, given a positive integer $K$, a student is called "outstanding" if their score is at least $K$ times the average score of all other students.


More precisely, let $S = A_1 + A_2 + \cdots + A_N$. Student $i$ is "outstanding" if and only if:


$A_i \times (N - 1) \geq K \times (S - A_i)$


Here, $S - A_i$ is the total score of the $N - 1$ students other than student $i$. When $N \geq 2$, dividing both sides of this inequality by $N - 1$ gives the equivalent condition "$A_i$ is at least $K$ times the average score of the other students." When $N = 1$, both sides become $0$, so the sole student always satisfies this condition regardless of their score or the value of $K$.


Find the number of "outstanding" students.


### Constraints


- $1 \leq N \leq 2 \times 10^5$


- $1 \leq K \leq 10^9$


- $0 \leq A_i \leq 10^9$ ($1 \leq i \leq N$)


- All input values are integers.



### Input


```text

$N$ $K$
$A_1$ $A_2$ $\ldots$ $A_N$
```


- The first line contains an integer $N$ representing the number of students and a positive integer $K$ representing the multiplier criterion for recognition, separated by a space.


- The second line contains $N$ integers $A_1, A_2, \ldots, A_N$ representing each student's score, separated by spaces.


### Output


Print the number of "outstanding" students in a single line.



### Sample Input 1


```text

5 2
10 2 3 1 4
```


### Sample Output 1


```text

1
```



### Sample Input 2


```text

4 3
5 5 5 5
```


### Sample Output 2


```text

0
```



### Sample Input 3


```text

10 5
100 3 2 1 4 2 3 1 2 2
```


### Sample Output 3


```text

1
```



### Sample Input 4


```text

20 2
50 50 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1
```


### Sample Output 4


```text

2
```



### Sample Input 5


```text

1 1000000000
0
```


### Sample Output 5


```text

1
```
