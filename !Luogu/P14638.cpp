#include <bits/stdc++.h>
constexpr int N = 50005, K = 15;
int64_t sum[N][K+1], ssum[N][K+1], f[K+1][K+1][N], g[N], ans[N];
int n, frg[K+1], a[N];
inline int64_t inqmax(int64_t s[N][K+1], int l, int r) {
    l = std::max(l, 1), r = std::min(r, n);
    assert(l <= r);
    int k = std::__lg(r - l + 1);
    return std::max(s[l][k], s[r - (1 << k) + 1][k]);
}
inline void solve(int l, int r) { // Assume l <= r < 2l
    assert(l <= r && r < 2 * l);
    memset(g, 0x80, sizeof g);
    for (int i = 1; i <= n; i += l) {
        int64_t res = -5e9;
        const int jr = std::min(i + l - 1, n);
        for (int j = i; j <= jr; j++) {
            if (j+l-1 <= n) res = std::max(res, inqmax(sum, j+l-1, j+r-1) - sum[j-1][0]);
            g[j] = std::max(g[j], res);
        }
        res = -5e9;
        for (int j = jr; j >= i; j--) {
            if (j-l+1 >= 1) res = std::max(res, inqmax(ssum, j-r+1, j-l+1) - ssum[j+1][0]);
            g[j] = std::max(g[j], res);
        }
        res = -5e9;
        for (int j = std::max(1, jr - r + 1); j <= i; j++)
            if (j+l-1 <= n) res = std::max(res, inqmax(sum, std::max(jr, j+l-1), j+r-1) - sum[j-1][0]);
        for (int j = i; j <= jr; j++) g[j] = std::max(g[j], res);
    }
}
inline void updateans(int64_t *v = g, int64_t *h = ans) {
    for (int i = 1; i <= n; i++) h[i] = std::max(h[i], v[i]);
}
int main() {
    int q;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) scanf("%d", &a[i]), sum[i][0] = sum[i-1][0] + a[i];
    for (int i = n; i >= 1; i--) ssum[i][0] = ssum[i+1][0] + a[i];
    for (int k = 1; k <= K; k++)
        for (int i = 1; i + (1<<k) - 1 <= n; i++) {
            sum[i][k] = std::max(sum[i][k - 1], sum[i + (1<<k-1)][k - 1]);
            ssum[i][k] = std::max(ssum[i][k - 1], ssum[i + (1<<k-1)][k - 1]);
        }
    for (int k = 0; k <= K; k++) {
        if ((1 << k) > n) break;
        frg[k] = std::min((1 << k + 1) - 1, n);
        solve(1 << k, frg[k]);
        memcpy(f[k][k], g, sizeof g);
    }
    for (int kl = 0; kl < K; kl++)
        for (int kr = kl + 1; kr <= K; kr++) {
            memcpy(f[kl][kr], f[kl][kr-1], sizeof g);
            updateans(f[kr][kr], f[kl][kr]);
        }
    scanf("%d", &q);
    for (int l, r; q--;) {
        scanf("%d%d", &l, &r);
        memset(ans, 0x80, sizeof ans);
        int mn = K+1, mx = -1;
        for (int k = 0; k <= K; k++) {
            if ((1 << k) > r) break;
            if ((1 << k) >= l && frg[k] <= r)
                mn = std::min(mn, k), mx = std::max(mx, k);
            else {
                int fl = std::max(1 << k, l), fr = std::min(frg[k], r);
                if (fl <= fr) solve(fl, fr), updateans();
            }
        }
        if (mn <= mx) updateans(f[mn][mx]);
        uint64_t res = 0;
        for (int i = 1; i <= n; i++)
            res ^= 1ull * i * ans[i];
        printf("%llu\n", res);
    }
}