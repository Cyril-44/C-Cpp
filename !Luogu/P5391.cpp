#include <stdio.h>
#include <string.h>
#include <vector>
constexpr int N = 20005;
std::vector<int> g[N];
int n, V;
struct { int size, hson, fa, x, y, ans; } tr[N];
int match[N];
int f[17][N];
namespace HLD {
    void dfs1(int u) {
        tr[u].size = 1;
        int mxs=0, mxp=0;
        for (int v : g[u]) {
            dfs1(v);
            tr[u].size += tr[v].size;
            if (tr[v].size > mxs)
                mxs = tr[v].size, mxp = v;
        }
        tr[u].hson = mxp;
    }
    void dfs2(int u, int dep) {
        for (int i = tr[u].x; i <= V; i++)
            f[dep][i] = std::max(f[dep][i], f[dep][i - tr[u].x] + tr[u].y);
        tr[u].ans = f[dep][V];
        if (tr[u].hson) {
            for (int v : g[u]) {
                if (v == tr[u].hson) continue;
                memcpy(f[dep + 1], f[dep], sizeof(int) * N);
                tr[v].ans = tr[u].ans;
                dfs2(v, dep + 1);
            }
            dfs2(tr[u].hson, dep);
        }
    }
    inline void solve(int tot) {
        dfs1(1);
        dfs2(1, 0);
    }
}
int main() {
    scanf("%d%d", &n, &V);
    int now = 1, tot = 1; char s[10];
    for (int i = 1, x, y; i <= n; i++) {
        scanf(" %s", s);
        if (*s == 'a') {
            scanf("%d%d", &x, &y);
            g[now].push_back(++tot);
            tr[tot].fa = now;
            tr[tot].x = x, tr[tot].y = y;
            now = tot;
        }
        else now = tr[now].fa;
        match[i] = now;
    }
    HLD::solve(tot);
    for (int i = 1; i <= n; i++)
        printf("%d\n", tr[match[i]].ans);
    return 0;
}