# F - Dungeon Conquest

https://atcoder.jp/contests/awc0200/tasks/awc0200_f

Time Limit: 2 sec / Memory Limit: 1024 MiB

Score : $366$ pts



### Problem Statement


Takahashi is exploring a dungeon in a game.


Takahashi has a maximum health of $M$. At the entrance of the dungeon (initial state), his health is $M$, and he has $0$ coins.


The dungeon has $N$ rooms numbered $1$ to $N$, and $N - 1$ rest areas numbered $1$ to $N - 1$.


Takahashi starts from Room $1$ and alternates between rooms and rest areas in the following order, aiming to clear Room $N$:


$\text{Room } 1 \to \text{Rest Area } 1 \to \text{Room } 2 \to \text{Rest Area } 2 \to \dots \to \text{Rest Area } N-1 \to \text{Room } N$


The events at each room and rest area are as follows:


- Clearing Room $i$ ($1 \le i \le N$):


Clearing Room $i$ consumes $A_i$ health. If his health immediately before clearing the room is strictly less than $A_i$, he cannot clear the room and the exploration fails (having exactly $0$ health immediately after clearing is allowed).


- Action at Rest Area $i$ ($1 \le i \le N - 1$):


Takahashi must choose and perform exactly one of the following two actions:


- Drink water from the spring: Restores $B_i$ health. However, if the health after recovery exceeds the maximum value $M$, it becomes $M$.


- Open the treasure chest: Obtains $C_i$ coins. Health does not change.


Find the maximum total number of coins Takahashi can ultimately obtain when he chooses his actions appropriately and successfully clears up to Room $N$.


It is guaranteed that there is at least one way to choose actions to clear up to Room $N$.


### Constraints


- $2 \le N \le 3000$


- $1 \le M \le 3000$


- $1 \le A_i \le M$ ($1 \le i \le N$)


- $1 \le B_i \le M$ ($1 \le i \le N - 1$)


- $1 \le C_i \le 10^9$ ($1 \le i \le N - 1$)


- There exists at least one way to choose actions to clear up to Room $N$.


- All input values are integers.



### Input


The input is given from Standard Input in the following format:


```text

$N$ $M$
$A_1$ $A_2$ $\dots$ $A_N$
$B_1$ $C_1$
$B_2$ $C_2$
$\vdots$
$B_{N-1}$ $C_{N-1}$
```


- The first line contains two space-separated integers $N$ and $M$, representing the number of rooms and the maximum health, respectively.


- The second line contains $N$ space-separated integers $A_1, A_2, \dots, A_N$, representing the health consumed to clear each room.


- The $i$-th of the following $N - 1$ lines ($1 \le i \le N - 1$) contains two space-separated integers $B_i$ and $C_i$, representing the amount of health restored and the number of coins obtained at Rest Area $i$, respectively.


### Output


Print the maximum total number of coins Takahashi can obtain in a single line.



### Sample Input 1


```text

4 10
4 5 4 3
5 10
6 30
4 15
```


### Sample Output 1


```text

30
```



### Sample Input 2


```text

3 10
2 2 2
1 50
1 70
```


### Sample Output 2


```text

120
```



### Sample Input 3


```text

8 15
7 8 4 10 5 6 9 3
8 50
6 120
10 30
5 200
7 80
9 150
4 90
```


### Sample Output 3


```text

210
```



### Sample Input 4


```text

20 50
25 30 15 40 20 35 10 25 30 20 45 15 30 25 35 20 10 40 30 15
20 1000000000
35 850000000
15 500000000
40 999999999
30 750000000
25 300000000
45 900000000
20 600000000
35 450000000
50 1000000000
15 200000000
30 800000000
40 650000000
25 400000000
35 950000000
20 350000000
45 880000000
30 550000000
50 1000000000
```


### Sample Output 4


```text

650000000
```



### Sample Input 5


```text

2 1
1 1
1 1
```


### Sample Output 5


```text

0
```
