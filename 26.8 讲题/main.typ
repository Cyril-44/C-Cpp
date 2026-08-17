#import "@preview/touying-simpl-swufe:0.2.1": *


#show: swufe-theme.with(
  aspect-ratio: "16-9",
  lang: "en", // ""en" for English, "zh" for Chinese
  font: ((name: "Palatino Linotype", covers: "latin-in-cjk"), "SimSun-ExtB"),

  // Basic information
  config-info(
    title: [2026.8 讲题],
    subtitle: [DS 专题选讲],
    short-title: [2026.8 讲题],
    authors: [Cyril],
    author: [Presenter: Cyril],
    date: datetime.today(),
    institution: ([安徽师范大学附属中学]),
    banner: ""
  ),

  config-colors(
    primary: rgb(1, 83, 139),
    primary-dark: rgb(0, 42, 70),
    secondary: rgb(255, 255, 255),
    neutral-lightest: rgb(255, 255, 255),
    neutral-darkest: rgb(0, 0, 0)
  ),
)

#title-slide()

#outline-slide()

= CF2182G

== Statement

=== #link("https://www.luogu.com.cn/problem/CF2182G")[CF2182G]

给定一颗 $n$ 个节点的树。

定义一个好的排列为，将排列首尾相接成环后，相邻点的距离不超过 $k$.

计算好的 DFS 序的个数。

$k < n <= 3 times 10^5$.

== Slide I / ii

Slide content.
$
  f_i &=2 \
#pause
  &=3
$

= CF1572F

== Statement



== Slide II / ii

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

= Conclusion
#ending-slide("Thank You!")
