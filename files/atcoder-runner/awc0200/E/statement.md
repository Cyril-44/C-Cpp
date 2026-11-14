# E - Project Assignment

https://atcoder.jp/contests/awc0200/tasks/awc0200_e

Time Limit: 2 sec / Memory Limit: 1024 MiB

Score : $333$ pts



### Problem Statement


Takahashi is a department manager at a company. There are $N$ employees in his department, and the $i$-th employee $(1 \leq i \leq N)$ has a skill point value of $S_i$.


One day, $M$ projects are launched simultaneously. The $j$-th project $(1 \leq j \leq M)$ has a priority $P_j$ and requires exactly $R_j$ employees to carry it out. Takahashi wants to assign exactly the required number of employees to every project. However, each employee can be assigned to at most one project, and it is acceptable for some employees to not be assigned to any project.


According to company policy, assignments are made sequentially following these rules:


- First, sort the $M$ projects in decreasing order of priority value. If multiple projects have the same priority value, the one with the smaller index ($j$ value) comes first.


- Following this order, assign employees to one project at a time, starting from the first project.


- When assigning employees to a project, if the number of employees not yet assigned to any project is less than the required number $R_j$ for that project, the entire assignment fails and processing is terminated (the results of all previously assigned projects are also invalidated).


- If there are enough employees, select the $R_j$ employees with the highest skill points from those not yet assigned to any project. If multiple employees have equal skill points, any of them may be chosen.


Note: Regardless of how ties among employees with equal skill points are broken, the total skill points assigned to each project is uniquely determined. This is because no matter which employees with the same skill points are chosen for the current project, the remaining set of employees (viewed as a multiset of skill points) remains unchanged.


Following the rules above, determine whether it is possible to assign the required number of employees to all projects. If it is possible, find the total skill points of the employees assigned to each project.


### Constraints


- $1 \leq N \leq 2 \times 10^5$


- $1 \leq M \leq 2 \times 10^5$


- $1 \leq S_i \leq 10^9$ $(1 \leq i \leq N)$


- $1 \leq P_j \leq 10^9$ $(1 \leq j \leq M)$


- $1 \leq R_j \leq N$ $(1 \leq j \leq M)$


- All input values are integers.


Note: It is NOT guaranteed that $R_1 + R_2 + \cdots + R_M \leq N$. If the total number of required employees exceeds the number of employees, the assignment may fail partway through.



### Input


```text

$N$ $M$
$S_1$ $S_2$ $\ldots$ $S_N$
$P_1$ $R_1$
$P_2$ $R_2$
$\vdots$
$P_M$ $R_M$
```


- The first line contains the number of employees $N$ and the number of projects $M$, separated by a space.


- The second line contains the skill points $S_1, S_2, \ldots, S_N$ of each employee, separated by spaces. $S_i$ is the skill point of the $i$-th employee.


- The following $M$ lines contain information about each project.


- The $j$-th of these lines $(1 \leq j \leq M)$, which is the $(2 + j)$-th line of the entire input, contains the priority $P_j$ and the required number of employees $R_j$ for the $j$-th project, separated by a space.


### Output


If the assignment fails following the rules above, output $-1$ on a single line.


If the assignment to all projects succeeds, output $M$ lines. The $j$-th line $(1 \leq j \leq M)$ should contain the total skill points of the employees assigned to the $j$-th project.



### Sample Input 1


```text

5 2
10 20 30 40 50
5 2
10 2
```


### Sample Output 1


```text

50
90
```



### Sample Input 2


```text

3 2
1 2 3
1 2
2 2
```


### Sample Output 2


```text

-1
```



### Sample Input 3


```text

10 3
5 15 25 35 45 55 65 75 85 95
3 3
7 2
7 4
```


### Sample Output 3


```text

75
180
240
```



### Sample Input 4


```text

15 4
100 200 300 400 500 600 700 800 900 1000 1100 1200 1300 1400 1500
1 2
5 3
5 4
10 3
```


### Sample Output 4


```text

900
3300
3000
4200
```



### Sample Input 5


```text

1 1
1000000000
1 1
```


### Sample Output 5


```text

1000000000
```
