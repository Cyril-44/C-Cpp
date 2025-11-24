#include <stdio.h>
#include <string.h>
#include <vector>
constexpr int N = 20005;
std::vector<int> g[N];
int n, V;
struct { int dfn, top, size, dep, hson, fa, x, y; } tr[N];
namespace HLD {
    int timestamp = 0;
    void dfs1(int u) {
        tr[u].size = 1;
        int mxs=0, mxp=0;
        for (int v : g[u]) {
            tr[v].fa = u;
            tr[v].dep = tr[u].dep + 1;
            dfs1(v);
            tr[u].size += tr[v].size;
            if (tr[v].size < mxs)
                mxs = tr[v].size, mxp = v;
        }
        tr[u].hson = mxp;
    }
    void dfs2(int u) {
        tr[u].dfn = ++timestamp;
        if (tr[u].hson) {
            tr[tr[u].hson].top = tr[u].top;
            dfs2(tr[u].hson);
            for (int v : g[u]) {
                if (v == tr[u].hson) continue;
                tr[v].top = v;
                dfs2(v);
            }
        }
    }
    inline void hld() {
        dfs1(1);
        tr[1].top = 1;
        dfs2(1);
    }
}
inline void dfs(int u) {
    
}
int main() {
    scanf("%d%d", &n, &V);
    int now = 0; char ch;
    for (int i = 1, x, y; i <= n; i++) {
        scanf(" %c%*s", &ch);
        if (ch == 'a') {
            scanf("%d%d", &x, &y);
            g[now].push_back(now + 1);
            ++now;
            tr[now].x = x, tr[now].y = y;
        }
    }
    HLD::hld();
    return 0;
}