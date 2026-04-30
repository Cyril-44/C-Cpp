#include <cstdio>
#include <cstdint>
#include <cstring>
#include <algorithm>
#include <vector>
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
可以使用线段树二分去模拟 WBLT/Treap/Splay 的 lower_bound 分裂。
*/

struct WBLT {
    constexpr static int64_t COV_DFT = (1ll << 62) - 1;
    struct Node {
        int ls, rs, cnt;
        int64_t lmx, rmn, add, cov;
        inline Node(int64_t v = 0) : ls(), rs(), cnt(1), lmx(v), rmn(v) {}
        inline Node(int l, int r) : ls(l), rs(r), add(), cov(COV_DFT) {}
        inline void pull(int64_t p, int64_t c) {
            if (c != COV_DFT) lmx = rmn = cov = c, add = 0;
            if (p) lmx += p, rmn += p, add += p;
        }
    } tr[N*3];
    int totcnt{};
    int bin[N*2]{0};
    inline void pushup(int u) {
        tr[u].cnt = tr[tr[u].ls].cnt + tr[tr[u].rs].cnt;
        tr[u].lmx = tr[tr[u].ls].lmx, tr[u].rmn = tr[tr[u].rs].rmn;
    }
    inline void pushdown(int u) {
        if (!tr[u].add && tr[u].cov == COV_DFT) return;
        tr[tr[u].ls].pull(tr[u].add, tr[u].cov); 
        tr[tr[u].rs].pull(tr[u].add, tr[u].cov);
        tr[u].add = 0, tr[u].cov = COV_DFT;
    }
    template<class... Args>
    inline int alloc(Args... args) {
        int u = bin[0] ? bin[bin[0]--] : ++totcnt;
        tr[u] = Node(args...);
        if (tr[u].ls) pushup(u);
        return u;
    }
    inline void recycle(int u) { bin[++bin[0]] = u; }
    int build(int l, int r) {
        if (l == r) return alloc();
        int mid = l + r >> 1;
        int ls = build(l, mid), rs = build(mid + 1, r);
        return alloc(ls, rs);
    }
    inline bool tooHeavy(int wl, int wr) { return wr * 3 < wl; }
    inline std::pair<int,int> cut(int u) { pushdown(u); recycle(u); return {tr[u].ls, tr[u].rs}; }
    inline int merge(int u, int v) {
        if (!u || !v) return u | v;
        if (tooHeavy(tr[u].cnt, tr[v].cnt)) {
            auto [ul, ur] = cut(u);
            if (tooHeavy(tr[ur].cnt + tr[v].cnt, tr[ul].cnt)) {
                auto [url, urr] = cut(ur);
                return merge(merge(ul, url), merge(urr, v));
            }
            return merge(ul, merge(ur, v));
        }
        if (tooHeavy(tr[v].cnt, tr[u].cnt)) {
            auto [vl, vr] = cut(v);
            if (tooHeavy(tr[u].cnt + tr[vl].cnt, tr[vr].cnt)) {
                auto [vll, vlr] = cut(vl);
                return merge(merge(u, vll), merge(vlr, vr));
            }
            return merge(merge(u, vl), vr);
        }
        return alloc(u, v);
    }
    inline std::pair<int,int> split(int u, int w) {
        if (!w) return {0, u};
        if (tr[u].cnt == w) return {u, 0};
        auto [ul, ur] = cut(u);
        if (w <= tr[ul].cnt) {
            auto [ull, ulr] = split(ul, w);
            return {ull, merge(ulr, ur)};
        }
        auto [url, urr] = split(ur, w - tr[ul].cnt);
        return {merge(ul, url), urr};
    }
    inline std::pair<int,int> lower_bound(int u, int64_t w) {
        if (w < tr[u].rmn) return {u, 0};
        if (w >= tr[u].lmx) return {0, u};
        auto [ul, ur] = cut(u);
        if (w >= tr[ul].rmn) {
            auto [ull, ulr] = lower_bound(ul, w);
            return {ull, merge(ulr, ur)};
        }
        auto [url, urr] = lower_bound(ur, w);
        return {merge(ul, url), urr};
    }
    int rt;
    inline void operate(int u, bool type, int64_t v) { // 0:add, 1:cov
        if (!u) return;
        int64_t op[2]{0, COV_DFT};
        op[type] = v;
        tr[u].pull(op[0], op[1]);
    }
    inline int64_t inquire(int u) { return tr[u].rmn; }
    inline void debug(int u) {
        if (!u) return;
        if (tr[u].ls) pushdown(u), debug(tr[u].ls), debug(tr[u].rs);
        else printf("%ld ", tr[u].lmx);
    }
} f;

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
    std::sort(all+1, all+1+k*2);
    // if (false && argc > 1 && !strcmp(argv[1], "dbg")) return (n<=2000&&printf("%ld\n", - BF::work(n, m))), 0;
    f.rt = f.build(1, m+1);
    // f.debug(f.rt); putchar('\n');
    int cur = 1;
    for (int i = 1; i <= n; i++) {
        if (cur > k*2 || all[cur].first.first > i) continue;
        rts[rts[0] = 1] = f.rt;
        int lastPos = 0;
        for (; cur <= k*2 && all[cur].first.first == i; cur++) {
            // printf("([%d], %ld, sp%d)", j.first, j.second, j.first - lastPos);
            auto [ul, ur] = f.split(rts[rts[0]], all[cur].first.second - lastPos);
            lastPos = all[cur].first.second;
            rts[rts[0]] = ul;
            rts[++rts[0]] = ur;
            f.operate(ur, 0, all[cur].second);
            // f.debug(ul); printf("\\ ");
            // f.debug(ur); printf("| ");
        }
        // f.debug(rts.back()); putchar('\n');
        // std::reverse(rts+1, rts+1+rts[0]);
        for (int j = 1; j < rts[0]; j++) {
            int u1 = rts[j];
            int u2 = rts[j+1];
            if (!u1) continue;
            int64_t last = f.inquire(u1);
            auto [ul, ur] = f.lower_bound(u2, last);
            f.operate(ul, 1, last);
            // f.debug(ul); printf("\\ ");
            // f.debug(ur); printf("| ");
            rts[j+1] = f.merge(u1, f.merge(ul, ur));
        }
        f.rt = rts[rts[0]];
        // f.debug(f.rt); putchar('\n');
    }
    printf("%ld\n", -f.inquire(f.rt));
    return 0;
}