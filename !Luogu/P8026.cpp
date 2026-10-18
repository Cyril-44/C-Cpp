#include <bits/stdc++.h>
constexpr int D = 201, N = 5001;
#ifdef CLANGD
#define N 1
#endif
std::mt19937_64 rng(std::random_device{}());
std::vector<int> pnts[N * D];
std::unordered_map<uint64_t, int> cnt;
uint64_t base[N * D], val[N];
int rt[N * D], n;
uint64_t ans = 0;
inline void change(int x, int y) { // rt[x] --> y
    int u = (x - 1) % n + 1;
    ans -= (--cnt[val[u]]) << 1 | 1;
    val[u] ^= base[rt[x]] ^ base[y];
    rt[x] = y, pnts[y].push_back(x);
    ans += (cnt[val[u]]++) << 1 | 1;
}
inline void merge(int u, int v) {
    u = rt[u], v = rt[v];
    if (u == v) return;
    if (pnts[u].size() < pnts[v].size()) std::swap(u, v);
    for (int x : pnts[v]) change(x, u);
}
int main() {
    int d, m;
    scanf("%d%d%d", &d, &n, &m);
    std::iota(rt+1, rt+1+d*n, 1);
    for (int i = 1; i <= d*n; i++) {
        pnts[i].push_back(i);
        base[i] = rng();
        val[(i-1)%n+1] ^= base[i];
    }
    for (int i = 1; i <= n; i++)
        ans += (cnt[val[i]]++) << 1 | 1;
    for (int a, b, k; m--; ) {
        scanf("%d%d%d", &a, &b, &k);
        merge(a + (k-1) * n, b + (k-1) * n);
        printf("%llu\n", ans);
    }
}