#import "@preview/touying-simpl-swufe:0.2.1": *
#import "@preview/cuti:0.4.0": show-cn-fakebold
#show: show-cn-fakebold
#let show_sol = sys.inputs.at("SOL", default: "false") == "true"

#show: swufe-theme.with(
  aspect-ratio: "16-9",
  lang: "en", // ""en" for English, "zh" for Chinese
  font: ((name: "Latin Modern Roman", covers: "latin-in-cjk"), "SimSun-ExtB"),

  // Basic information
  config-info(
    title: [DS 专题选讲],
    subtitle: [2026.8 讲题],
    short-title: [Data Structures],
    authors: [Cyril],
    author: [Presenter: Cyril],
    date: datetime.today(),
    banner: [
      #image("logo.png")
      #if show_sol {[ Full Presentation ]} else {[ Preview ]}
    ]
  ),

  config-colors(
    primary: rgb("#8b0101"),
    primary-dark: rgb("#460000"),
    secondary: rgb(255, 255, 255),
    neutral-lightest: rgb(255, 255, 255),
    neutral-darkest: rgb(0, 0, 0)
  ),
)

#title-slide()

== Abstract

#if show_sol [
这是一份 DS 选讲题单。可能涵盖了线段树、树链剖分、分块等数据结构。

大部分题目来自于学长 #link("https://codeforces.com/profile/1225Xia")[xez]。

只有 CF2182G 是我自己做出来的题目，用时大约 2.5h，并没有什么参考意义。
]

PPT 上的题意均经过简化，可能需要从原始题意浅层转化得到。

题目描述上方的链接可以直接跳转到原题。

没有提到强制在线的题目默认可以考虑离线。

#outline-slide()

= CF2182G

== Statement

=== #link("https://www.luogu.com.cn/problem/CF2182G")[CF2182G Short Garland]

给定一颗 $n$ 个节点的树，以 1 为根。

定义一个好的排列为，将排列首尾相接成环后，相邻两个数所代表的节点的距离不超过 $k$.

计算好的 DFS 序的个数。

$k < n <= 3 times 10^5$.

#if show_sol [

]


= CF1572F

== Statement

=== #link("https://www.luogu.com.cn/problem/CF1572F")[CF1572F Stations]

现在有 $n$ 座塔，有高度 $h_i$ 和广播范围 $w_i$.

定义塔 $i$ 能向塔 $j$ 发送信息，当且仅当 $j in [i, w_i]$ 且对于 $k in (i, j]$ 都有 $h_k < h_i$，即 $h_i$ 是 $h_(i dots j)$ 中的严格最大值。

定义 $b_i$ 为能接收到塔 $i$ 发送信息的塔的数量。

初始有 $forall i in [1, n], h_i = 0, w_i = i$.

接下来有 $q$ 个事件，两种操作：
+ $(c, g)$ 修改：$h_c <- display(max_(i=1)^n {h_i} + 1)$，$w_c <- g$
+ $(l, r)$ 询问：$limits(sum)_(i=l)^r b_i $

$n, q, g <= 2 times 10^ 5$.

#if show_sol [

== Slide II / ii


]

= LG P8861

== Statement

=== #link("https://www.luogu.com.cn/problem/P8861")[P8861 线段]

有一个初始为空的线段集，你需要处理 $q$ 组询问，每组询问的格式为如下三种之一：

+ 加入一条新线段 $[l,r]$。
+ 将线段集里所有与 $[l,r]$ 相交的线段修改为其与 $[l,r]$ 的交。
+ 求出线段集里所有与 $[l,r]$ 相交的线段与 $[l,r]$ 的交的长度和。

一条线段 $[a,b]$ 的长度为 $b-a$. 在本题中，线段可能退化为单点。

#strong[注意：你需要在线地处理每一组询问。]

记 $q_1, q_2, q_3$ 表示三种询问的数量。\
有 $n <= 2 times 10^5, q_1, q_2 <= 10^5, q_3 <= 3 times 10^5$.

= LG P15397

== Statement

=== #link("https://www.luogu.com.cn/problem/P15397")[P15397 浙江旅行团 / hangzhou]

