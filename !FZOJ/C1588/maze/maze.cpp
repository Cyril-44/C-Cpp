#include <cstdio>
#include <cstring>
#include <vector>
#include <bitset>
#include <queue>
#include <numeric>
constexpr int N = 1001;
char mp[N][N];
int n, m;
std::vector<int> g[N*N], ig[N*N];
inline int id(int x, int y) { return (x-1) * m + y-1; }
static int que[N*N];
template<size_t N> inline void bfs(std::bitset<N>& vis, std::vector<int> g[], int u) {
    int hd=0, tl=0;
    que[tl++] = u;
    vis[u] = true;
    while (hd < tl) {
        u = que[hd++];
        for (int v : g[u]) if (!vis[v])
            vis[v] = true, que[tl++] = v;
    }
}
template<size_t N> inline void bf(int a, int b, int c, int d) {
    std::bitset<N> bs1, bs2;
    bfs(bs1, g, id(a, b));
    if (!bs1[id(c, d)]) puts("-1");
    else {
        bfs(bs2, ig, id(c, d));
        printf("%zu\n", (bs1 & bs2).count());
    }
}
struct DSU {
    std::vector<int> fa;
    DSU(int n) : fa(n) { std::iota(fa.begin(), fa.end(), 0); }
    int find(int u) { return u == fa[u] ? u : (fa[u] = find(fa[u])); }
    bool unite(int u, int v) {
        u = find(u), v = find(v);
        if (u == v) return false;
        return fa[u] = v, true;
    }
};
int main() {
    int tid, Q;
    scanf("%d%d%d%d", &tid, &n, &m, &Q);
    for (int i = 1; i <= n; i++) {
        scanf("%s", mp[i] + 1);
        for (int j = 1; j <= m; j++) {
            if (i-1 >= 1 && mp[i][j] != 'U') g[id(i,j)].push_back(id(i-1,j)), ig[id(i-1,j)].push_back(id(i,j));
            if (j-1 >= 1 && mp[i][j] != 'L') g[id(i,j)].push_back(id(i,j-1)), ig[id(i,j-1)].push_back(id(i,j));
            if (i+1 <= n && mp[i][j] != 'D') g[id(i,j)].push_back(id(i+1,j)), ig[id(i+1,j)].push_back(id(i,j));
            if (j+1 <= m && mp[i][j] != 'R') g[id(i,j)].push_back(id(i,j+1)), ig[id(i,j+1)].push_back(id(i,j));
        }
    }
    if (tid == 3) {
        DSU dsu(n*m);
        return 0;
    }
    while (Q--) {
        int a, b, c, d;
        scanf("%d%d%d%d", &a, &b, &c, &d);
        if (tid == 1) bf<300*300>(a, b, c, d);
        else if (tid == 2) bf<1000>(a, b, c, d);
    }
    return 0;
}