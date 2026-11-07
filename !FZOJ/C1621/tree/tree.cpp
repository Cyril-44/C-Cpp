#include <bits/stdc++.h>
constexpr int N = 200005;
#ifdef CLANGD
#define N 25
#endif
constexpr uint64_t B = 31, MD = (uint64_t)1e8 + 7;
uint64_t base[N];
std::vector<std::pair<int, char>> g[N];
char s[N];
int end[N], m, sqrtm;
int64_t ans[N];
std::unordered_map<uint64_t, int> mp;
namespace DVD {
    bool vis[N];
    int sz[N], centroid, overallSz;
    void dfs1(int u, int fa) {
        sz[u] = 1;
        int mxsz = 0;
        for (auto [v, c] : g[u]) if (v != fa && !vis[v]) {
            dfs1(v, u);
            sz[u] += sz[v];
            mxsz = std::max(mxsz, sz[v]);
        }
        mxsz = std::max(mxsz, overallSz - sz[u]);
        if (mxsz <= overallSz/2) centroid = u;
    }
    int rt;
    std::vector<uint64_t> hash[N];
    void dfs2(int u, int fa, uint64_t now) {
        hash[rt].push_back(now);
        for (auto [v, c] : g[u])
            dfs2(v, u, (now * B + (c - 'a')) % MD);
    }
    void dfs(int u, int s) {
        overallSz = s, dfs1(u, 0);
        u = centroid, vis[u] = true;
        for (auto [v, c] : g[u])
            rt = v, dfs2(v, u, c - 'a');
        if (s <= sqrtm) {
            
        } else {
            
        }
        for (auto [v, c] : g[u]) {
            hash[v].clear(), hash[v].shrink_to_fit();
            dfs(v, sz[u] > sz[v] ? sz[v] : s - sz[u]);
        }
    }

}
int main() {
    int n;
    scanf("%d%d", &n, &m);
    base[0] = 1;
    for (int i = 1; i <= n; i++) base[i] = base[i-1] * B % MD;
    for (int i = 1; i < n; i++) {
        int u, v; char ch; scanf("%d%d %c", &u, &v, &ch);
        g[u].emplace_back(v, ch), g[v].emplace_back(u, ch);
    }
    sqrtm = sqrt(m);
    for (int i = 1; i <= m; i++) {
        scanf("%s", s[end[i-1]]);
        end[i] = end[i-1] + strlen(s + end[i-1]);
        for (int i = 1; i <= m; i++) {
            
        }
    }
}