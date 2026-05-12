#include <cstdio>
#include <cmath>
#include <cstring>
constexpr int N = 500005;
#define For(i, s, t) for (int i = (s); i <= (t); ++i)
#define roF(i, t, s) for (int i = (t); i >= (s); --i)
int f[N], g[N], pw2[N], M; // f[i] 表示可以表示区间 [1,i] 且选中的数和为 i 的方案数（即刚好选不了 i+1 的方案数）
inline void add(int &x, int y) { if ((x += y) >= M) x -= M; }
inline void sub(int &x, int y) { if ((x -= y) < 0) x += M; }
inline int sqrt2(int n) {
    int res = sqrt(2*n);
    while (res * (res+1) / 2 > n) --res;
    return res;
}
void solve(int n) {
    if (n <= 1) return;
    solve(n >> 1);
    /* 考虑最终的 f 通过容斥去计算。合法的方案数肯定就是
        f[i] = 互异拆分数 - sum j=1 to i-2 (f[j] * (有元素[j+2,i]，做01背包求容量为 i-j 的方案数))
       显然这个乘法实在是太难算了，所以我们把这个 f[j] 的系数放到物品的价值上去，01背包求价值和。很常见的拆贡献 Trick。
       更进一步的，我们可以将 g[j] 设置为 f[j]，然后 k=j+2 to i, l=n downTo k: g[l] += g[l-k]; 这样就能直接使用 g[i] 作为式子里面的第二项了。
       感觉还是不好算，仍然做出来 Ferrer 图像
       +++++|++++ 9  易于发现前面是有一坨的，这一坨就是 (j+1)*s，其中 s 是背包里的物品数量（s \in [0,+oo)）。
       +++++|++   7  根据这个发现，我们只需要将 DP 转置，然后将原本的 g[j] 设置为 f[j] 改成 g[j + (j+1)*s] 都设置为 f[j]，然后从 1~mx 做完全背包就可以了。
       +++++|+    6
       333333211

    */
    memset(g, 0, sizeof(int) * (n+1));
    roF (i, sqrt2(n), 1) {
        roF (j, n, i) g[j] = g[j - i];
        roF (j, i-1, 0) g[j] = 0;
        for (int j = 0; j + (j+2)*i <= n; j++)
            add(g[j + (j+2)*i], f[j]);
        For (j, i, n) add(g[j], g[j - i]);
    }
    For (i, n/2 + 1, n) sub(f[i], g[i]);
}
int main() {
    int n;
    scanf("%d%d", &n, &M);
    pw2[0] = 1;
    For (i, 1, n) add(pw2[i] = pw2[i-1], pw2[i-1]);
    /* 先使用 Ferrer 图像将 DP 转置。
       +++++ 5  一行的+ 表示选择的数字大小，比如这里选择了 {5,4,3,2} sum=14
       ++++  4  如果一列一列看，
       +++   3  发现就是一个钦定最大值后，
       ++    2  从 1~最大值 都 **必须至少选择一个** 的完全背包。
       44321 */
    f[0] = 1; // 目前的 f 就是互异拆分数
    roF (i, sqrt2(n), 1) {
        roF (j, n, i + 1) f[j] = f[j - i]; // 先做一遍 01背包 保证选。
        roF (j, i, 1) f[j] = 0;
        For (j, i, n) add(f[j], f[j - i]);
    }
    solve(n);
    int sum = 0;
    For (i, 0, n-1)
        add(sum, 1l * f[i] * pw2[n-i-1] % M);
    int ans = pw2[n];
    sub(ans, sum);
    printf("%d\n", ans);
    /* f[1] = f[3] = 1; // f[i] 表示 可以表示区间 [1,i] 的方案数
    for (int i = 3; i <= n; i++)
        for (int k = n; k >= i - 1; k--)
            add(f[std::min(i + k, n)], f[k]);
    printf("%d\n", f[n]); */
    return 0;
}