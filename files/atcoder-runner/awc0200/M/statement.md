# M - Smooth Substring

https://atcoder.jp/contests/awc0200/tasks/awc0200_m

Time Limit: 2 sec / Memory Limit: 1024 MiB

Score : $500$ pts



### Problem Statement


You are given an integer $D$ ($0 \leq D \leq 9$). First, we define a "smooth number" for non-negative integers using $D$.


The standard decimal representation of a non-negative integer $v$ is the decimal representation without leading zeros if $v \geq 1$, and 0 if $v = 0$. A non-negative integer $v$ is a smooth number if, in its standard decimal representation, the absolute difference between any two adjacent digits is at most $D$. Non-negative integers with a 1-digit standard decimal representation ($0$ to $9$) are always smooth numbers because there are no adjacent pairs of digits.


Next, for a positive integer $x$, we define $f(x)$ as follows.


Consider the standard decimal representation of $x$ as a string $S$. Let $L$ be the length of $S$. There are $L(L+1)/2$ contiguous substrings of $S$ (obtained by extracting one or more consecutive characters of $S$), distinguished by their starting and ending positions. For example, if $x = 1023$, then $S = $ 1023, and the $10$ contiguous substrings are 1, 10, 102, 1023, 0, 02, 023, 2, 23, and 3.


For each of these $L(L+1)/2$ contiguous substrings, we determine if it represents a smooth number using the following procedure:


- Interpret the contiguous substring as a non-negative integer in base 10. If there are leading zeros, ignore them to obtain the numerical value (for example, 023 is interpreted as $23$, and 00 as $0$).


- Determine if the resulting non-negative integer is a smooth number. Note that this determination is based on the standard decimal representation of that non-negative integer. That is, it is judged by the differences between adjacent digits in its standard decimal representation after converting to a number, not in the original substring itself.


$f(x)$ is the number of contiguous substrings among the $L(L+1)/2$ ones that are determined to be smooth numbers by the above procedure.


Note: Contiguous substrings extracted from different positions are counted separately, even if they have the same numerical value. For example, if $x = 121$, then from $S = $ 121, the 1 at the 1st character and the 1 at the 3rd character both have the value $1$ (a smooth number), but they are counted separately as $2$ occurrences.


You are given a positive integer $N$ as a string in decimal representation. Find the sum $f(1) + f(2) + \cdots + f(N)$ modulo $10^9 + 7$.


### Constraints


- $N$ is the decimal representation of a positive integer without leading zeros.


- $1 \leq |N| \leq 10^5$


- Here, $|N|$ denotes the number of digits in the decimal representation of $N$.


- $0 \leq D \leq 9$


- $D$ is an integer.



### Input


```text

$N$
$D$
```


- The first line contains a positive integer $N$ as a string in decimal representation.


- The second line contains an integer $D$, the maximum allowed absolute difference between adjacent digits in the condition for smooth numbers.


### Output


Print the sum $f(1) + f(2) + \cdots + f(N)$ modulo $10^9 + 7$ in a single line.



### Sample Input 1


```text

20
1
```


### Sample Output 1


```text

34
```



### Sample Input 2


```text

101
0
```


### Sample Output 2


```text

206
```



### Sample Input 3


```text

3141592653589793238462643383279
2
```


### Sample Output 3


```text

584892474
```



### Sample Input 4


```text

12345678901234567890123456789012345678901234567890123456789012345678901234567890
4
```


### Sample Output 4


```text

940477512
```



### Sample Input 5


```text

1
9
```


### Sample Output 5


```text

1
```
