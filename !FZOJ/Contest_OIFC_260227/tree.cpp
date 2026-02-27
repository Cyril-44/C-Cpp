#include <cstdio>
#include <cstring>
#include <limits>
#include <algorithm>
#include <random>
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
int a[N], map[N];
std::vector<int> g[N];
int f[N][32];
void dfs(int u, int fa) {
    memset(f[u], 0x3f, sizeof(int) * 32);
    f[u][1 << map[a[u]]] = 1;
    for (int v : g[u]) {
        if (v == fa) continue;
        dfs(v, u);
        for (int su = 31; su >= 0; --su) {
            if (f[u][su] == 0x3f3f3f3f) continue;
            for (int sv = 0; sv ^ 32; ++sv)
                f[u][su | sv] = std::min(f[u][su | sv], f[u][su] + f[v][sv]);
        }
    }
}
inline int solve(int n) {
    dfs(1, 0);
    int ans = std::numeric_limits<int>::max();
    for (int i = 1; i <= n; i++) ans = std::min(ans, f[i][31]);
    return ans;
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
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> gen(0, 4);
    int ans = std::numeric_limits<int>::max();
    for (int tot = 120; tot--; ) {
        for (int i = 1; i <= mx; i++) map[i] = gen(rng);
        ans = std::min(ans, solve(n));
    }
    printf("%d\n", ans);
    return 0;
}