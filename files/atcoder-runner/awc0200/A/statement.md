# A - Striped Cloth

https://atcoder.jp/contests/awc0200/tasks/awc0200_a

Time Limit: 2 sec / Memory Limit: 1024 MiB

Score : $200$ pts



### Problem Statement


Takahashi is dyeing a cloth consisting of $N$ cells arranged in a horizontal row in a striped pattern.


From left to right, he paints the first $K$ cells red, the next $K$ cells blue, the next $K$ cells red, and so on, alternating between red and blue every $K$ cells. If $N$ is not a multiple of $K$, the last stripe will be shorter than $K$ cells.


Print the resulting cloth as a string of length $N$, from left to right, using R for red cells and B for blue cells.


### Constraints


- $1 \leq N \leq 100$


- $1 \leq K \leq 100$


- All input values are integers.



### Input


The input is given from Standard Input in the following format:


```text

$N$ $K$
```


- The first line contains the number of cells $N$ and the stripe width $K$, separated by a space.


### Output


Print one line containing the string of length $N$ representing the dyed cloth.



### Sample Input 1


```text

6 2
```


### Sample Output 1


```text

RRBBRR
```



### Sample Input 2


```text

6 3
```


### Sample Output 2


```text

RRRBBB
```



### Sample Input 3


```text

4 1
```


### Sample Output 3


```text

RBRB
```



### Sample Input 4


```text

99 1
```


### Sample Output 4


```text

RBRBRBRBRBRBRBRBRBRBRBRBRBRBRBRBRBRBRBRBRBRBRBRBRBRBRBRBRBRBRBRBRBRBRBRBRBRBRBRBRBRBRBRBRBRBRBRBRBR
```



### Sample Input 5


```text

6 1
```


### Sample Output 5


```text

RBRBRB
```
