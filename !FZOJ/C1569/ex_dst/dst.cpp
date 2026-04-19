#include <cassert>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
#include <tuple>
#include <limits>
#include <queue>
constexpr int N = 205, M = 1005;
std::tuple<int,int,int> qmods[N];
std::pair<int,int> qasks[N];
enum Type { NONE, MOD, ASK } vis[M];
int sum[N][M];
int multiply[M];

constexpr static int INF = 0x80808080;
struct MaxCostMaxFlow {
    inline bool bfs() {
        dis.assign(n+1, INF);
        vis.assign(n+1, false);
        head.assign(n+1, 0);
        dis[S] = 0;
        for (std::queue<int> q({S}); !q.empty(); q.pop()) {
            int u = q.front();
            vis[u] = false;
            for (const auto &x : g[u]) {
                int v, cap, cost, bak;
                std::tie(v, cap, cost, bak) = x;
                if (cap && dis[v] < dis[u] + cost) {
                    dis[v] = dis[u] + cost;
                    if (!vis[v]) q.push(v), vis[v] = true;
                }
            }
        }
        return dis[T] != INF;
    }
    int dfs(int u, int infl) {
        vis[u] = true;
        if (u == T) return infl;
        int outfl = 0;
        for (int &i = head[u]; i < (int)g[u].size(); i++) {
            int v, cap, cost, bak;
            std::tie(v, cap, cost, bak) = g[u][i];
            if (!vis[v] && cap && dis[v] == dis[u] + cost) {
                int fl = dfs(v, std::min(infl, cap));
                if (fl) {
                    mxcs += cost * fl;
                    infl -= fl, outfl += fl;
                    std::get<1>(g[u][i]) -= fl, std::get<1>(g[v][bak]) += fl;
                    if (!infl) return outfl;
                } else dis[v] = INF;
            }
        }
        return outfl;
    }
    inline void add(int fr, int to, int cp, int cst) {
        g[fr].emplace_back(to, cp, cst, g[to].size());
        g[to].emplace_back(fr, 0, -cst, g[fr].size() - 1);
    }
    inline std::pair<int,int> calc() {
        if (mxfl) return {mxfl, mxcs};
        while (bfs()) {
            do vis.assign(n+1, false), mxfl += dfs(S, std::numeric_limits<int>::max());
            while (vis[T]);
        }
        return {mxfl, mxcs};
    }
    inline MaxCostMaxFlow(int n, int s, int t) : g(n+1), n(n), S(s), T(t), mxfl(), mxcs() {}
    std::vector<std::vector<std::tuple<int,int,int,int>>> g;
    std::vector<int> head, dis;
    std::vector<bool> vis;
    int n, S, T, mxfl, mxcs;
};

inline int calc(int i, const std::tuple<int,int,int> &t) {
    return std::get<2>(t) * (sum[i][std::get<1>(t)] - sum[i][std::get<0>(t)-1]);
}
inline int calc(int i, const std::pair<int,int> &t) {
    return sum[i][t.second] - sum[i][t.first-1];
}
inline void update(const std::tuple<int,int,int> &t) {
    for (int i = std::get<0>(t); i <= std::get<1>(t); i++)
        multiply[i] += std::get<2>(t);
}
inline int calc(const std::pair<int,int> &p) {
    int res = 0;
    for (int i = p.first; i <= p.second; i++)
        res += multiply[i];
    return res;
}
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    std::vector<std::tuple<int,int,int>> tmods;
    std::vector<std::pair<int,int>> tasks;
    for (int i = 1; i <= n; i++) {
        int type, l, r, w, pos;
        scanf("%d%d%d", &type, &l, &r);
        if (type == 1) scanf("%d", &w);
        scanf("%d", &pos);
        if (pos) {
            vis[pos] = Type(type);
            if (type == 1) qmods[pos] = {l, r, w};
            else qasks[pos] = {l, r};
        } else {
            if (type == 1) tmods.emplace_back(l, r, w);
            else tasks.emplace_back(l, r);
        }
    }
    
    std::vector<int> nones;
    for (int i = 1; i <= n; i++)
        if (vis[i] == NONE) nones.push_back(i);

    { // 前面的修改单独计算
        for (int i = n; i >= 1; i--) {
            memcpy(sum[i], sum[i+1], sizeof(int) * (m+1));
            if (vis[i] == ASK)
                for (int j = qasks[i].first; j <= qasks[i].second; j++) ++sum[i][j];
        }
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= m; j++)
                sum[i][j] += sum[i][j-1];
        int S = tmods.size()*2, T = tmods.size()*2+1;
        MaxCostMaxFlow mcmf(T, S, T);
        for (int i = 0; i < (int)tmods.size(); i++)
            for (int j = 0; j < (int)tmods.size(); j++)
                mcmf.add(i<<1, j<<1|1, 1, calc(nones[j], tmods[i]));
        for (int i = 0; i < (int)tmods.size(); i++)
            mcmf.add(S, i<<1, 1, 0),
            mcmf.add(i<<1|1, T, 1, 0);
        assert(mcmf.calc().first == (int)tmods.size());
        for (int i = 0; i < (int)tmods.size(); i++) {
            bool flg = false;
            for (const auto &x : mcmf.g[i<<1]) {
                int v, cap, cost, bak;
                std::tie(v, cap, cost, bak) = x;
                if (cost >= 0 && cap == 0 && v < (int)tmods.size()*2) {
                    qmods[nones[v>>1]] = tmods[i];
                    vis[nones[v>>1]] = MOD;
                    flg = true; break;
                }
            }
            if (!flg) throw;
        }
    }
    
    nones.erase(nones.begin(), nones.begin() + tmods.size());
    
    { // 后面的查询单独计算
        for (int i = 1; i <= n; i++) {
            memcpy(sum[i], sum[i-1], sizeof(int) * (m+1));
            if (vis[i] == MOD) {
                int l, r, w;
                std::tie(l, r, w) = qmods[i];
                for (int j = l; j <= r; j++) sum[i][j] += w;
            }
        }
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= m; j++)
                sum[i][j] += sum[i][j-1];
        int S = tasks.size()*2, T = tasks.size()*2+1;
        MaxCostMaxFlow mcmf(T, S, T);
        for (int i = 0; i < (int)tasks.size(); i++)
            for (int j = 0; j < (int)tasks.size(); j++)
                mcmf.add(i<<1, j<<1|1, 1, calc(nones[j], tasks[i]));
        for (int i = 0; i < (int)tasks.size(); i++)
            mcmf.add(S, i<<1, 1, 0),
            mcmf.add(i<<1|1, T, 1, 0);
        assert(mcmf.calc().first == (int)tasks.size());
        for (int i = 0; i < (int)tasks.size(); i++) {
            bool flg = false;
            for (const auto &x : mcmf.g[i<<1]) {
                int v, cap, cost, bak;
                std::tie(v, cap, cost, bak) = x;
                if (cost >= 0 && cap == 0 && v < (int)tasks.size()*2) {
                    qasks[nones[v>>1]] = tasks[i];
                    vis[nones[v>>1]] = ASK;
                    flg = true; break;
                }
            }
            if (!flg) throw;
        }
    }
    int ans = 0;
    for (int i = 1; i <= n; i++)
        if (vis[i] == MOD)
            update(qmods[i]);
        else if (vis[i] == ASK)
            ans += calc(qasks[i]);
        else throw;
    printf("%d\n", ans);
    return 0;
}