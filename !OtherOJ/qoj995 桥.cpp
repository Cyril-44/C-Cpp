#include <cstdio>
#include <cstring>
constexpr int N = 100005, M = 500005;
template<typename T> inline void umin(T &x, T y) { (x > y) && (x = y); }
struct Edge {
    int fr, to, nxt;
    bool isBrg;
} e[M << 1];
int head[N]{1};
inline void addedg(int fr, int to) {
    e[++head[0]] = {fr, to, head[fr], false};
    head[fr] = head[0];
}
int dfn[N], low[N], ts;
void tarjan(int u, int faEdg) {
    dfn[u] = low[u] = ++ts;
    for (int i = head[u], v; i; i = e[i].nxt) {
        if ((i ^ faEdg) == 1) continue;
        v = e[i].to;
        if (!dfn[v])
            tarjan(v, i), umin(low[u], low[v]);
        else umin(low[u], low[v]);
        if (low[v] > dfn[u]) e[i].isBrg = e[i^1].isBrg = true;
    }
}
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int u, v, i = m; i--; ) {
        scanf("%d%d", &u, &v);
        addedg(u, v);
        addedg(v, u);
    }
    for (int i = 1; i <= n; i++)
        if (!dfn[i]) tarjan(i, 0);
    for (int j = 2; j <= (m<<1); j += 2)
        if (e[j].isBrg) printf("%d %d\n", e[j].fr, e[j].to);
        
    return 0;
}