定义本题中的矩阵为 $(min, +)$ $2 times 2$ 矩阵，满足运算 $(A times B)_(i k) = min(A_(i j) + B_(j k))$. \
单位元 $epsilon = mat(0, 10^9; 10^9, 0)$. #h(1em) 定义 $xor$ 表示按位异或。

你需要维护 $m$ 个 bot，版本 0 时bot $i$ 在位置 $a_i$，矩阵 $M_i = epsilon$，评分 $r_i = 0$.

结算 bot $i$：$r_i <- r_i xor ("A" xor M_i_(00) + "B" xor M_i_(01) + "C" xor M_i_(10) + "D" xor M_i_(11)), M_i <- epsilon $. 

一共有 $q$ 组询问，三种操作：\ 
#strong[第 $i$ 组询问建立在版本 $t_i$ 之上，并成立版本 $i$。即需要可持久化。]
+ $(l, r, c)$ 修改：使 bot $l~r$ 结算，然后区间赋值 $a_(l dots r) <- c$.
+ $(c, V)$ 修改：令所有位置在 $c$ 的机器人的矩阵 $M_i <- M_i times V$.
+ $(x)$ 查询：对 bot $x$ 结算之后，求其评分 $r_i$.

#strong[强制在线。]$n, m, q <= 2 times 10^5, 1 <= a_i, c <= n, 0 <= "A","B","C","D" < 2^32, 0 <= V_(i j) < 2^8$. 

= LG P12485

== Statement

=== #link("https://www.luogu.com.cn/problem/P12485")[P12485 [集训队互测 2024] PM 大师]

对于可重集 $S$ 定义 $"mex"(S)$ 表示最小的 #strong[正整数] $x$ 满足 $x in.not S$.

定义 $f(a) -> b$，要求 $-1<=a_i<=n$： 
$display(b_i=cases(
  a_i & a_i != 0,
  limits("mex")_(j=1)^(i-1){b_j} #h(1em) &a_i=0
))$

现在给定长度为 $n$ 的数组 $a$，#strong[保证初始时 $a_i in {-1,0}$ 且数组 $a$ 不全为 $0$.]

$q$ 次操作 $(x,k,y)$ 表示先将 $a_x <- k$，然后求出使用 $a$ 生成的数组 $b$ 中 $b_y$ 的值。

#strong[保证任意时刻为 $0$ 的 $a_i$ 不会被修改，不为 $0$ 的 $a_i$ 不会被修改为 $0$.]

$n, q <= 10^6$.


= LG P8987

== Statement

=== #link("https://www.luogu.com.cn/problem/P8987")[P8987 [北大集训 2021] 简单数据结构]

你有一个长度为 $n$ 的序列 $a$，下面你要进行 $q$ 次修改或询问。

1. 给定 $v$，将#strong[所有] $a_i$ 变为 $min{a_i, v}$。
2. 将#strong[所有] $a_i$ 变为 $a_i + i$。
3. 给定 $l, r$，询问 $limits(sum)_(i=l)^r a_i$。

$n,q <= 2 times 10^5, a_i, v <= 10^12$

= CF1685E

== Statement

=== #link("https://www.luogu.com.cn/problem/CF1685E")[CF1685E The Ultimate LIS Problem]

给定一个由数字 $1$ 到 $2n+1$ 组成的排列 $p$.

你需要处理 $q$ 次操作 $(u,v)$，每次操作将交换 $p_(u)$ 和 $p_(v)$.

每次操作后，找出 $p$ 的任意一个循环移位 $p_k, p_(k+1), dots, p_(2n+1), p_1, p_2, dots, p_(k-1)$，使得该移位后的排列的 $"LIS"$ 长度不超过 $n$，输出 $k$ 或者判断不存在这样的移位。

这里 $"LIS"(a)$ 表示序列 $a$ 的最长严格递增子序列的长度。

$2<=n<=10^5, q<=10^5$.

#if show_sol [
#figure(
  table(
    columns: (1fr, 1fr, 1fr),
    stroke: none,
    align: center + horizon,
    inset: .5em,
    table.hline(stroke: 2pt),
    [Name], [Age], [Major],
    table.hline(stroke: 1pt),
    [Zhang San], [23], [Finance],
    [Li Si], [22], [Economics],
    [Wang Wu], [24], [Accounting],
    table.hline(stroke: 2pt),
  ),
  caption: "Example Table",
)
]

#ending-slide("Thank You!")
