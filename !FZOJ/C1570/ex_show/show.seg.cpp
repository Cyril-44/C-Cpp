#include <cstdio>
#include <cstdint>
#include <cstring>
#include <algorithm>
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
可以使用线段树二分去模拟 Treap/Splay 的 lower_bound 分裂。
*/
struct SegTr {
#define GoL (r = mid, u = u<<1)
#define GoR (l = mid + 1, u = u<<1|1)
    constexpr static int64_t COV_DFT = (1ll << 62) - 1;
    int n;
    struct Node {
        int64_t lmx, rmn, add, cov;
        [[gnu::always_inline]] inline void plus(int64_t p) { lmx += p, rmn += p, add += p; }
        [[gnu::always_inline]] inline void cover(int64_t c) { lmx = rmn = cov = c, add = 0; }
        inline void pull(int64_t p, int64_t c) {
            if (c ^ COV_DFT) cover(c);
            if (p) plus(p);
        }
    } tr[N << 2];
    int L, R;
    int64_t X;
    inline void pushup(int u) {
        tr[u].lmx = tr[u<<1].lmx;
        tr[u].rmn = tr[u<<1|1].rmn;
    }
    inline void pushdown(int u) {
        if (!tr[u].add && tr[u].cov == COV_DFT) return;
        tr[u<<1].pull(tr[u].add, tr[u].cov);
        tr[u<<1|1].pull(tr[u].add, tr[u].cov);
        tr[u].add = 0, tr[u].cov = COV_DFT;
    }
    void updcv(int u, int l, int r) {
        if (L <= l && r <= R) tr[u].cover(X);
        else {
            int mid = l + r >> 1;
            pushdown(u);
            if (L <= mid) updcv(u << 1, l, mid);
            if (mid < R) updcv(u << 1 | 1, mid + 1, r);
            pushup(u);
        }
    }
    inline void updateAdd(int L, int64_t x) {
        int u = 1, l = 1, r = n;
        static int sta[30];
        while (L > l) {
            pushdown(u);
            int mid = l + r >> 1;
            sta[++sta[0]] = u;
            if (L <= mid) tr[u<<1|1].plus(x), GoL;
            else GoR;
        }
        tr[u].plus(x);
        while (sta[0]) pushup(sta[sta[0]--]);
    }
    inline void updateCover(int l, int r, int64_t x) {
        L = l, R = r, X = x;
        updcv(1, 1, n);
    }
    inline int64_t inquire(int R) {
        int u = 1, l = 1, r = n;
        while (R < r) {
            pushdown(u);
            int mid = l + r >> 1;
            mid < R ? GoR : GoL;
        }
        return tr[u].rmn;
    }
    inline int lower_bound(int L, int R, int64_t val) {
        int u = 1, l = 1, r = n;
        while (l != r) {
            pushdown(u);
            int mid = l + r >> 1;
            if (mid < L) GoR;
            else if (mid >= R) GoL;
            else if (tr[u<<1|1].lmx <= val) GoL;
            else GoR;
        }
        if (l == L && tr[u].rmn <= val) return -1;
        return l;
    }
    void debug(int u, int l, int r) {
        if (l == r) printf("%ld ", tr[u].rmn);
        else {
            int mid = l + r >> 1;
            pushdown(u);
            debug(u << 1, l, mid);
            debug(u << 1 | 1, mid + 1, r);
        }
    }
#undef GoL
#undef GoR
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
    std::sort(all+1, all+1+k*2, [](const std::pair<std::pair<int,int>,int64_t> &a, const std::pair<std::pair<int,int>,int64_t> &b){return a.first < b.first;} );
    // if (false && argc > 1 && !strcmp(argv[1], "dbg")) return (n<=2000&&printf("%ld\n", - BF::work(n, m))), 0;
    f.n = m + 1;
    // f.debug(f.rt); putchar('\n');
    int cur = 1;
    for (int i = 1; i <= n; i++) {
        if (cur > k*2 || all[cur].first.first > i) continue;
        rts[0] = 0;
        for (; cur <= k*2 && all[cur].first.first == i; cur++) {
            // printf("([%d], %ld, sp%d)", j.first, j.second, j.first - lastPos);
            int now = all[cur].first.second;
            f.updateAdd(now + 1, all[cur].second);
            if (rts[rts[0]] != now) rts[++rts[0]] = now;
        }
        if (rts[rts[0]] != m+1) rts[++rts[0]] = m+1;
        for (int j = 1; j < rts[0]; j++) {
            int u1 = rts[j];
            int u2 = rts[j+1];
            if (!u1) continue;
            int64_t last = f.inquire(u1);
            int u = f.lower_bound(u1 + 1, u2, last);
            if (~u) f.updateCover(u1 + 1, u, last);
        }
    }
    printf("%ld\n", -f.inquire(m+1));
    return 0;
}
