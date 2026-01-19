#include <bits/stdc++.h>
constexpr int N = 30005, K = 16;
inline void getSA(int n, char *s, int *sa, int *rk_, int *height, int sigma = 127) {
    static int tsa[N], h[N], cnt[N];
    int *rk = rk_, *trk = h;
    auto radixSort = [&]() { 
        memset(cnt, 0, sizeof(int) * (sigma + 1));
        for (int i = 1; i <= n; i++) ++cnt[rk[i]];
        for (int i = 1; i <= sigma; i++) cnt[i] += cnt[i-1];
        for (int i = n; i >= 1; i--) sa[cnt[rk[tsa[i]]]--] = tsa[i];
    };
    for (int i = 1; i <= n; i++) tsa[i] = i, rk[i] = s[i];
    radixSort();
    for (int w = 1; w < n; w <<= 1) {
        int top = 0;
        for (int i = n - w + 1; i <= n; i++) tsa[++top] = i;
        for (int i = 1; i <= n; i++) if (sa[i] > w) tsa[++top] = sa[i] - w;
        radixSort();
        sigma = 0;
        for (int i = 1; i <= n; i++)
            trk[sa[i]] = sigma += !(rk[sa[i]] == rk[sa[i-1]] && (sa[i]+w <= n && sa[i-1]+w <= n && rk[sa[i] + w] == rk[sa[i-1] + w]));
        std::swap(rk, trk);
    }
    if (rk != rk_) memcpy(rk_+1, rk+1, sizeof(int) * n);
    for (int i = 1, k = 0; i <= n; i++) {
        if (k) --k;
        while (i+k<=n && sa[rk[i]-1]+k<=n && s[i + k] == s[sa[rk[i] - 1] + k]) ++k;
        height[rk[i]] = k;
    }
}

char s[N], rs[N];
int sa[N], rk[N], h[N], rsa[N], rrk[N], rh[N];
using SparseTable = int[N][K+1];
SparseTable lcpst, lcsst;
int n;
inline void initST(int n, int *a, SparseTable st) {
    for (int i = 1; i <= n; i++) st[i][0] = a[i];
    for (int k = 1; k <= K; k++)
        for (int i = 1, rg = n - (1 << k) + 1; i <= rg; i++)
            st[i][k] = std::min(st[i][k-1], st[i+(1<<k-1)][k-1]);
}
inline int lcp(int l, int r) { // 给定的 l,r 是 下标，先转成 rk
    l = rk[l], r = rk[r];
    if (l > r) std::swap(l, r);
    if (l == r) return n - sa[l] + 1;
    int k = 31 - __builtin_clz(r - l++);
    return std::min(lcpst[l][k], lcpst[r-(1<<k)+1][k]);
}
inline int lcs(int l, int r) {
    l = rrk[n - l + 1], r = rrk[n - r + 1];
    if (l > r) std::swap(l, r);
    if (l == r) return n - rsa[l] + 1;
    int k = 31 - __builtin_clz(r - l++);
    return std::min(lcsst[l][k], lcsst[r-(1<<k)+1][k]);
}

int f[N], g[N];
inline void solveSingle() {
    scanf("%s", s+1);
    n = strlen(s + 1);
    getSA(n, s, sa, rk, h);
    std::reverse_copy(s+1, s+1+n, rs+1);
    getSA(n, rs, rsa, rrk, rh);
    initST(n, h, lcpst);
    initST(n, rh, lcsst);
    memset(f + 1, 0, sizeof(int) * n);
    memset(g + 1, 0, sizeof(int) * n);
    for (int l = 1; l <= (n>>1); l++) {
        for (int i = l, j = l + l; j <= n; i += l, j += l) {
            int llcp = std::min(l, lcp(i, j)), llcs = std::min(l - 1, lcs(i-1, j-1));
            if (llcp + llcs >= l) { // 两个关键点之间有 runlen = l
                int range = llcp + llcs - (l-1);
                ++f[j + llcp - range], --f[j + llcp]; // f[j-(range-lcp) .. j+llcp-1] ++
                ++g[i - llcs], --g[i - llcs + range]; // g[i-lcs .. i+(range-lcs)] ++
                // printf("++f[%d..%d], ++g[%d..%d]\n", j + llcp - range, j + llcp-1, i - llcs, i - llcs + range - 1);
            }
        }
    }
    long long ans = 0;
    for (int i = 1; i <= n; i++) {
        f[i] += f[i-1], g[i] += g[i-1];
        ans += 1ll * f[i-1] * g[i];
    }
    printf("%lld\n", ans);
}
int main() {
    int T;
    scanf("%d", &T);
    while (T--) solveSingle();
    return 0;
}