# 📘 题目：异或

## 🧾 题目描述
小 $\delta$ 喜欢异或。因此，他想到了这样一个问题：给定非负整数 $l,r$，满足 $l \le r$。你需要告诉他，对于所有 $l \le l' \le r' \le r$，$\bigoplus\limits_{i=l'}^{r'}i$ 的结果的种类数。因为他真的很喜欢异或，所以他会问你很多次。

## 📥 输入格式
第一行，一个正整数 $T$，代表数据组数。

接下来 $T$ 行，每行两个非负整数 $l,r$。

## 📤 输出格式
$T$ 行，每行一个非负整数，代表问题的答案。

## 📌 约束条件
对于所有测试数据保证：$0 \le l \le r \le 10^{18}$，$1 \le T \le 10^3$。


​	


<table class="table table-bordered"><thead><tr><th rowspan="1">测试点</th><th rowspan="1">$l$</th><th rowspan="1">$r$</th><th rowspan="1">$T$</th><th rowspan="1">特殊性质</th></tr></thead><tbody><tr><td rowspan="1">$1 \sim 3$</td><td rowspan="2">$\le r$</td><td rowspan="1">$\le 200$</td><td rowspan="1">$\le 20$</td><td rowspan="2">否</td></tr><tr><td rowspan="1">$4 \sim 8$</td><td rowspan="1">$\le 10^{3}$</td><td rowspan="1">$\le 50$</td></tr><tr><td rowspan="1">$9 \sim 11$</td><td rowspan="2">$=0$</td><td rowspan="3">$\le 10^{18}$</td><td rowspan="3">$\le 10^{3}$</td><td rowspan="1">是</td></tr><tr><td rowspan="1">$12 \sim 16$</td><td rowspan="2">否</td></tr><tr><td rowspan="1">$17 \sim 20$</td><td rowspan="1">$\le r$</td></tr></tbody></table> 

特殊性质：保证存在 $k$，使得 $r=2^k-1$。

## 🧪 样例
### 样例 1
**输入：**
```
7
0 3
0 7
0 1000
1 1
3 10
197 251
123456789 234567890

```
**输出：**
```
4
8
1013
1
14
89
189773281

```


