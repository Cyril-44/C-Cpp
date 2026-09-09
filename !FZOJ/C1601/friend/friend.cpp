#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <cassert>
#ifdef CLANGD
constexpr int N = 14, M = 14;
#else
constexpr int N = 100004, M = 10000004;
#endif
std::vector<int> g[N], grp[M];
int p[M / 10], mnfac[M], dfn[N], low[N], sz[N], mxsz[N];
bool vis[N], np[M], cut[N];
int n, top, fir_big;
int dfs1(int u) {
    vis[u] = true; int sz = 1;
    for (int v : g[u]) if (!vis[v]) sz += dfs1(v);
    return sz;
}
void tarjan(int u, int fa) {
    sz[u] = 1;
    dfn[u] = low[u] = ++top;
    int ch = 0;
    int ss = 0; // 被割掉的才能算
    for (int v : g[u])
        if (!dfn[v]) {
            ++ch;
            tarjan(v, u);
            sz[u] += sz[v];
            // mxsz[u] = std::max(mxsz[u], sz[v]);
            low[u] = std::min(low[u], low[v]);
            if (fa && low[v] >= dfn[u]) {
                cut[u] = true;
                mxsz[u] = std::max(mxsz[u], sz[v]);
                ss += sz[v];
            }
        }
        else if (v != fa) low[u] = std::min(low[u], dfn[v]);
    mxsz[u] = std::max(mxsz[u], fir_big - 1 - ss);
    if (!fa && ch >= 2) cut[u] = true;
}
int main() {
    for (int i = 2; i < M; i++) {
        if (!np[i]) p[++p[0]] = i, mnfac[i] = i;
        for (int j = 1; j <= p[0] && i * p[j] < M; j++) {
            np[i * p[j]] = true;
            mnfac[i * p[j]] = p[j];
            if (i % p[j] == 0) break;
        }
    }
    int T; scanf("%d", &T);
    while (T--) {
        scanf("%d", &n);
        for (int i = 4; i < M; i++) grp[i].clear();
        for (int i = 1, x; i <= n; i++) {
            int fac[10]{0}, cnt[10]{0};
            scanf("%d", &x);
            while (x > 1) {
                int val = mnfac[x];
                fac[++fac[0]] = val;
                do x /= val, ++cnt[fac[0]];
                while (x % val == 0);
            }
            for (int j = 1; j <= fac[0]; j++) {
                assert(!np[fac[j]]);
                if (cnt[j] > 1) grp[fac[j] * fac[j]].push_back(i);
                for (int k = j+1; k <= fac[0]; k++)
                    grp[fac[j] * fac[k]].push_back(i);
            }
        }
        for (int i = 1; i <= n; i++) g[i].clear();
        for (int i = 4; i < M; i++) {
            if (grp[i].size() <= 1) continue;
            for (int j = 1; j < (int)grp[i].size(); j++)
                g[grp[i][j]].push_back(grp[i][j-1]), g[grp[i][j-1]].push_back(grp[i][j]);
            g[grp[i].front()].push_back(grp[i].back());
            g[grp[i].back()].push_back(grp[i].front());
        }
        memset(vis, 0, sizeof vis);
        memset(cut, 0, sizeof cut);
        int mx = 0, mx2 = 0, mxid = 0;
        for (int i = 1; i <= n; i++)
            if (!vis[i]) {
                int s = dfs1(i);
                if (s > mx) mx2 = mx, mx = s, mxid = i;
                else if (s > mx2) mx2 = s;
            }
        fir_big = mx;
        memset(dfn, 0, sizeof dfn);
        memset(sz, 0, sizeof sz);
        memset(mxsz, 0, sizeof mxsz);
        top = 0; tarjan(mxid, 0);
        int ans = n;
        for (int i = 1; i <= n; i++) {
            if (dfn[i]) {
                if (cut[i]) ans = std::min(ans, std::max({mx2, mx - sz[i], mxsz[i]}));
                else ans = std::min(ans, std::max(mx - 1, mx2));
            }
            else ans = std::min(ans, mx);
        }
        printf("%d\n", ans);
    }
    return 0;
}