#include <stdio.h>
#include <string.h>
#include <vector>
#include <set>
constexpr int N = 10005;
int val[N], mnv[N];
std::vector<int> g[N];
int bl2[N], top;
namespace Tarjan {
    unsigned dfn[N], low[N], ts;
    int sta[N];
    bool insta[N];
    void dfs(int u) {
        dfn[u] = low[u] = ++ts;
        sta[++sta[0]] = u;
        insta[u] = true;
        for (int v : g[u]) {
            if (!dfn[v]) {
                dfs(v);
                low[u] = std::min(low[u], low[v]);
            }
            else if (insta[v]) {
                low[u] = std::min(low[u], dfn[v]);
            }
        }
        if (dfn[u] == low[u]) {
            ++top;
            do {
                bl2[sta[sta[0]]] = top;
                if (~val[sta[sta[0]]]) {
                    if (~mnv[top])
                        mnv[top] = std::min(mnv[top], val[sta[sta[0]]]);
                    else
                        mnv[top] = val[sta[sta[0]]];
                }
                insta[sta[sta[0]]] = false;
            } while (sta[sta[0]--] != u);
        }
    }
    inline void tarj(int n) {
        for (int i = 1; i <= n; i++)
            if (!dfn[i]) dfs(i);
    }
}
bool hasin[N];
int main() {
#ifndef ONLINE_JUDGE
    freopen("P1262_4.in", "r", stdin);
    freopen("P1262.out", "w", stdout);
#endif
    memset(val, -1, sizeof val);
    memset(mnv, -1, sizeof mnv);
    int n, p;
    scanf("%d%d", &n, &p);
    for (int id, v; p--; ) {
        scanf("%d%d", &id, &v);
        val[id] = v;
    }
    scanf("%d", &p);
    for (int u, v; p--; ) {
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
    }
    Tarjan::tarj(n);
    for (int i = 1; i <= n; i++)
        for (int j : g[i])
            if (bl2[i] ^ bl2[j])
                hasin[bl2[j]] = true;
    int ans = 0;
    std::set<int> unavaliable;
    for (int i = 1; i <= top; i++)
        if (!hasin[i]) {
            if (~mnv[i]) ans += mnv[i];
            else unavaliable.insert(i);
        }
    if (top == 1) ans = mnv[1];
    if (unavaliable.empty()) {
        printf("YES\n%d\n", ans);
        return 0;
    }
    puts("NO");
    for (int i = 1; i <= top; i++)
        if (mnv[i] == -1) unavaliable.insert(i);
    for (int i = 1; i <= n; i++)
        if (unavaliable.count(bl2[i]))
            return printf("%d\n", i), 0;
    return 0;
}