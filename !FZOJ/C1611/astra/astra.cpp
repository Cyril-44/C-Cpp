#include <bits/stdc++.h>
#ifdef CLANGD
constexpr int N = 25;
#else
constexpr int N = 200005;
#endif
using Pii = std::pair<int,int>;
std::vector<Pii> g[N];
std::vector<int> ng[N];
Pii fa[N];
int n, k;
namespace BuildGraph {
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
namespace PntDivide {
    int curSubTrSz, centroid;
    bool vis[N];
    int sons[N][2], dep[N];
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
    std::vector<int> cnt[N]; int curSonIdx;
    int dfs2(int u, int fa = 0, int dep = 1) {
        if (cnt[0].size() == dep) cnt[0].push_back(1); else ++cnt[0][dep];
        if (cnt[curSonIdx].size() == dep) cnt[curSonIdx].push_back(1); else ++cnt[curSonIdx][dep];
        int sz = 1;
        for (int v : ng[u]) if (!vis[v] && v != fa)
            sz += dfs2(v, u, dep + 1);
        return sz;
    }
    void dfs(int u, int cursubsz) {
        curSubTrSz = cursubsz; dfs1(u);
        vis[u = centroid] = true;
        int top = 0;
        cnt[0].assign(1, 0);
        for (int v : ng[u]) if (!vis[v])
            cnt[++top].assign(1, 0), sons[top][0] = v, sons[top][1] = dfs2(v);
        for (int i = 1; i <= top; i++) {
            
        }
        for (int i = 1; i <= top; i++)
            dfs(sons[i][0], sons[i][1]);
    }
}
int main() {
    scanf("%d%d", &n, &k);
    for (int u, v, c, i = 1; i < n; i++) {
        scanf("%d%d%d", &u, &v, &c);
        g[u].emplace_back(v, c);
        g[v].emplace_back(u, c);
    }
    BuildGraph::work();
    
}