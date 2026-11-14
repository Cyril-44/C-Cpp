# C - Arrow Navigation

https://atcoder.jp/contests/awc0200/tasks/awc0200_c

Time Limit: 2 sec / Memory Limit: 1024 MiB

Score : $266$ pts



### Problem Statement


Takahashi is exploring a town consisting of cells arranged in a grid with $H$ rows and $W$ columns. The cell at the $i$-th row from the top and the $j$-th column from the left is denoted as $(i, j)$. Row numbers increase from north to south, and column numbers increase from west to east.


Each cell has a directional signpost displaying either a south-pointing arrow v or an east-pointing arrow >. The signposts for the cells in the $i$-th row are represented by the string $S_i$, where the $j$-th character of $S_i$ corresponds to the signpost at cell $(i, j)$.


Takahashi starts at cell $(1, 1)$ in the northwest corner and repeatedly moves according to the following rules:


- If he is currently at cell $(i, j)$ and the signpost on that cell is v (south-pointing), he attempts to move to cell $(i+1, j)$.


- If he is currently at cell $(i, j)$ and the signpost on that cell is > (east-pointing), he attempts to move to cell $(i, j+1)$.


If the destination is outside the grid, Takahashi does not make that move and stops at his current cell. Specifically, if he is at cell $(i, j)$ and attempts to move south but $i + 1 > H$, or attempts to move east but $j + 1 > W$, Takahashi stops.


Since each move is always exactly one cell to the south or east, Takahashi can move at most $H - 1$ times southward and at most $W - 1$ times eastward from $(1, 1)$, so he will always stop within $H + W - 2$ moves from the start.


Determine the coordinates of the cell where Takahashi stops.


### Constraints


- $1 \leq H \leq 500$


- $1 \leq W \leq 500$


- $H, W$ are integers


- $S_i$ is a string of length $W$ consisting only of v and >



### Input


```text

$H$ $W$
$S_1$
$S_2$
$\vdots$
$S_H$
```


- The first line contains an integer $H$ representing the number of rows and an integer $W$ representing the number of columns of the grid, separated by a space.


- The following $H$ lines provide the information for each row of the grid as a string. The information for the $i$-th row $(1 \leq i \leq H)$ is given on the $(i + 1)$-th line of the input as a string $S_i$ of length $W$. The $j$-th character of $S_i$ represents the signpost displayed at cell $(i, j)$, which is either v (south-pointing) or > (east-pointing).


### Output


Output the row number and column number of the cell where Takahashi stops, separated by a space, on a single line. That is, if the cell where he stops is $(r, c)$, output $r$ and $c$ separated by a space.



### Sample Input 1


```text

3 3
v>v
vv>
>>>
```


### Sample Output 1


```text

3 3
```



### Sample Input 2


```text

3 4
>>>v
vv>v
>v>>
```


### Sample Output 2


```text

3 4
```



### Sample Input 3


```text

5 6
v>>v>>
>v>>v>
v>v>>v
>>vv>>
vv>>v>
```


### Sample Output 3


```text

5 5
```



### Sample Input 4


```text

8 10
>>v>>>v>>>
v>v>>v>v>>
>>v>v>>>v>
v>>>v>v>>v
>v>>v>>>v>
v>v>>>v>>v
>>v>v>>v>>
v>>>v>>>>>
```


### Sample Output 4


```text

8 10
```



### Sample Input 5


```text

1 1
v
```


### Sample Output 5


```text

1 1
```
