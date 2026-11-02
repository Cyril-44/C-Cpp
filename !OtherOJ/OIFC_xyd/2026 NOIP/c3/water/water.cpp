#include <bits/stdc++.h>
constexpr int N = 10;
inline int64_t min(int64_t x, int64_t y) { return x < y ? x : y; }
inline int64_t max(int64_t x, int64_t y) { return x > y ? x : y; }
int h[N], pre[N], suf[N], n, ans;
int64_t k;
inline void chk(int p) {
    pre[1] = h[1];
    for (int i = 2; i <= n; i++) pre[i] = std::max(pre[i-1], h[i]);
    suf[n] = h[n];
    for (int i = n-1; i >= 1; i--) suf[i] = std::max(suf[i+1], h[i]);
    int64_t now = 0;
    for (int i = 1; i <= n; i++)
        now += std::min(pre[i], suf[i]) - h[i];
    if (now >= k) ans = std::min(ans, p);
}
void dfs(int i, int j) {
    if (i > n) return chk(j);
    for (int l = 0; l <= 8 - j; l++) {
        h[i] += l;
        dfs(i+1, j+l);
        h[i] -= l;
    }
}
int main() {
    freopen("water.in", "r", stdin);
    freopen("water.out", "w", stdout);
    int T;
    scanf("%d", &T);
    while (T--) {
        scanf("%d%lld", &n, &k);
        for (int i = 1; i <= n; i++) scanf("%d", h+i);
        if (n == 3) { // max(h[2], min(h[1],h[3])) >= k
            int64_t ans = min(
                max(0, k - h[2]),
                max(0, k - h[1]) + max(0, k - h[3])
            );
            printf("%lld\n", ans);
        } else {
            ans = 9;
            dfs(1, 0);
            printf("%d\n", ans == 9 ? -1 : ans);
        }
    }
}