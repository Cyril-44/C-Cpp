#include <bits/stdc++.h>
constexpr int N = 205;
template<typename Tp>
struct MaxFlow {
    MaxFlow(int n_, int s, int t) : g(new EdgeList[n_+1]), dis(new int[n_+1]), head(new int[n_+1]), n(n_), S(s), T(t), modified(false), maxflow() {}
    ~MaxFlow() { delete[] g; delete[] dis; delete[] head; }
    inline void addedg(int fr, int to, Tp c) {
        modified = true;
        g[fr].emplace_back(to, c, (int)g[to].size());
        g[to].emplace_back(fr, Tp(0), (int)g[fr].size() - 1);
    }
    inline bool bfs() {
        modified = true;
        static int que[$];
        memset(dis, 0, sizeof(int) * (n+1));
        memset(head, 0, sizeof(int) * (n+1));
        int hd = 0, tl = 0;
        que[tl++] = S; dis[S] = 1;
        while (hd ^ tl) {
            int u = que[hd++];
            for (const auto &[v, cap, bak] : g[u])
                if (cap && !dis[v]) dis[v] = dis[u] + 1, que[tl++] = v;
        }
        return dis[T];
    }
    Tp dfs(int u, Tp infl) {
        if (u == T) return infl;
        Tp outfl{};
        for (int &i = head[u]; i ^ (int)(g[u].size()); i++) {
            auto &[v, cap, bak] = g[u][i];
            if (cap && dis[v] == dis[u] + 1) {
                Tp fl = dfs(v, std::min(cap, infl));
                if (fl) {
                    infl -= fl, outfl += fl;
                    cap -= fl, std::get<1>(g[v][bak]) += fl;
                } else dis[v] = 0;
            }
        }
        return outfl;
    }
    inline Tp operator()() {
        if (!modified) return maxflow;
        while (bfs()) maxflow += dfs(S, std::numeric_limits<Tp>::max());
        return maxflow;
    }
    using EdgeList = std::vector<std::tuple<int, Tp, int>>;
    EdgeList *g;
    int *dis, *head, n, S, T;
    bool modified;
    Tp maxflow;
};
namespace Binary {
std::vector<int> edgs[N << 1];
bool vis[N << 1];
void dfs(int u) {
    if (vis[u]) return;
    for (int v : edgs[u]) dfs(v);
}
inline std::vector<int> mincover(MaxFlow<int> &mf, int n, int m) {
    int S = n+1, T = n+2;
    for (int i = 1; i <= n; i++)
        if (i <= m) mf.addedg(S, i, 1);
        else mf.addedg(i, T, 1);
    std::vector<int> ans;
    ans.reserve(n - mf());
    std::set<int> st;
    for (int i = m+1; i <= n; i++) st.insert(i);
    for (int u = 1; u <= m; u++)
        for (const auto &[v, cap, bak] : mf.g[u])
            if (cap == 0) {
                edgs[u].push_back(v);
            } else {
                edgs[v].push_back(u);
                st.erase(v);
            }
    for (int i : st) dfs(i);
    for (int i = 1; i <= m; i++)
        if (vis[i]) ans.push_back(i);
    for (int i = m+1; i <= n; i++)
        if (!vis[i]) ans.push_back(i);
    return ans;
}
inline std::vector<int> maxstandalone(MaxFlow<int> &mf, int n, int m) {
    auto anstmp = std::move(mincover(mf, n, m));
    std::vector<int> ans;
    auto ptr = anstmp.begin();
    for (int i = 1; i <= n; i++)
        if (ptr != anstmp.end() && *ptr == i) ++ptr;
        else ans.push_back(i);
    return ans;
}
inline std::vector<std::pair<int,int>> match(MaxFlow<int> &mf, int n, int m) {
    int S = n+1, T = n+2;
    for (int i = 1; i <= n; i++)
        if (i <= m) mf.addedg(S, i, 1);
        else mf.addedg(i, T, 1);
    std::vector<std::pair<int,int>> ans;
    ans.reserve(n - mf());
    for (int u = 1; u <= m; u++)
        for (const auto &[v, cap, bak] : mf.g[u])
            if (cap == 0) ans.emplace_back(u, v);
    return ans;
}
}