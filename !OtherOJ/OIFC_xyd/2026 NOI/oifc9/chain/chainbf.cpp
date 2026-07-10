#include <vector>
#include <algorithm>
#include <fstream>
#include <cstring>
#include <queue>
#include <cstdint>
constexpr int N = 500005, M = 1000005, Q = 100005;
using Pii = std::pair<int,int>;
using Pli = std::pair<int64_t, int>;
#define For(i, s, t) for (int i = (s); i <= (t); i++)
struct Data {
    int u, v, w, len;
    friend bool operator<(const Data &x, const Data &y) { return x.len < y.len; }
} edg[M];
struct Query {
    int l, r, id;
    friend bool operator<(const Query &x, const Query &y) { return x.l < y.l; }
} que[Q];
int64_t ans[Q];
int n, m, q;
struct Sub1 {
    static constexpr int N = 305;
    int64_t f[N][N];
    void operator()() {
        memset(f, 0x3f, sizeof f);
        For(i,1,m) f[edg[i].u][edg[i].v] = f[edg[i].v][edg[i].u] = edg[i].w;
        For(k,1,n) For(i,1,n) For(j,1,n) if(i!=j&&i!=k&&j!=k)
            f[i][j] = std::min(f[i][j], f[i][k] + f[k][j]);
        For(i,1,q) ans[que[i].id] = f[que[i].l][que[i].r];
    }
};
namespace Sub2 {
    std::vector<Pii> g[N];
    int64_t dis[N];
    bool vis[N];
    inline void dij(int s) {
        std::priority_queue<Pli, std::vector<Pli>, std::greater<Pli>> pq;
        pq.emplace(0, s);
        memset(dis, 0x3f, sizeof dis);
        memset(vis, 0, sizeof vis);
        dis[s] = 0;
        while (!pq.empty()) {
            auto [wu, u] = pq.top(); pq.pop();
            if (vis[u]) continue;
            vis[u] = true;
            for (auto [v, w] : g[u])
                if (!vis[v] && dis[v] > dis[u] + w) {
                    dis[v] = dis[u] + w;
                    pq.emplace(dis[v], v);
                }
        }
    }
};
namespace Sub3 {
    struct BIT {
        int tr[N]{};
        void upd(int p, int x) { for (; p <= n; p += p & -p) tr[p] += x; }
        int sum(int p) { int res = 0; for (; p > 0; p -= p & -p) res += tr[p]; return res; }
    } f;
    int sum1[N], sum2[N];
    inline void work() {
        For(i, 1, m) sum1[edg[i].v]++;
        For(i, 1, n) sum1[i] += sum1[i-1];
        std::sort(edg+1, edg+1+m);
        For(i, 1, m) {
            sum2[edg[i].v] += f.sum(edg[i].v) - f.sum(edg[i].u-1);
            f.upd(edg[i].v, -sum2[edg[i].v] + 1);
        }
        For(i, 1, n) sum2[i] += sum2[i-1];
        For(i, 1, q) ans[que[i].id] = (sum1[que[i].r] - sum1[que[i].l-1]) - (sum2[que[i].r] - sum2[que[i].l-1]);
    }
};
int main() {
    std::ifstream fin("chain.in");
    std::ofstream fout("chain.out");
    fin >> n >> m >> q;
    bool w1 = true;
    For(i, 1, m)  {
        fin >> edg[i].u >> edg[i].v >> edg[i].w;
        if (edg[i].u > edg[i].v) std::swap(edg[i].u, edg[i].v);
        if (edg[i].w != 1) w1 = false;
        edg[i].len = edg[i].v - edg[i].u;
    }
    for (int i = 1; i <= q; i++) {
        fin >> que[i].l >> que[i].r;
        if (que[i].l > que[i].r) std::swap(que[i].l, que[i].r);
        que[i].id = i;
    }
    if (n <= 300) Sub1{}();
    else if (q <= 150) {
        For(i, 1, m) {
            Sub2::g[edg[i].u].emplace_back(edg[i].v, edg[i].w);
            Sub2::g[edg[i].v].emplace_back(edg[i].u, edg[i].w);
        }
        std::sort(que+1, que+1+q);
        for (int i = 1, p; i <= q; i = p) {
            int x = que[i].l;
            Sub2::dij(x);
            for (p = i; p <= q && que[p].l == que[i].l; p++)
                ans[que[p].id] = Sub2::dis[que[p].r];
        }
    }
    for (int i = 1; i <= q; i++)
        fout << ans[i] << '\n';
    return 0;
}
