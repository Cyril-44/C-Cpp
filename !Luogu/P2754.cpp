#include <bits/stdc++.h>
constexpr int N = 15, M = 25, K = 55;
template<typename Tp>
struct MaxFlow {
    MaxFlow(int n_, int s, int t) : g(n_+1), dis(n_+1), head(n_+1), n(n_), S(s), T(t), maxflow() {}
    inline void addedg(int fr, int to, Tp c) {
        g[fr].emplace_back(to, c, (int)g[to].size());
        g[to].emplace_back(fr, Tp(0), (int)g[fr].size() - 1);
    }
    inline bool bfs() {
        static int que[N * M * K];
        memset(dis.data(), 0, sizeof(int) * (n+1));
        memset(head.data(), 0, sizeof(int) * (n+1));
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
        while (bfs()) maxflow += dfs(S, std::numeric_limits<Tp>::max());
        return maxflow;
    }
    inline void resize(int n_) { n = n_; g.resize(n+1), dis.resize(n+1), head.resize(n+1); }
    using EdgeList = std::vector<std::tuple<int, Tp, int>>;
    std::vector<EdgeList> g;
    std::vector<int> dis, head;
    int n, S, T;
    Tp maxflow;
};
int fa[N + 1];
inline int find(int x) { return x ^ fa[x] ? (fa[x] = find(fa[x])) : x; }
inline void unite(int x, int y) {
    x = find(x), y = find(y);
    if (x ^ y) fa[x] = y;
}
struct SpaceShip {
    int cap, ptr;
    std::vector<int> arr;
    inline void input() {
        int n;
        scanf("%d%d", &cap, &n);
        arr.resize(n);
        for (int &i : arr) scanf("%d", &i);
        int last = arr.back();
        for (int i : arr) unite(last + 2, i + 2), last=i;
        ptr = 0;
    }
    inline std::pair<int,int> operator()() {
        int tp = ptr;
        if ((++ptr) == (int)arr.size()) ptr = 0;
        return {arr[tp], arr[ptr]};
    }
} ship[M];
int main() {
    int n, m, k;
    scanf("%d%d%d", &n, &m, &k);
    for (int i = 1; i <= n + 2; i++) fa[i] = i;
    for (int i = 1; i <= m; i++) ship[i].input();
    constexpr int EARTH = 2, MOON = 1;
    if (find(EARTH) != find(MOON)) return puts("0"), 0;
    int prefix = 2;
    MaxFlow<int> mf(n+2, EARTH, MOON);
    for (int i = 1; ; i++, prefix += n) { // 按照时间 i 分层建图，每一层 n 个点，上下层用 +oo 的边连接。
        mf.resize(prefix + (n << 1));
        for (int j = 1; j <= n; j++) {
            mf.addedg(j + prefix, j + prefix + n, std::numeric_limits<int>::max());
        }
        for (int j = 1; j <= m; j++) {
            auto [fr, to] = ship[j]();
            fr += fr > 0 ? prefix     : 2; // 如果是地球月球直接搞成源点和汇点
            to += to > 0 ? prefix + n : 2;
            if (fr != MOON && to != EARTH) mf.addedg(fr, to, ship[j].cap);
        }
        if (mf() >= k) return printf("%d\n", i), 0;
    }
    return 0;
}
