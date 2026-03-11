#include <cstdio>
#include <cstring>
#include <algorithm>
constexpr int N = 5005, MOD = 998244353;
int pw2[N], C[N][N], Cs[N][N];
struct Node {
    int price, id;
    explicit operator int() const { return price; }
} a[N];
inline bool operator< (const Node &x, const Node &y) {
    return (int)x > (int)y || int(x) == int(y) && x.id < y.id;
}
inline void add(int &x, int y) { if ((x += y) >= MOD) x -= MOD; }
inline int Csum(int n, int l, int r) { // returns (n, l) + (n, l+1) + ... + (n, r)
    if (r > n) r = n;
    if (r < 0 || r < l) return 0;
    int res = Cs[n][r];
    if (l > 0) ((res -= Cs[n][l-1]) < 0) && (res += MOD);
    return res;
}
int main() {
    pw2[0] = 1;
    int Tid, T, n = 5000, m;
    scanf("%d%d", &Tid, &T);
    for (int i = 1; i <= n; i++) add(pw2[i] = pw2[i-1], pw2[i-1]);
    for (int i = 0; i <= n; i++) {
        C[i][0] = C[i][i] = 1;
        for (int j = 1; j < i; j++)
            add(C[i][j] = C[i-1][j-1], C[i-1][j]);
    }
    memcpy(Cs, C, sizeof C);
    for (int i = 0; i <= n; i++)
        for (int j = 1; j <= i; j++)
            add(Cs[i][j], Cs[i][j-1]);
    while (T--) {
        scanf("%d%d", &n, &m);
        for (int i = 1; i <= n; i++)
            scanf("%d", &a[i].price), a[i].id = i;
        std::sort(a+1, a+1+n);
        int ans = 0; // 不合法的方案数
        for (int i = 2; i < n; i++) {
            int L = std::upper_bound(a+1, a+1+n, 2 * int(a[i]), [](int x, const Node&y){return x > int(y);}) - a;
            int R = L; // L --> 第一个 a[pos] < 2*a[i]，R --> 第一个 a[pos] <= a[i]+a[j]，L~R-1 就是 2*a[i] > a[pos] > a[i]+a[j]
            for (int j = i+1; j <= n; j++) {
                while (R <= n && int(a[R]) > int(a[i]) + int(a[j])) ++R;
                // add(ans, 1ull * Csum(i - 2, i + L - m - 1, i + R - 1 - m - 1) * pw2[n - j] % MOD);
                for (int k = L; k < R; k++) {
                    if (i+k-m-1>=0) add(ans, 1ull* C[i - 2][i + k - m - 1] * pw2[n - j] % MOD);
                    // for (int x = 0; x <= i - k - 1; x++)
                    //     if (m - i + x >= 0) add(ans, 1ull * C[i - k - 1][x] * C[k - 1][m - i + x] % MOD * pw2[n - j] % MOD);
                }
            }
        }
        for (int i = 2; i <= n; i++) {
            int L = std::upper_bound(a+1, a+1+n, 2 * int(a[i]), [](int x, const Node &y){return x > int(y);}) - a;
            int R = std::lower_bound(a+1, a+1+n, int(a[i]), [](const Node&x, int y){return int(x) > y;}) - a;
            // add(ans, Csum(i - 2, i + L - m - 1, i + R - 1 - m - 1));
            for (int k = L; k < R; k++) {
                if (i+k-m-1>=0) add(ans, C[i - 2][i + k - m - 1]);
                // for (int x = 0; x <= i - k - 1; x++)
                //     if (m - i + x >= 0) add(ans, 1ull * C[i - k - 1][x] * C[k - 1][m - i + x] % MOD);
            }
        }
        printf("%d\n", (pw2[n] - ans + MOD) % MOD);
    }
    return 0;
}
/*
O(N^4logN) 解法：先将 a 从大到小排序
考虑**按照性价比排序后**有这样的结构 ai(1)...(2)ak(2)....(2)aj(1)
那么就是 ai > ak/2，于是我们原本需要的 ak 被 ai 取了一个 1 之后，剩余的钱不足以购买 ak，导致必须购买性价比其次的，w=1 的 aj。所以又有 ak > ai + aj
  结合一下就是 2ai > ak > ai + aj
之后考虑在原来的 a 数组中这样的结构： 2ai ..[M].. ak(2) ..[N].. ai+aj ..[O].. ai(1) ..[P].. ak/2 ..[Q].. aj(1)
那么我们知道**性价比**中 ai 和 ak/2 之间不能再有别的w=1物品，否则结构就不是唯一的，会算重复。
  那么放到原数组中也就是 P 段中的都要设置 w=2，且有保证 ai >= ap >= ak/2 --> ak/2 >= ap/2 即不存在 w=2 的时候还有不满足的情况
现在考虑原数组中 Q 段的物品，这一部分直接 w=2 即可，即使性价比保留在 ak/2 ~ aj 中也没有关系，因为只剩一元，会被卡住直到 aj
现在考虑原数组中 M 的部分，这一部分设置 w=1/2，因为如果设置为 2 那性价比为 am/2 属于 [ak/2, ai]，也就是相当于 性价比 ai~ak/2 的物品的合力共同卡掉了 ak
现在考虑原数组中 NO 的部分，这一部分设置 w=1/2 都可以，因为从性价比考虑，实际上 w=1 会在 ai 之前取，w=2 会在 ak/2 之后被 ak 卡住。
于是我们知道了，如果已经钦定了符合要求的 ai, aj, ak（满足 2ai > ak > ai+aj, ai >= aj），
实际上每一段的要求是固定下来的，除了 NO 段。
考虑 NO 段怎么做。
可以枚举 w=2 的数量，设其为 x（x + y = Size）
那么现在，在原数组中就是 ....(1/2) ..[M]..(1/2) ak(2) ..[N]..(1 *y, 2 *x) ..[O]..(1) ai(1) ..[P]..(2) ..[Q]..(2) aj(1) ....(1/2)
那么实际上我们在取完 ai 之后就只剩 1 元了，一共取了 (i - x) 个数，前面有 k - 1 个自由选择的位置，目前前面还需要 m-i+x 个 w=2 的位置保证在 ak 前卡到只剩 1r。
  于是得到 C(Size,x) * C(k-1,m-i+x)
考虑我们实际上漏算了最后还剩 1r 的情况，也就是 ai(1)ak(2)....(2)
在原数组中：....(1/2) 2ai ..[M]..(1/2) ak(2) ..[N]..(1 *y, 2 *x) ai(1) ..[P]..(2) ak/2 ....(2)
那也可以将 N 段进行 同理枚举。

优化：
  1.发现 k 的取值范围（2ai ~ ai+aj）是一个滑动窗口，log 消掉。
  2.发现上面的式子是 \sum_{x=0}^{i-k-1} C(i-k-1,x) * C(k-1,m-i+x) = C(i-k-1, i-k-1-x) * C(k-1, m-i+x)。
    这两个组合数如果有贡献，要求 x 的区间应该是 [0, i-k+1] \cap [i-m, k+i-m-1]。
    而现在证明，将枚举区间变为 0 ~ k+i-m-1 对于答案没有影响。证明我也不会。
    好的，化成范德蒙德卷积。n消除一个。
  3.发现 k 的求和很有意思，C(i-2, ....) 一直没有变过，所以前缀和优化，n消除一个。
至此时间复杂度 O(N^2) 足以通过本题。
*/