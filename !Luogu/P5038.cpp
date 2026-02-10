#include <stdio.h>
#include <string.h>
#include <vector>
#include <tuple>
#include <limits>
constexpr int N = 45;
using LL = long long;
constexpr std::pair<int,int> Dir[]{{-1,0},{0,1},{1,0},{0,-1}};
int a[N][N];
/*
如果最终全部变成 x,
S 向 黑点连边，x - aij
黑点向白点连边，+oo
白点向 T 连边，x - aij
如果能跑满流就是合法
*/
namespace FastI {
    char buf[1 << 20], *p1{}, *p2{};
    [[gnu::always_inline]] inline char gc() {
        if (__builtin_expect(p1 == p2, 0)) p2 = (p1 = buf) + fread(buf, 1, sizeof buf, stdin);
        return *p1++;
    }
    [[gnu::always_inline]] inline void in(int &x) {
        char ch = gc();
        while (ch < '0' || ch > '9') ch = gc();
        for (x = 0; ch >= '0' && ch <= '9'; ch = gc())
            x = (x << 3) + (x << 1) + (ch ^ '0');
    }
} using FastI::in;
using EdgeList = std::vector<std::tuple<int, LL, int>>;
EdgeList g[N*N];
int dis[N*N], head[N*N], que[N*N];
struct MaxFlow {
    int n, S, T;
    LL maxflow;
    MaxFlow(int n_, int s, int t) : n(n_), S(s), T(t), maxflow() { for (int i = 1; i <= n_; i++) g[i].clear(); }
    inline void addedg(int fr, int to, LL c) {
        g[fr].emplace_back(to, c, (int)g[to].size());
        g[to].emplace_back(fr, 0ll, (int)g[fr].size() - 1);
    }
    inline bool bfs() {
        memset(dis, 0, sizeof(int) * (n+1));
        memset(head, 0, sizeof(int) * (n+1));
        int hd = 0, tl = 0;
        que[tl++] = S; dis[S] = 1;
        while (hd ^ tl) {
            int u = que[hd++];
            for (const auto &[v, cap, bak] : g[u])
                if (cap && !dis[v]) {
                    dis[v] = dis[u] + 1;
                    if (v == T) return true;
                    que[tl++] = v;
                }
        }
        return false;
    }
    LL dfs(int u, LL infl) {
        if (u == T) return infl;
        LL outfl{};
        for (int &i = head[u]; i ^ (int)(g[u].size()); i++) {
            auto &[v, cap, bak] = g[u][i];
            if (cap && dis[v] == dis[u] + 1) {
                LL fl = dfs(v, std::min(cap, infl));
                if (fl) {
                    infl -= fl, outfl += fl;
                    cap -= fl, std::get<1>(g[v][bak]) += fl;
                } else dis[v] = -1;
            }
        }
        return outfl;
    }
    inline LL operator()() {
        if (maxflow) return maxflow;
        while (bfs()) maxflow += dfs(S, std::numeric_limits<LL>::max());
        return maxflow;
    }
};
int main() {
    int T, n, m;
    in(T);
    while (T--) {
        in(n), in(m);
        int mx = 0;
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= m; j++) {
                in(a[i][j]);
                mx = std::max(mx, a[i][j]);
            }
        int nm = n * m >> 1;
        LL black = 0, white = 0;
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= m; j++)
                ((i & 1) ^ (j & 1) ? white : black) += a[i][j];
        auto check = [&](LL x) {
#define mapper(x, y) ((x - 1) * m + y)
            int S = n * m + 1, T = S + 1;
            MaxFlow mf(T, S, T);
            for (int i = 1; i <= n; i++)
                for (int j = 1; j <= m; j++)
                    ((i&1) ^ (j&1)) ? 
                    mf.addedg(mapper(i, j), T, x - a[i][j]) : 
                    mf.addedg(S, mapper(i, j), x - a[i][j]);
            for (int i = 1; i <= n; i++)
                for (int j = 1; j <= m; j++)
                    if ((i&1) ^ (j&1))
                        for (int k = 0; k ^ 4; k++) {
                            int tx = i + Dir[k].first, ty = j + Dir[k].second;
                            if (1 <= tx && tx <= n && 1 <= ty && ty <= m)
                                mf.addedg(mapper(tx, ty), mapper(i, j), x);
                        }
#undef mapper
            // fprintf(stderr, "Working on %lld, expected %d*%lld-%lld, found %lld%s\n", x, nm, x, white, mf(), mf() == nm * x - white ? " (Equal)" : "");
            return mf() == nm * x - white;
        };
        // fprintf(stderr, "=========\n");
        if ((n&1) && (m&1)) {
            LL x = black - white;
            // fprintf(stderr, "%lld\n", x);
            x >= mx && check(x) ? printf("%lld\n", x * n * m - black - white >> 1) : puts("-1");
        }
        else if (black != white) puts("-1");
        else {
            LL l = mx, r = 1e13, mid;
            while (l <= r) {
                mid = l + r >> 1;
                if (check(mid)) r = mid - 1;
                else l = mid + 1;
            }
            printf("%lld\n", l > (LL)1e13 ? -1 : l * n * m - black - white >> 1); 
        }
    }
    return 0;
}