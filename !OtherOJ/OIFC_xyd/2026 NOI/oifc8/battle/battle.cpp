#include <cstdio>
#include <cstring>
#include <algorithm>
constexpr int N = 500005;
int64_t sum[N], a2[N];
int a[N], pre[2][N], suf[2][N], n;
// f[i] = 第一个 a 前面 / 后面(rev=1) 的，且 a[pos]>=a[i]（或 a[pos]>a[i] 当 gt=1）的 pos
inline void monoget(int f[], bool gt, bool rev) {
    if (rev) std::reverse(a+1, a+1+n);
    static int sta[N]; int top = 0;
    for (int i = 1; i <= n; sta[++top] = i++) {
        while (top && a[sta[top]] < a[i] + gt) --top;
        f[i] = sta[top];
    }
    if (rev) std::reverse(f+1, f+1+n), std::reverse(a+1, a+1+n), std::transform(f+1, f+1+n, f+1, [](int x){ return n+1-x; });
}
inline void rangeAdd(int l, int r, int64_t x) { sum[l] += x, sum[r+1] -= x; }
inline void solve() {
    int k;
    scanf("%d%d", &n, &k);
    for (int i = 1; i <= n; i++) scanf("%d", &a[i]), a2[i] = 1ll * a[i] * a[i];
    a[0] = a[n+1] = 1e9; memset(sum, 0, sizeof(int64_t) * (n+1));
    monoget(pre[0], 0, 0), monoget(pre[1], 1, 0);
    monoget(suf[0], 0, 1), monoget(suf[1], 1, 1);
    for (int i = 1; i <= n; i++) {
        { // 处理所有 L<=R 的 (m, len)
            int l = pre[1][i], j = suf[0][i], r = j;
            while (a[i] + k >= a[r]) r = suf[1][r];
            int _l = std::max(i, (l+j)/2), _r = std::min(j, (i+r)/2) - 1;
            for (int m = _l; m <= _r; m++)
                rangeAdd(std::max(j-m, m-i+1), std::min(m-l, r-m-1), a2[m]);
        } // l < m-len+1 <= i <= m < j <= m+len < r
        { // 处理所有 L>R 的 (m, len)
            int p = pre[0][i], j = pre[1][i], r = suf[1][i], l = j;
            while (a[i] + k >= a[l]) l = pre[1][l];
            int _l = std::max(p, (l+i)/2), _r = std::min(i, (j+r)/2) - 1;
            for (int m = _l; m <= _r; m++)
                rangeAdd(std::max(i-m, m-j+1), std::min(m-l, r-m-1), a2[m]);
        } // l < m-len+1 <= j <= p <= m < i <= m+len < r
    }
    for (int i = 1; i <= n; i++) sum[i] += sum[i-1];
    for (int i = 1; i <= n; i++) printf("%lld ", sum[i]);
    putchar('\n');
}
int main() {
    freopen("battle.in", "r", stdin);
    freopen("battle.out", "w", stdout);
    int T;
    scanf("%d", &T);
    while (T--) solve();
}