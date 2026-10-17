#include <bits/stdc++.h>
constexpr int N = 500005, M = 750005;
#ifdef CLANGD
#define N 55
#define M 15
#endif
std::pair<int,int> edgs[M];
std::vector<std::pair<int,int>> g[N];
int sta[M], fr[M], sz[M]; // 是哪一个环的边，环的大小
int64_t f[N];
bool vis[N];
void dfs1(int u, int coming = 0) {
    vis[u] = true;
    for (auto [v, i] : g[u]) if (i != coming) {
        if (fr[i]) continue;
        if (vis[v]) { // 找到新的环
            fr[i] = ++fr[0]; sz[fr[0]] = 1;
            do fr[sta[sta[0]--]] = fr[0], ++sz[fr[0]];
            while (edgs[sta[sta[0]+1]].first != v && edgs[sta[sta[0]+1]].second != v);
        }
        else {
            int pre = sta[0];
            sta[++sta[0]] = i;
            dfs1(v, i);
            sta[0] = std::min(sta[0], pre);
        }
    }
}
class Fetch {
    int start, ringid;
    void dfs(int u, int fa) {
        ring.push_back(u);
        for (auto [v, i] : g[u])
            if (fr[i] == ringid && v != fa && v != start)
                { dfs(v, u); break; }
    }
public:
    Fetch(int u, int id) : start(u), ringid(id) {
        ring.reserve(sz[id]);
        dfs(u, 0);
    }
    std::vector<int> ring;
};
std::vector<std::pair<int,int64_t>> extra[N];
void dfs(int u) {
    vis[u] = true;
    for (auto [v, id] : g[u]) if (!vis[v]) {
        id = fr[id];
        if (!id) dfs(v), f[u] += f[v] + 1;
        else {
            auto r = Fetch(u, id).ring;
            for (int x : r) vis[x] = true;
            for (int i = 1; i < sz[id]; i++) {
                dfs(r[i]);
                f[u] += f[r[i]] + 1;
            }
            for (int i = sz[id] / 2 + 1; i+1 < sz[id]; i++) {
                int64_t curfri = f[r[i]] + 1, ptr = sz[id] - 2 * i;
                auto it = std::upper_bound(extra[r[i]].begin(), extra[r[i]].end(), std::pair<int,int64_t>{ptr, 1ll<<60});
                if (it != extra[r[i]].begin())
                    curfri -= (--it)->second;
                f[u] += curfri;
                extra[u].emplace_back(ptr, f[r[i]] + 1);
            }
        }
    }
    std::sort(extra[u].begin(), extra[u].end());
    for (int i = 1; i < (int)extra[u].size(); i++)
        extra[u][i].second += extra[u][i-1].second;
}
inline void solveSingle() {
    memset(fr, 0, sizeof fr);
    memset(vis, 0, sizeof vis);
    memset(sz, 0, sizeof sz);
    int n, m; std::cin >> n >> m;
    for (int i = 1; i <= n; i++) g[i].clear(), extra[i].clear();
    for (int u, v, i = 1; i <= m; i++) {
        std::cin >> u >> v;
        edgs[i] = {u, v};
        g[u].emplace_back(v, i);
        g[v].emplace_back(u, i);
    }
    for (int i = 1; i <= n; i++) std::sort(g[i].begin(), g[i].end());
    dfs1(1);
    assert(m - n + 1 == fr[0]);
    memset(vis, 0, sizeof vis);
    memset(f, 0, sizeof f);
    dfs(1);
    std::cout << 1 + f[1] << '\n';
}
int main() {
    std::cin.tie(nullptr) -> sync_with_stdio(false);
    int C, T; std::cin >> C >> T;
    while (T--) solveSingle();
}