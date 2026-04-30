#include <cstdio>
#include <cstdint>
#include <cassert>
#include <cstring>
#include <random>
#include <algorithm>
#include <map>
constexpr int N = 1000005;
struct FastI {
    char buf[1 << 20], *p1, *p2;
    FastI() : p1(), p2() {}
    [[gnu::always_inline]] inline char gc() {
        if (p1 == p2) p2 = (p1=buf) + fread(buf, 1, sizeof buf, stdin);
        return *p1++;
    }
    template<typename T>
    inline void operator()(T &x) {
        char ch = gc();
        bool flg{};
        while ((ch < '0' || ch > '9') && (ch != '-')) ch = gc();
        if (ch == '-') flg = true, ch = gc();
        for (x = 0; ch >= '0' && ch <= '9'; ch = gc())
            x = (x << 3) + (x << 1) + (ch ^ '0');
        if (flg) x = -x;
    }
} in;
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
    Treap() : rng(1145141919810ull), totcnt(), rt() {}
    struct Node {
        int ls, rs, cnt;
        int64_t val, lmx, rmn, add, cov;
        uint64_t weight;
        inline void pull(int64_t p, int64_t c) {
            if (c ^ COV_DFT) val = lmx = rmn = cov = c, add = 0;
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
        ++totcnt;
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
            auto ret = std::move(lower_bound(tr[u].ls, w));
            tr[u].ls = ret.second;
            pushup(u);
            return {ret.first, u};
        }
        auto ret = std::move(lower_bound(tr[u].rs, w));
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
            auto ret = std::move(split(tr[u].ls, w));
            tr[u].ls = ret.second;
            pushup(u);
            return {ret.first, u};
        }
        auto ret = std::move(split(tr[u].rs, w - tr[tr[u].ls].cnt - 1));
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
std::pair<std::pair<int,int>,int64_t> all[N << 1];
int rts[N << 1];
int main(int argc, char** argv) {
    int n, m, k;
    in(n), in(m), in(k);
    for (int i = 1; i <= k; i++) {
        int a, b, c, d, p;
        in(a), in(b), in(c), in(d), in(p);
        all[i*2-1] = {{a, d}, p};
        all[i*2] = {{b, c}, -p};
        // printf("(%d,%d)+=%d\n(%d,%d)-=%d\n", a,d,p,b,c,p);
    }
    std::sort(all+1, all+1+k*2, [](const std::pair<std::pair<int,int>,int64_t> &a, const std::pair<std::pair<int,int>,int64_t> &b){return a.first < b.first;} );
    // if (false && argc > 1 && !strcmp(argv[1], "dbg")) return (n<=2000&&printf("%ld\n", - BF::work(n, m))), 0;
    for (int i = 0; i <= m; i++) f.push_back();
    // f.debug(f.rt); putchar('\n');
    int cur = 1;
    for (int i = 1; i <= n; i++) {
        if (cur > k*2 || all[cur].first.first > i) continue;
        rts[rts[0] = 1] = f.rt;
        int lastPos = 0;
        for (; cur <= k*2 && all[cur].first.first == i; cur++) {
            // printf("([%d], %ld, sp%d)", j.first, j.second, j.first - lastPos);
            auto [ul, ur] = std::move(f.split(rts[rts[0]], all[cur].first.second - lastPos));
            lastPos = all[cur].first.second;
            rts[rts[0]] = ul;
            rts[++rts[0]] = ur;
            f.tr[ur].pull(all[cur].second, Treap::COV_DFT);
            // f.debug(ul); printf("\\ ");
            // f.debug(ur); printf("| ");
        }
        // f.debug(rts.back()); putchar('\n');
        // std::reverse(rts+1, rts+1+rts[0]);
        for (int j = 1; j < rts[0]; j++) {
            int u1 = rts[j];
            int u2 = rts[j+1];
            if (!u1) continue;
            int64_t last = f.inquireLast(u1);
            auto [ul, ur] = std::move(f.lower_bound(u2, last));
            f.tr[ul].pull(0, last);
            // f.debug(ul); printf("\\ ");
            // f.debug(ur); printf("| ");
            rts[j+1] = f.merge(u1, f.merge(ul, ur));
        }
        f.rt = rts[rts[0]];
        // f.debug(f.rt); putchar('\n');
    }
    printf("%ld\n", -f.inquireLast(f.rt));
    return 0;
}
