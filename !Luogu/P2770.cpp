#include <bits/stdc++.h>
constexpr int N = 105;
template<typename Tp>
struct MaxCostMaxFlow {
    MaxCostMaxFlow(int n_, int s, int t) : g(new EdgeList[n_+1]), head(new int[n_+1]), dis(new Tp[n_+1]), vis(new bool[n_+1]), n(n_), S(s), T(t), maxflow(), maxcost() {}
    ~MaxCostMaxFlow() { delete[] g; delete[] head; delete[] dis; }
    inline void operator()(int fr, int to, Tp c1, Tp c2) {
        g[fr].emplace_back(to, c1, c2, (int)g[to].size());
        g[to].emplace_back(fr, 0, -c2, (int)g[fr].size() - 1);
    }
    inline bool spfa() {
        static int que[N << 1];
        memset(head, 0, sizeof(int) * (n+1));
        memset(dis, 0x80, sizeof(Tp) * (n+1));
        memset(vis, 0, sizeof(bool) * (n+1));
        int hd = 0, tl = 0;
        que[tl++] = S, dis[S] = 0;
        while (hd ^ tl) {
            int u = que[hd++];
            vis[u] = false;
            for (const auto &[v, cap, cost, bak] : g[u]) {
                if (cap && dis[v] < dis[u] + cost) {
                    dis[v] = dis[u] + cost;
                    if (!vis[v]) que[tl++] = v;
                }
            }
        }
        return dis[T] != (int)0x80808080;
    }
    Tp dfs(int u, Tp infl) {
        if (u == T) return infl;
        vis[u] = true;
        Tp outfl = 0;
        for (int &i = head[u]; i < (int)g[u].size(); i++) {
            auto &[v, cap, cost, bak] = g[u][i];
            if (cap && !vis[v] && dis[v] == dis[u] + cost) {
                Tp fl = dfs(v, std::min(infl, cap));
                if (fl) {
                    maxcost += fl * cost;
                    infl -= fl, outfl += fl;
                    cap -= fl, std::get<1>(g[v][bak]) += fl;
                }
            }
        }
        vis[u] = false;
        return outfl;
    }
    inline std::pair<Tp,Tp> operator()() {
        while (spfa()) maxflow += dfs(S, std::numeric_limits<Tp>::max());
        return {maxflow, maxcost};
    }
    using EdgeList = std::vector<std::tuple<int,Tp,Tp,int>>;
    EdgeList *g;
    int *head;
    Tp *dis;
    bool *vis;
    int n, S, T;
    Tp maxflow, maxcost;
};
std::string name[N];
int to[N];
/*
考虑每个点经过一次就是 1 的费用，所以拆点，内连 (1,1) 表示只能经过一次，费用是 1
S 到 1、1和n的内连、n 到 T，容量都是2表示能走两次
如果最大流=2即有解
注意这题 S->T->S 这个跑不出来，要特判
*/
int main() {
    std::cin.tie(nullptr) -> sync_with_stdio(false);
    int n, m;
    std::cin >> n >> m;
    std::map<std::string, int> mp;
    for (int i = 1; i <= n; i++) std::cin >> name[i], mp[name[i]] = i;
    int S=0, T=1;
    MaxCostMaxFlow<int> mcmf(2*n+1, S, T);
    bool directFromStartToEnd = false;
    for (int i = 1; i <= m; i++) {
        std::string frs, tos;
        std::cin >> frs >> tos;
        if (mp[frs] > mp[tos]) std::swap(frs, tos);
        if (frs == name[1] && tos == name[n]) directFromStartToEnd = true;
        mcmf(mp[frs] << 1 | 1, mp[tos] << 1, 1, 0);
    }
    mcmf(S, 1 << 1, 2, 0);
    mcmf(n << 1 | 1, T, 2, 0);
    for (int i = 2; i < n; i++) mcmf(i << 1, i << 1 | 1, 1, 1);
    mcmf(1 << 1, 1 << 1 | 1, 2, 1);
    mcmf(n << 1, n << 1 | 1, 2, 1);
    auto [mf, mc] = mcmf();
    if (mf != 2) {
        if (directFromStartToEnd) {
            std::cout << 2 << '\n'
                      << name[1] << '\n'
                      << name[n] << '\n'
                      << name[1] << '\n';
        } else std::cout << "No Solution!\n";
    } else {
        for (int i = 2; i < n; i++)
            for (const auto &[v, cap, cost, bak] : mcmf.g[i << 1 | 1])
                if (2 <= v && v <= 2*n+1 && (v & 1 ^ 1) && (v>>1) != i && !cap)
                    { to[i] = v >> 1; break; }
        int to1 = -1, to2 = -1;
        for (const auto &[v, cap, cost, bak] : mcmf.g[1 << 1 | 1])
            if (2 <= v && v <= 2*n+1 && (v>>1) != 1 && (v & 1 ^ 1) && !cap)
                { if (~to1) to2 = v >> 1; else to1 = v >> 1; }
        std::vector<int> route1, route2;
        for (int i = to1; i; i = to[i]) route1.push_back(i);
        for (int i = to2; i; i = to[i]) route2.push_back(i);
        route2.pop_back();
        std::reverse(route2.begin(), route2.end());
        std::cout << mc - 2 << '\n';
        std::cout << name[1] << '\n';
        for (int i : route1) std::cout << name[i] << '\n';
        for (int i : route2) std::cout << name[i] << '\n';
        std::cout << name[1] << '\n';
    }
    return 0;
}