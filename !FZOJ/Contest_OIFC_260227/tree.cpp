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
std::vector<int> g[N];
int f[N][32];
void dfs(int u, int fa) {
    memset(f[u], 0x3f, sizeof(int) * 32);
    f[u][1 << a[u]-1] = 1;
    for (int v : g[u]) {
        if (v == fa) continue;
        dfs(v, u);
        for (int su = 31; su >= 0; --su)
            for (int sv = 0; sv ^ 32; ++sv)
                f[u][su | sv] = std::min(f[u][su | sv], f[u][su] + f[v][sv]);
    }
}
inline int solve(int n) {
    dfs(1, 0);
    int ans = 1145141919;
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
    
    return 0;
}