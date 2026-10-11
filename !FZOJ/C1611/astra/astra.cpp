#include <bits/stdc++.h>
#ifdef CLANGD
constexpr int N = 25;
#else
constexpr int N = 200005;
#endif
using Pii = std::pair<int,int>;
std::vector<Pii> g[N];
std::vector<int> ng[N<<1];
int n, k;
namespace BuildGraph {
    Pii fa[N];
    int rep[N];
    void dfs1(int u) {
        for (auto [v, c] : g[u]) if (v != fa[u].first) fa[v] = {u, c}, dfs1(v);
    }
    int curEdgIdx, curEdgClr;
    void dfs2(int u) {
        ng[curEdgIdx].push_back(u);
        ng[u].push_back(curEdgIdx);
        for (auto [v, c] : g[u]) if (v != fa[u].first && c == curEdgClr) dfs2(v);
    }
    inline void work() {
        dfs1(1);
        int top = n;
        for (int u = 1; u <= n; u++)
            for (auto [v, c] : g[u]) if (v != fa[u].first && c != fa[u].second)
                curEdgIdx = ++top, ng[top].push_back(u), ng[u].push_back(top), dfs2(v);
    }
}
int cnt1[N], cnt2[N];
namespace CalcCnt1 {
    int curSubTrSz, centroid;
    bool vis[N<<1];
    int dfs1(int u, int fa = 0) {
        int mxsz = 0, szu = 1;
        for (int v : ng[u]) if (!vis[v] && v != fa) {
            int szv = dfs1(v, u);
            szu += szv;
            mxsz = std::max(mxsz, szv);
        }
        mxsz = std::max(mxsz, curSubTrSz - szu);
        if (mxsz <= curSubTrSz / 2) centroid = u;
        return szu;
    }
    std::vector<std::pair<int,int>> subtr[N];
    std::vector<int> cnt[N]; int curSonIdx;
    int dfs2(int u, int fa = 0, int dep = 1) {
        subtr[curSonIdx].emplace_back(u, dep);
        if (cnt[0].size() == dep) cnt[0].emplace_back();
        if (cnt[curSonIdx].size() == dep) cnt[curSonIdx].emplace_back(0);
        if (u <= n) ++cnt[0][dep], ++cnt[curSonIdx][dep];
        int sz = 1;
        for (int v : ng[u]) if (!vis[v] && v != fa)
            sz += dfs2(v, u, dep + 1);
        return sz;
    }
    void dfs(int u, int cursubsz) {
        curSubTrSz = cursubsz; dfs1(u);
        vis[u = centroid] = true;
        cnt[0].assign(1, 0);
        std::vector<std::pair<int,int>> sons;
        for (int v : ng[u]) if (!vis[v]) {
            curSonIdx = (int)sons.size() + 1;
            cnt[curSonIdx].assign(1, 0);
            subtr[curSonIdx].clear();
            sons.emplace_back(v, dfs2(v));
        }
        for (int i = 0; i <= (int)sons.size(); i++)
            for (int j = 1; j < (int)cnt[i].size(); j++)
                cnt[i][j] += cnt[i][j-1];
        for (int i = 1; i <= (int)sons.size(); i++)
            for (auto [u, dep] : subtr[i])
                if (u <= n && dep <= k) cnt1[u] += cnt[0][k - dep] - cnt[i][k - dep];
        for (int j = 1; j < (int)cnt[0].size() && j <= k; j++)
            cnt1[u] += cnt[0][j];
        for (auto [v, s] : sons) dfs(v, s);
    }
    inline void launchPointDivision() { dfs(1, n); }
}
namespace CalcCnt2 {
    constexpr int N = ::N << 1;
    int* f[N];
    int dep[N], lson[N], dfn[N], _f[N], top=0;
    void dfs1(int u, int fa=0) {
        for (int v : ng[u]) if (v != fa) {
            dfs1(v, u);
            if (dep[v] >= dep[lson[u]]) lson[u] = v;
            dep[u] = std::max(dep[u], dep[v] + 1);
        }
    }
    void dfs2(int u, int fa=0) {
        dfn[u] = ++top;
        f[u] = _f + top;
        if (u <= n) f[u][0]++;
        if (lson[u]) {
            dfs2(lson[u], u);
            for (int v : ng[u]) if (v != fa && v != lson[u]) {
                dfs2(v, u);
                for (int j = 0; j <= dep[v]; j++)
                    f[u][j+1] += f[v][j];
            }
        }
        if (u <= n && k <= dep[u]) cnt2[u] = f[u][k];
    }
    inline void launchDP() {
        dfs1(1);
        dfs2(1);
    }
}
int64_t com3(int n) {
    return n * (n-1ll) * (n-2ll) / 6;
}
int main() {
    scanf("%d%d", &n, &k);
    for (int u, v, c, i = 1; i < n; i++) {
        scanf("%d%d%d", &u, &v, &c);
        g[u].emplace_back(v, c);
        g[v].emplace_back(u, c);
    }
    BuildGraph::work();
    CalcCnt1::launchPointDivision();
    CalcCnt2::launchDP();
    int64_t ans = 0;
    for (int i = 1; i <= n; i++)
        ans += com3(cnt1[i]) - com3(cnt2[i]);
    printf("%lld\n", ans);
}