#include <cstdio>
#include <cstdint>
#include <vector>
#include <cassert>
#include <algorithm>
#include <numeric>
constexpr int N = 500005;
std::vector<std::pair<int,int>> mods[N << 2], ques[N];
int fa[N], sz[N], history[N][2], histCnt;
inline int find(int u) { return u ^ fa[u] ? find(fa[u]) : u; }
inline bool unite(int u, int v) {
    u = find(u), v = find(v);
    if (u == v) return false;
    if (sz[u] > sz[v]) std::swap(u, v);
    fa[u] = v, sz[v] += sz[u];
    ++histCnt;
    history[histCnt][0] = u, history[histCnt][1] = v;
    return true;
}
inline void rollback(int cnt) {
    while (cnt--) {
        int u = history[histCnt][0], v = history[histCnt][1];
        --histCnt;
        fa[u] = u, sz[v] -= sz[u];
    }
}
uint64_t ans;
int L, R, X, Y;
void modify(int u, int l, int r) {
    if (L <= l && r <= R) mods[u].emplace_back(X, Y);
    else {
        int mid = l + r >> 1;
        if (L <= mid) modify(u << 1, l, mid);
        if (mid < R) modify(u << 1 | 1, mid + 1, r);
    }
}
void solve(int u, int l, int r) {
    int cnt = 0;
    for (const auto &[x, y] : mods[u])
        cnt += unite(x, y);
    if (l == r) {
        for (const auto &[x, y] : ques[l])
            ans += 1ull * sz[find(x)] * sz[find(y)];
    } else {
        int mid = l + r >> 1;
        solve(u << 1, l, mid);
        solve(u << 1 | 1, mid + 1, r);
    }
    rollback(cnt);
}
/* 先拆贡献，算每条边在多少种路径中有贡献
 * 发现答案就是，对于每条权值为 v 的边，去掉所有权值为 v 的边，边两头的连通块的乘积就是贡献的路径数量
 */
int main() {
    int n;
    scanf("%d", &n);
    std::iota(fa+1, fa+1+n, 1);
    std::fill(sz+1, sz+1+n, 1);
    for (int i = 1, x, y, v; i < n; i++) {
        scanf("%d%d%d", &x, &y, &v);
        X = x, Y = y;
        if (v-1 >= 1) L = 1, R = v-1, modify(1, 1, n); // 1 ~ v-1 时刻，<x,y> 边都是存在的
        if (v+1 <= n) L = v+1, R = n, modify(1, 1, n); // v+1 ~ n 时刻也是一样的
        ques[v].emplace_back(x, y);
    }
    solve(1, 1, n);
    printf("%llu\n", ans);
    return 0;
}