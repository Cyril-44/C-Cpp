#include <cstdio>
#include <cstring>
#include <algorithm>
#include <bitset>
#include <vector>
#include <numeric>
#include <iostream>
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
std::bitset<N*N> mp, vis;
inline bool read() {
    char ch = getchar_unlocked();
    while (ch != '#' && ch != '.') ch = getchar_unlocked();
    return ch == '.';
}
inline int p2id(int x, int y) { return x * N + y; }
inline Vec2 id2p(int i) { return {i / N, i % N}; }
std::ostream& operator<<(std::ostream& os, const Vec2& o) {
    return os << '(' << o.X << ' ' << o.Y << ')';
}

struct UFS {
    int fa[N*N];
    int find(int u) { return u == fa[u] ? u : (fa[u] = find(fa[u])); }
    bool join(int u, int v) {
        u = find(u), v = find(v);
        if (u == v) return false;
        return fa[u] = v;
    }
} ufs;

std::vector<std::pair<int, Vec2>> g[N*N];
Vec2 wsum[N*N]; // 从根到当前点的树上路径和
Vec2 base[N*N]; int flg[N*N], rt; // flg[id] 表示以 id 为根的连通块内，环向量集合状态，0 表示没有，1 表示有一个，-1 表示有多于两个
void dfs(int u, int fa) {
    if (flg[rt] == -1) return;
    vis[u] = true;
    for (const auto &[v, w] : g[u]) if (v != fa) {
        if (vis[v]) {
            Vec2 ring = wsum[v] - wsum[u] - w;
            if (ring.X || ring.Y) {
                // std::cerr << "Fetched valid ring " << ring << " on node " << id2p(u) << " with root " << id2p(rt) << std::endl;
                if (!base[rt].X && !base[rt].Y) base[rt] = ring, flg[rt] = 1;
                else if (!parallel(ring, base[rt])) {
                    flg[rt] = -1; // 线性无关
                    return;
                }
            }
        } else {
            wsum[v] = wsum[u] + w;
            dfs(v, u);
            if (flg[rt] == -1) return;
        }
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
    Rep(i, n) Rep(j, m) {
        int id = p2id(i, j);
        mp[id] = read();
        flg[id] = 0;
        vis[id] = false;
        ufs.fa[id] = id;
        wsum[id] = base[id] = {};
        g[id].clear();
    }
    scanf("%d", &k);
    Rep(i, n) Rep(j, m) {
        int id = p2id(i, j);
        if (mp[id])
            for (auto drc : Dr) {
                int ti = i + drc[0], tj = j + drc[1]; Vec2 w{};
                fixCoord(ti, tj, w);
                int tid = p2id(ti, tj);
                if (mp[tid]) {
                    ufs.join(id, tid), g[id].emplace_back(tid, w);
                    // std::cerr << Vec2(i,j) << "-->" << Vec2(ti,tj) << "  " << w << std::endl;
                }
            }
    }
    Rep(i, n) Rep(j, m) {
        int id = p2id(i, j);
        if (mp[id] && ufs.fa[id] == id) dfs(rt = id, -1);
    }
    Rep(_, k) {
        int64_t sx, sy, tx, ty;
        scanf("%ld%ld%ld%ld", &sx, &sy, &tx, &ty);
        int sid = p2id(sx % n, sy % m), tid = p2id(tx % n, ty % m);
        if (sx == tx && sy == ty) puts("Bread");
        else {
            int root = ufs.find(sid);
            if (root != ufs.find(tid)) puts("Map"); // 不在同一连通块
            else if (flg[root] == -1) puts("Bread"); // 在同一连通块，且线性无关（可通往任意点）
            else {
                Vec2 delta = Vec2(tx/n - sx/n, ty/m - sy/m) - wsum[tid] + wsum[sid];
                if (!delta.X && !delta.Y) puts("Bread"); // 特殊情况，内部块可以直接到达
                else if (flg[root] == 0) puts("Map"); // 根本无法跨矩阵
                else if (parallel(delta, base[root])) puts("Bread"); // 只有一组基底，必须线性相关才行
                else puts("Map");
            }
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
