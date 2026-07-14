#include <cstdio>
#include <cstring>
#include <algorithm>
#include <bitset>
#include <vector>
#include <numeric>
constexpr int N = 2000;
#define Rep(i, n) for (int i = 0; i < n; i++)
using Vec2 = std::pair<int,int>;
#define X first
#define Y second

inline Vec2 operator+(Vec2 a, Vec2 b) { return Vec2(a.X+b.X, a.Y+b.Y); }
inline Vec2 operator-(Vec2 a) { return Vec2(-a.X, -a.Y); }
inline Vec2 operator-(Vec2 a, Vec2 b) { return Vec2(a.X-b.X, a.Y-b.Y); }
inline Vec2& operator+=(Vec2& a, Vec2 b) { a.X+=b.X, a.Y+=b.Y; return a; }
inline Vec2& operator-=(Vec2& a, Vec2 b) { a.X-=b.X, a.Y-=b.Y; return a; }
inline bool parallel(Vec2 a, Vec2 b) { return 1ll * a.X * b.Y == 1ll * a.Y * b.X; }

constexpr int Dr[4][2]{{-1,0},{0,1},{1,0},{0,-1}};
std::bitset<N*N> mp, flg; // flg[id] 表示以 id 为根的连通块内，环向量集合是否线性无关。
inline bool read() {
    char ch = getchar_unlocked();
    while (ch != '#' && ch != '.') ch = getchar_unlocked();
    return ch == '.';
}
inline int p2id(int x, int y) { return x * N + y; }
inline Vec2 id2p(int i) { return {i / N, i % N}; }

struct UFS {
    int fa[N];
    void init(int n, int m) { Rep(i, n) std::iota(fa+i*N, fa+i*N+m, i*N); }
    int find(int u) { return u == fa[u] ? u : (fa[u] = find(fa[u])); }
    bool join(int u, int v) {
        u = find(u), v = find(v);
        if (u == v) return false;
        return fa[u] = v;
    }
} ufs;

std::vector<std::pair<int, Vec2>> g[N];
Vec2 wsum[N*N]; // 从根到当前点的树上路径和
Vec2 now; int rt;
void dfs(int u, int fa) {
    if (flg[rt]) return;
    if (wsum[u].X || wsum[u].Y) {
        if (!now.X && !now.Y) now = wsum[u];
        else if (!parallel(wsum[u], now)) flg[rt] = true; // 线性无关
    }
    for (const auto &[v, w] : g[u]) if (v != fa) {
        wsum[v] = wsum[u] + w;
        dfs(v, u);
    }
}

/* 考虑使用向量(x,y)描述从一个点到另外一个点跨越的矩阵形态（横着多少，竖着多少），
我们对矩阵内的联通点建边(0,0)，两边的跨越边建(1,0)(0,1)(-1,0)(0,-1)其中之一。
然后把这个图上的所有环上的权值和提出来形成向量集合（具体做法是，基本环只需要 树边+一条非树边 就可以直接得到）
 提出来，看看是否都是线性相关（都平行），如果不是，那么这个连通块里面所有点，在坐标系上任何位置都随便走。
否则，我们把矩阵跳跃的那个差值向量弄出来，看看是不是和这个连通块的向量线性相关，是的话就能到，否则不能。 */
inline void solveSingle() {
    int n, m, k;
    scanf("%d%d", &n, &m);
    auto fixCoord = [n, m](int &x, int &y, Vec2 &w) {
        if (x < 0) x = n-1, w.X = -1;
        else if (x >= n) x = 0, w.X = 1;
        if (y < 0) y = m-1, w.Y = -1;
        else if (y >= m) y = 0, w.Y = 1;
    };
    ufs.init(n, m);
    Rep(i, n) Rep(j, m) if(read()) mp.set(p2id(i, j));
    scanf("%d", &k);
    Rep(i, n) Rep(j, m) {
        int id = p2id(i, j);
        if (mp[id])
            for (auto drc : Dr) {
                int ti = i + drc[0], tj = j + drc[1]; Vec2 w;
                fixCoord(ti, tj, w);
                int tid = p2id(ti, tj);
                if (mp[tid]) ufs.join(id, tid), g[id].emplace_back(tid, w);
            }
    }
    Rep(i, n) Rep(j, m) {
        int id = p2id(i, j);
        if (ufs.fa[id] == id) dfs(id, 0);
    }
    Rep(_, k) {
        int sx, sy, tx, ty;
        scanf("%d%d%d%d", &sx, &sy, &tx, &ty);
        int sid = p2id(sx, sy), tid = p2id(tx, ty);
        if (sid == tid) puts("Bread");
        else if (ufs.find(sid) != ufs.find(tid)) puts("Map");
        else {
            Vec2 delta = Vec2(tx/n - sx/n, ty/n - sy/n);
        }
    }
}
int main() {
    freopen("map.in", "r", stdin);
    freopen("map.out", "w", stdout);
    int T;
    scanf("%d", &T);
    while (T--) solveSingle();
    return 0;
}
