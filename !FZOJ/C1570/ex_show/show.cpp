#include <cstdio>
#include <cstdint>
#include <cassert>
#include <cstring>
#include <algorithm>
#include <map>
constexpr int N = 50005, M = 100005;
struct Point {
    int x, y, v;
    inline bool operator<(const Point &o) const { return x + y < o.x + o.y; }
} pnts[N << 1];
int64_t sum[M];
int n;
/*
考虑只有 Pos_b < Pos_c 或者 Pos_d < Pos_a 的时候 p 才会有负贡献。
设置 F[i][j] 表示当前已经选了前 i 个红色和前 j 个紫色的位置，产生的最小负贡献。
经典 trick：考虑将这个状态用平面直角坐标系去刻画，发现实质上我们要弄一条 (0,0) 到 (n,m) 的路径。
发现 (b,c) 对应的点如果有贡献一定在最终路径的上方，(a,d) 则是下方。
    有题目保证 a<b 且 c<d，我们知道 (b,c) 在 (a,d) 的左上方，不存在选了 (b,c) 没选 (a,d) 的情况。
    所以我们直接设置 (a,d) 权值为 p，(b,c) 权值为 
转移显然是 F[i][j] = min(F[i-1][j], F[i][j-1]) + A[i][j]，其中 A 是点的权值。
*/
struct SegTr {
    struct Node { int64_t min, cov, add; } tr[N << 2];
    int P, L, R;
    int64_t X;
    inline void pushdown(int u) {
        if (tr[u].cov) {
            tr[u<<1].min = tr[u<<1|1].min =
            tr[u<<1].cov = tr[u<<1|1].cov = tr[u].cov;
            tr[u<<1].add = tr[u<<1|1].add = 0;
            tr[u].cov = 0;
        }
        if (tr[u].add) {
            tr[u<<1].add += tr[u].add, tr[u<<1|1].add += tr[u].add;
            tr[u<<1].min += tr[u].add, tr[u<<1|1].min += tr[u].add;
            tr[u].add = 0;
        }
    }
    inline void pushup(int u) {
        tr[u].min = tr[u<<1].min + tr[u<<1|1].min;
    }
    void add(int u, int l, int r) {
        if (L <= l && r <= R) tr[u].min += X, tr[u].add += X;
        else {
            int mid = l + r >> 1;
            pushdown(u);
            if (L <= mid) add(u<<1, l, mid);
            if (mid < R) add(u<<1|1, mid + 1, r);
            pushup(u);
        }
    }
    void cov(int u, int l, int r) {
        if (L <= l && r <= R) tr[u].min = X, tr[u].cov = X, tr[u].add = 0;
        else {
            int mid = l + r >> 1;
            pushdown(u);
            if (L <= mid) cov(u<<1, l, mid);
            if (mid < R) cov(u<<1|1, mid + 1, r);
            pushup(u);
        }
    }
    int64_t que(int u, int l, int r) {
        if (l == r) return tr[u].min;
        int mid = l + r >> 1;
        pushdown(u);
        if (P <= mid) return que(u<<1, l, mid);
        return que(u<<1|1, mid + 1, r);
    }
    inline void update(int l, int r, int x, bool mode) {
        L=l, R=r, X=x;
        if (mode) cov(1, 1, n);
        else add(1, 1, n);
    }
    inline int64_t inquire(int p, int x) {
        P=p, X=x;
        return que(1, 1, n);
    }
};
std::map<int,int> v[N];
int main() {
    int m, k;
    scanf("%d%d%d", &n, &m, &k);
    for (int i = 1; i <= k; i++) {
        int a, b, c, d, p;
        scanf("%d%d%d%d%d", &a, &b, &c, &d, &p);
        v[d][a] += p;
        v[b][c] -= p;
    }
    
    int64_t ans;
    printf("%ld\n", ans);
    return 0;
}