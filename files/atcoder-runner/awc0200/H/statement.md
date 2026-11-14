# H - Team Division

https://atcoder.jp/contests/awc0200/tasks/awc0200_h

Time Limit: 2 sec / Memory Limit: 1024 MiB

Score : $400$ pts



### Problem Statement


In a programming contest hosted by Takahashi, $N$ participants will be divided into one or more teams to compete in a team competition. The participants are numbered $1$ to $N$, and the rating of participant $i$ is $A_i$.


Takahashi has decided to divide the participants into teams such that all of the following conditions are satisfied:


- Every participant belongs to exactly one team.


- Each team has at least $K$ participants.


To reduce the disparity in skill within teams, the "imbalance" of a team is defined as the difference between the maximum and minimum ratings among the participants belonging to that team.


Find the minimum possible sum of the imbalances of all teams.


If no division satisfies the conditions, print -1.


### Constraints


- $1 \leq N \leq 2 \times 10^5$


- $1 \leq K \leq 2 \times 10^5$


- $1 \leq A_i \leq 10^9$


- All input values are integers.



### Input


The input is given in the following format:


```text

$N$ $K$
$A_1$ $A_2$ $\dots$ $A_N$
```


- The first line contains an integer $N$ representing the number of participants and an integer $K$ representing the lower bound on the number of participants in each team, separated by a space.


- The second line contains $N$ space-separated integers $A_1, A_2, \dots, A_N$ representing the ratings of the participants.


### Output


Print the minimum possible sum of the imbalances of all teams in a single line.


If no division satisfies the conditions, print -1.



### Sample Input 1


```text

5 2
10 1 4 15 3
```


### Sample Output 1


```text

8
```



### Sample Input 2


```text

3 5
100 200 150
```


### Sample Output 2


```text

-1
```



### Sample Input 3


```text

12 3
25 100 22 102 105 30 200 205 201 28 203 101
```


### Sample Output 3


```text

18
```



### Sample Input 4


```text

30 4
48 12 99 35 14 88 52 91 19 33 85 41 18 95 38 72 50 22 67 93 45 78 61 29 83 55 69 75 63 31
```


### Sample Output 4


```text

64
```



### Sample Input 5


```text

1 1
1000000000
```


### Sample Output 5


```text

0
```
