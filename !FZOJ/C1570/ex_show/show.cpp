#include <cstdio>
#include <cstdint>
#include <cassert>
#include <cstring>
#include <random>
#include <algorithm>
#include <map>
constexpr int N = 1000005;
struct Point {
    int x, y, v;
    inline bool operator<(const Point &o) const { return x + y < o.x + o.y; }
} pnts[N << 1];
/*
考虑只有 Pos_b < Pos_c 或者 Pos_d < Pos_a 的时候 p 才会有负贡献。
设置 F[i][j] 表示当前已经选了前 i 个红色和前 j 个紫色的位置，产生的最小负贡献。
经典 trick：考虑将这个状态用平面直角坐标系去刻画，发现实质上我们要弄一条 (0,0) 到 (n,m) 的路径。
发现 (b,c) 对应的点如果有贡献一定在最终路径的上方，(a,d) 则是下方。
    有题目保证 a<b 且 c<d，我们知道 (b,c) 在 (a,d) 的左上方，不存在选了 (b,c) 没选 (a,d) 的情况。
    所以我们直接设置 (a,d) 权值为 p，(b,c) 权值为 -p。
换一种方式去理解这个 DP，考虑 j 是高度，那么有 F[i][j] = min(F[i-1][1]...F[i-1][j]) + Sum[i][j]，其中 Sum[i][j] 是 sum_{k=1}^j A[i][k]。
那么我们只需要每个转移做一个 Prefix-Min，然后去加上前缀和即可。
直接暴力 Prefix-Min，暴力加即可。
可以使用线段树二分去模拟 Treap 的 lower_bound 分裂。
*/

