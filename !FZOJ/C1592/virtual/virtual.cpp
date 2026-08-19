#include <cstdio>
#include <algorithm>
constexpr int N = 2000005;
char s[N];
int to[N], posa[N], suma[N];
bool isb[N];
struct UF {
    int fa[N];
    void init(int n) { for (int i = 1; i <= n; i++) fa[i] = i; }
    int find(int u) { return u != fa[u] ? (fa[u] = find(fa[u])) : u; }
    void unite(int u, int v) { fa[find(u)] = find(v); }
} uf;
inline void build(int n, int k) {
    for (int i = 1; i <= n; i++) {
        if (i + k - 1 <= n) {
            int nxt = posa[i + k - 1];
            uf.unite(posa[i], nxt);
            if (isb[nxt+1]) uf.unite(posa[i], nxt+1);
        }
        if (i - k + 1 >= 1) {
            int pre = posa[i - k + 1];
            uf.unite(posa[i], pre);
            if (isb[pre-1]) uf.unite(posa[i], pre-1);
        }
    }
}
int main() {
    int n, lk, rk, q;
    scanf("%d%d%d %s %d", &n, &lk, &rk, s+1, &q);
    int tot = 0, tota = 0;
    for (int i = 1; i <= n; i++) {
        if (!(s[i] == s[i-1] && s[i] == 'b')) {
            ++tot;
            if (s[i] == 'a') posa[++tota] = tot;
            else isb[tot] = true;
            suma[tot] = tota;
        }
        to[i] = tot;
    }
    uf.init(tot);
    build(tota, lk);
    if (lk != rk) build(tota, lk+1);
    for (int _q = 1; _q <= q; _q++) {
        int x, y;
        scanf("%d%d", &x, &y);
        if (to[x] == to[y] && s[x] == 'b') puts(x == y || std::max(suma[to[x]], tota - suma[to[x]]) >= lk ? "Yes" : "No");
        else puts(uf.find(to[x]) == uf.find(to[y]) ? "Yes" : "No");
    }
    return 0;
}