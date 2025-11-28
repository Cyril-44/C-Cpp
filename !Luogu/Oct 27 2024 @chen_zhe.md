@chen_zhe 
其实这题真的不需要这些特判、二分搞整数

$$
\begin{aligned}
  \text{range} = [\dfrac{{v}^2 - {v_0}^2}{2a} + 10^{-4}, L]
  &\gets 
  \begin{cases}
    a>0 \\
    v_0 \le v
  \end{cases} \\
  \text{range} = [d, \dfrac{{v}^2 - {v_0}^2}{2a} - 10^{-4}]
  &\gets 
  \begin{cases}
    a<0 \\
    v_0 \ge v
  \end{cases}
\end{aligned}
$$

还有一些整数的情况就不列举了
查询的时候直接查询 `lower_bound((int)ceil(l))` 和 `upper_bound((int)floor(r))` 就行了

~~考场只想到这种卡精度的方法~~

那么为啥可行呢？

显然 因为 $ |a| < 10^3 $ 我们算出来的位移 $d$ 精度就是 $ \dfrac{1}{2|a|} \ge \dfrac{1}{2 \cdot 10^{-3}} $ 而我加上的小位移是 $\dfrac{1}{10^{-4}}$ 所以是完全可以的！

~~哈哈哈哈我真是个天才~~