struct Treap {
    constexpr static int64_t COV_DFT = (1ll << 62) - 1;
    std::mt19937 rng;
    int totcnt;
    Treap() : rng(std::random_device{}()), totcnt(), rt() {}
    struct Node {
        int ls, rs, cnt;
        int64_t val, lmx, rmn, add, cov;
        uint64_t weight;
        inline void pull(int64_t p, int64_t c) {
            if (c != COV_DFT) val = lmx = rmn = cov = c, add = 0;
            if (p) val += p, lmx += p, rmn += p, add += p;
        }
    } tr[N << 2];
    int rt;
    inline void pushup(int u) {
        tr[u].cnt = tr[tr[u].ls].cnt + 1 + tr[tr[u].rs].cnt;
        tr[u].lmx = tr[u].ls ? tr[tr[u].ls].lmx : tr[u].val;
        tr[u].rmn = tr[u].rs ? tr[tr[u].rs].rmn : tr[u].val;
    }
    inline void pushdown(int u) {
        if (tr[u].add == 0 && tr[u].cov == COV_DFT) return;
        if (tr[u].ls) tr[tr[u].ls].pull(tr[u].add, tr[u].cov);
        if (tr[u].rs) tr[tr[u].rs].pull(tr[u].add, tr[u].cov);
        tr[u].add = 0, tr[u].cov = COV_DFT;
    }
    inline int alloc() {
        tr[++totcnt] = { 0,0, 1, 0ll,0ll,0ll,0ll, COV_DFT, rng() };
        return totcnt;
    }
    inline void push_back() {
        rt = merge(rt, alloc());
    }
    int merge(int u, int v) {
        if (!u || !v) return u | v;
        if (tr[u].weight < tr[v].weight)
            return pushdown(u), tr[u].rs = merge(tr[u].rs, v), pushup(u), u;
        return pushdown(v), tr[v].ls = merge(u, tr[v].ls), pushup(v), v;
    }
    std::pair<int,int> lower_bound(int u, int64_t w) { // Find the first position to insert w, and split into 2 parts
        if (tr[u].lmx <= w) return {0, u};
        if (tr[u].rmn > w) return {u, 0};
        pushdown(u);
        if (w > tr[u].val) {
            auto ret = lower_bound(tr[u].ls, w);
            tr[u].ls = ret.second;
            pushup(u);
            return {ret.first, u};
        }
        auto ret = lower_bound(tr[u].rs, w);
        tr[u].rs = ret.first;
        pushup(u);
        return {u, ret.second};
    }
    std::pair<int,int> split(int u, int w) { // Split the tree to {lsize=w, r}
        if (w == 0) return {0, u};
        if (tr[u].cnt == w) return {u, 0};
        // if (tr[u].cnt < w) assert(0);
        pushdown(u);
        if (tr[tr[u].ls].cnt >= w) {
            auto ret = split(tr[u].ls, w);
            tr[u].ls = ret.second;
            pushup(u);
            return {ret.first, u};
        }
        auto ret = split(tr[u].rs, w - tr[tr[u].ls].cnt - 1);
        tr[u].rs = ret.first;
        pushup(u);
        return {u, ret.second};
    }
    inline int64_t inquireLast(int u) { return tr[u].rmn; }
    void debug(int u) {
        if (!u) return;
        pushdown(u);
        assert(!tr[u].ls || tr[u].weight < tr[tr[u].ls].weight);
        assert(!tr[u].rs || tr[u].weight < tr[tr[u].rs].weight);
        debug(tr[u].ls);
        printf(" %ld[%d] ", tr[u].val, tr[u].cnt);
        debug(tr[u].rs);
    }
} f; // 当前这一列的 dp 数组
std::map<int,int64_t> v[N];
namespace BF {
    int64_t f[25][25], s[25][25];
    inline int64_t work(int n, int m) {
        for (int i = 1; i <= n; i++) {
            int64_t premn = 0;
            for (int j = 1; j <= m; j++) {
                s[i][j] = s[i][j-1] + v[i][j];
                f[i][j] = (premn = std::min(premn, f[i-1][j])) + s[i][j];
            }
        }
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= m; j++)
                printf("%ld%c", f[i][j], " \n"[j==m]);
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= m; j++)
                printf("%ld%c", s[i][j], " \n"[j==m]);
        return f[n][m];
    }
}
int main(int argc, char** argv) {
    int n, m, k;
    scanf("%d%d%d", &n, &m, &k);
    int64_t sum = 0;
    for (int i = 1; i <= k; i++) {
        int a, b, c, d, p;
        scanf("%d%d%d%d%d", &a, &b, &c, &d, &p);
        v[a][d] += p;
        v[b][c] -= p;
        // printf("(%d,%d)+=%d\n(%d,%d)-=%d\n", a,d,p,b,c,p);
        sum += p;
    }
    if (argc > 1 && !strcmp(argv[1], "dbg")) return printf("%ld\n", - BF::work(n, m)), 0;
    for (int i = 0; i <= m; i++) f.push_back();
    // f.debug(f.rt); putchar('\n');
    int64_t ans;
    for (int i = 1; i <= n; i++) {
        std::vector<int> rts; rts.reserve(1 + v[i].size());
        rts.push_back(f.rt);
        int lastPos = 0;
        for (auto j : v[i]) {
            // printf("([%d], %ld, sp%d)", j.first, j.second, j.first - lastPos);
            auto ret = f.split(rts.back(), j.first - lastPos);
            lastPos = j.first;
            rts.pop_back();
            rts.push_back(ret.first);
            rts.push_back(ret.second);
            f.tr[rts.back()].pull(j.second, Treap::COV_DFT);
            // f.debug(ret.first); printf("| ");
        }
        // f.debug(rts.back()); putchar('\n');
        if (i == n) ans = f.inquireLast(rts.back());
        std::reverse(rts.begin(), rts.end());
        while (rts.size() > 1) {
            int u1 = rts.back(); rts.pop_back();
            int u2 = rts.back(); rts.pop_back();
            int64_t last = f.inquireLast(u1);
            auto ret = f.lower_bound(u2, last);
            f.tr[ret.first].pull(0, last);
            rts.push_back(f.merge(u1, f.merge(ret.first, ret.second)));
        }
        f.rt = rts.front();
        // f.debug(f.rt); putchar('\n');
    }
    printf("%ld\n", -ans);
    return 0;
}