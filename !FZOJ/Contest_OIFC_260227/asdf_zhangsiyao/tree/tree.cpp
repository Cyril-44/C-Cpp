#include <limits>
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <vector>
#include <map>
constexpr int N = 100005, K = 17;
namespace FastI {
    char buf[1 << 20], *p1{}, *p2{};
    [[gnu::always_inline]] inline char gc() {
        if (__builtin_expect(p1 == p2, 0)) p2 = (p1 = buf) + fread(buf, 1, sizeof buf, stdin);
        return *p1++; 
    }
    inline void in(int &x) {
        char ch = gc();
        while (ch < '0' || ch > '9') ch = gc();
        for (x = 0; ch >= '0' && ch <= '9'; ch = gc())
            x = (x << 3) + (x << 1) + (ch & 0xf);
    }
} using FastI::in;
int a[N];
int fa[N], dep[N];
std::vector<int> g[N];
namespace LCA {
int st[N][K+1];
int dfn[N], ts = 0;
void dfs(int u, int pre) {
    st[dfn[u] = ++ts][0] = pre;
    for (int v : g[u]) {
        if (v == pre) continue;
        dfs(v, u);
    }
}
inline int calc(int x, int y) { return dfn[x] < dfn[y] ? x : y; }
inline void init(int n, int rt = 1) {
    dfs(rt, 0);
    for (int k = 1; k <= K; k++)
        for (int i = 1; i + (1 << k) - 1 <= n; i++)
            st[i][k] = calc(st[i][k-1], st[i + (1<<k-1)][k-1]);
}
inline int lca(int x, int y) {
    if (x == y) return x;
    if ((x = dfn[x]) > (y = dfn[y])) std::swap(x, y);
    int k = 31 - __builtin_clz(y - x++);
    return calc(st[x][k], st[y - (1<<k) + 1][k]);
}
inline int dis(int u, int v) {
    return dep[u] + dep[v] - 2 * dep[lca(u, v)];
}
}
void init(int u) {
    for (int v : g[u]) {
        if (v == fa[u]) continue;
        fa[v] = u, dep[v] = dep[u] + 1;
        init(v);
    }
}
namespace Ai5 {
int f[N][32];
void dfs(int u) {
    memset(f[u], 0x3f, sizeof(int) * 32);
    f[u][1 << a[u]-1] = 1;
    for (int v : g[u]) {
        if (v == fa[u]) continue;
        dfs(v);
        for (int su = 31; su >= 0; --su)
            for (int sv = 0; sv ^ 32; ++sv)
                f[u][su | sv] = std::min(f[u][su | sv], f[u][su] + f[v][sv]);
    }
}
inline void solve(int n) {
    dfs(1);
    int ans = 1145141919;
    for (int i = 1; i <= n; i++) ans = std::min(ans, f[i][31]);
    printf("%d\n", ans);
}
}
namespace BruteForce {
inline int dis(int u, const std::vector<int>& vv) {
    int mn = std::numeric_limits<int>::max();
    for (int v : vv) mn = std::min(mn, LCA::dis(u, v));
    return mn;
}
int ans;
inline void dfs(int u, int fa, int dep, std::vector<int> occur) {
    if (std::find(occur.begin(), occur.end(), a[u]) == occur.end()) {
        occur.push_back(a[u]);
        if (occur.size() == 5) return ans = std::min(ans, dep), void();
    }
    for (int v : g[u]) {
        if (v == fa) continue;
        dfs(v, u, dep + 1, occur);
    }
}
inline void solve(int n) {
    std::map<int, std::vector<int>> allmp;
    for (int i = 1; i <= n; i++) allmp[a[i]].push_back(i);
    std::vector<std::pair<int,std::vector<int>>> all;
    all.reserve(allmp.size());
    for (const auto &i : allmp) all.push_back(i);
    ans = n;
    for (int u = 1; u <= n; u++) {
        std::map<int, std::vector<int>> mp;
        for (int v : g[u]) {
            if (a[v] == a[u]) continue;
            mp[a[v]].push_back(v);
            if (mp.size() == 4) { puts("5"); return; }
        }
        switch (mp.size()) {
        case 2: {
            int c0 = mp.begin()->first, c1 = mp.rbegin()->first;
            auto occured = [&](int col) { return col == a[u] || col == c0 || col == c1; };
            const auto &v0 = mp.begin()->second, &v1 = mp.rbegin()->second;
            for (auto it2 = all.begin(); it2 != all.end(); ++it2) { if (occured(it2->first)) continue;
            for (auto it3 = std::next(it2); it3 != all.end(); ++it3) { if (occured(it3->first)) continue;
            for (int p0 : v0) for (int p1 : v1)
            for (int p2 : it2->second) for (int p3 : it3->second) {
                int sol = 3 + std::min(dis(p2, {u, p0, p1}) + dis(p3, {u, p0, p1, p2}), 
                                       dis(p3, {u, p0, p1}) + dis(p2, {u, p0, p1, p3}));
                ans = std::min(ans, sol);
            }}}
        break; }
        case 3: {
            auto occured = [&](int col) { return col == a[u] || mp.count(col); };
            for (auto it3 = all.begin(); it3 != all.end(); ++it3) { if (occured(it3->first)) continue;
            for (int p0 : mp.begin()->second)
            for (int p1 : std::next(mp.begin())->second)
            for (int p2 : mp.rbegin()->second)
            for (int p3 : it3->second) {
                int sol = 4 + dis(p3, {u, p0, p1, p2});
                ans = std::min(ans, sol);
            }}
        }}
    }
    for (int i = 1; i <= n; i++) dfs(i, 0, 1, {});
    printf("%d\n", ans);
}
}
int main() {
    freopen("tree.in", "r", stdin);
    freopen("tree.out", "w", stdout);
    int n, mx = 0;
    in(n);
    for (int i = 1; i <= n; i++) {
        in(a[i]);
        mx = std::max(mx, a[i]);
    }
    for (int i = 1, u, v; i < n; i++) {
        in(u), in(v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    init(1); LCA::init(n);
    if (mx <= 5) return Ai5::solve(n), 0;
    BruteForce::solve(n);
    return 0;
}