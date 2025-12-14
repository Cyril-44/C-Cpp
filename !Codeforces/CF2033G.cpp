#include <stdio.h>
#include <string.h>
#include <algorithm>
using std::max;
const int N = 200005, M = 18;
struct Edge {
    int to, nxt;
} e[N << 1];
int head[N];
inline void addedg(const int &fr, const int &to) {
    e[++head[0]] = Edge{to, head[fr]};
    head[fr] = head[0];
}
bool vis[N];
int d[N], fur[N][2], fa[N][20];
bool chain[N];
inline void dfs(const int &u, const int &fa) {
    fur[u][0] = u;
    vis[u] = true;
    ::fa[u][0] = fa;
    chain[u] = !e[head[u]].nxt;
    for (int i = head[u], v; i; i = e[i].nxt) {
        v = e[i].to;
        if (vis[v]) continue;
        d[v] = d[u] + 1;
        dfs(v, u);
        if (!chain[v])
            chain[u] = false;
        if (d[fur[v][0]] > d[fur[u][0]])
            fur[u][1] = fur[u][0], fur[u][0] = fur[v][0];
        else if (d[fur[v][0]] > d[fur[u][1]])
            fur[u][1] = fur[v][0];
    }
}
inline void initfa(const int &n) {
    for (int k = 1; k <= M; k++)
        for (int i = 1; i <= n; i++)
            fa[i][k] = fa[fa[i][k-1]][k-1];
}
inline int getfa(int u, const int &d) {
    for (int k = M; k >= 0; k--)
        if (d & (1<<k))
            u = fa[u][k];
    return u;
}
inline int getnearpfa(int u) {
    if (!chain[u]) return u;
    for (int k = M; k >= 0; k--)
        if (chain[fa[u][k]])
            u = fa[u][k];
    return fa[u][0];
}
int main() {
    int t, n, q, u, v;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        memset(head, 0, sizeof(int) * (n+1));
        for (int i = 1; i < n; i++) {
            scanf("%d%d", &u, &v);
            addedg(u, v);
            addedg(v, u);
        }
        memset(vis, 0, sizeof(bool) * (n+1));
        memset(fur, 0, sizeof(int) * 2 * (n+1));
        memset(chain, 0, sizeof(bool) * (n+1));
        d[1] = 1;
        dfs(1, 1), initfa(n);
        scanf("%d", &q);
        while (q--) {
            scanf("%d%d", &u, &v);
            int _fa = getfa(u, v), _nfa = getnearpfa(u), ans = 0;
            if (d[_fa] < d[_nfa]) {
                ans = d[fur[u][0]] - d[u];
            }
            else {
                if (fur[_nfa][0] ^ fur[u][0])
                    ans = max(ans, d[fur[_nfa][0]] - d[_nfa] + d[u] - d[_nfa]);
                else
                    ans = max(ans, d[fur[_nfa][1]] - d[_nfa] + d[u] - d[_nfa]);
                if (fur[_fa][0] ^ fur[_nfa][0])
                    ans = max(ans, d[fur[_fa][0]] - d[_fa] + d[u] - d[_fa]);
                else
                    ans = max(ans, d[fur[_fa][1]] - d[_fa] + d[u] - d[_fa]);
            }
            printf("%d ", ans);
        }
        putchar('\n');
    }
    return 0;
}