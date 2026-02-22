#include <bits/stdc++.h>
template<typename Tp>
struct MaxFlow {
    MaxFlow(int n_, int s, int t) : g(new EdgeList[n_+1]), dis(new int[n_+1]), head(new int[n_+1]), que(new int[n_+1]), n(n_), S(s), T(t), modified(false), maxflow() {}
    ~MaxFlow() { delete[] g; delete[] dis; delete[] head; }
    inline void addedg(int fr, int to, Tp c) {
        modified = true;
        g[fr].emplace_back(to, c, (int)g[to].size());
        g[to].emplace_back(fr, Tp(0), (int)g[fr].size() - 1);
    }
    inline bool bfs() {
        modified = true;
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
    int *dis, *head, *que, n, S, T;
    bool modified;
    Tp maxflow;
};
template<typename Tp>
struct MCapFlow {
    MCapFlow(int n_) : mf(n_+2, n_+1, n_+2), deg(new int[n_+1]()), n(n_), S(n_+1), T(n_+2), aval() {}
    inline int addedg(int fr, int to, Tp lwr, Tp upr) {
        mf.addedg(fr, to, upr - lwr);
        deg[fr] += lwr, deg[to] -= lwr;
        return (int)mf.g[to].size() - 1;
    }
    Tp getaval() {
        if (aval) return aval;
        for (int i = 0; i <= n; i++) // 如果 in-out > 0 连 T 否则连 S
            if (deg[i] > 0) mf.addedg(i, T, deg[i]);
            else if (deg[i] < 0) mf.addedg(S, i, -deg[i]);
        aval = mf();
        for (const auto &[v, cap, bak] : mf.g[S])
            if (cap) return -1; // S 到 x 必须满流
        return aval;
    }
    Tp getaval(int s, int t) {
        if (aval) return aval;
        int id = addedg(t, s, static_cast<Tp>(0), std::numeric_limits<Tp>::max()); // 加入 t-->s [0, +oo] 的边
        if (getaval() == -1) return -1;
        return std::get<1>(mf.g[s][id]); // 此时的可行流是 t-->s 的流量
    }
    Tp getmx(int s, int t) {
        if (aval) throw;
        addedg(t, s, static_cast<Tp>(0), std::numeric_limits<Tp>::max()); // 加入 t-->s [0, +oo] 的边
        int idt = (int)mf.g[t].size() - 1, ids = (int)mf.g[s].size() - 1;
        if (getaval() == -1) return -1;
        int res = std::get<1>(mf.g[s][ids]); // 此时的可行流是 t-->s 的流量
        std::get<1>(mf.g[t][idt]) = std::get<1>(mf.g[s][ids]) = static_cast<Tp>(0); // 删除 t-->s的边
        mf.S = s, mf.T = t, mf.maxflow = 0, mf.modified = true;
        return res + mf();
    }
    Tp getmn(int s, int t) {
        if (aval) throw;
        addedg(t, s, static_cast<Tp>(0), std::numeric_limits<Tp>::max()); // 加入 t-->s [0, +oo] 的边
        int idt = (int)mf.g[t].size() - 1, ids = (int)mf.g[s].size() - 1;
        if (getaval() == -1) return -1;
        int res = std::get<1>(mf.g[s][ids]); // 此时的可行流是 t-->s 的流量
        std::get<1>(mf.g[t][idt]) = std::get<1>(mf.g[s][ids]) = static_cast<Tp>(0); // 删除 t-->s的边
        mf.S = t, mf.T = s, mf.maxflow = 0, mf.modified = true;
        return res - mf();
    }
    MaxFlow<Tp> mf;
    std::vector<std::tuple<int,int,Tp,Tp>> edgs;
    Tp *deg; // deg 记录的是 入lwr - 出lwr
    int n, S, T;
    Tp aval;
    MaxFlow<Tp>* operator->() { return &mf; }
};
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    MCapFlow<int> mcf(n);
    std::vector<std::tuple<int, int, int>> edgs;
    for (int u, v, l, r; m--; ) {
        scanf("%d%d%d%d", &u, &v, &l, &r);
        edgs.emplace_back(l, v, mcf.addedg(u, v, l, r));
    }
    if (mcf.getaval() == -1) puts("No");
    else {
        puts("Yes");
        for (const auto &[base, v, i] : edgs)
            printf("%d\n", base + std::get<1>(mcf->g[v][i]));
    }
    return 0;
}