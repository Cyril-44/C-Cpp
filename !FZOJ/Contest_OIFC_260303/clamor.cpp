#include <cassert>
#include <cstdio>
#include <cmath>
constexpr int N = 1000005;
char s[N];
struct SegTr {
    struct Node {
        int sum;
        bool cov, inv;
        Node() : sum(), cov(), inv() {}
        inline void pull(int len, bool cv, bool rv) {
            if (cv) cov = true, sum = len, inv = false;
            if (rv) inv = !inv, sum = len - sum;
        }
    } tr[N << 2];
    void build(int u, int l, int r) {
        if (l == r) tr[u].sum = s[l*2-isOdd] == '(';
        else {
            int mid = l + r >> 1;
            build(u << 1, l, mid);
            build(u << 1 | 1, mid + 1, r);
            pushup(u);
        }
    }
    SegTr(int n_, int o) : n(n_), isOdd(o) { build(1, 1, n_); }
    inline void pushup(int u) { tr[u].sum = tr[u<<1].sum + tr[u<<1|1].sum; }
    inline void pushdown(int u, int llen, int rlen) {
        tr[u<<1].pull(llen, tr[u].cov, tr[u].inv);
        tr[u<<1|1].pull(rlen, tr[u].cov, tr[u].inv);
        tr[u].cov = tr[u].inv = false;
    }
    int L, R, X, n;
    int isOdd;
    void upd(int u, int l, int r) {
        if (L <= l && r <= R) {
            switch (X) {
                case -1: tr[u].pull(r - l + 1, false, true); break;
                default: tr[u].pull(r - l + 1, true, X);
            } return;
        }
        int mid = l + r >> 1;
        pushdown(u, mid - l + 1, r - mid);
        if (L <= mid) upd(u << 1, l, mid);
        if (mid < R) upd(u << 1 | 1, mid + 1, r);
        pushup(u);
    }
    int que(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u].sum;
        int mid = l + r >> 1, res = 0;
        pushdown(u, mid - l + 1, r - mid);
        if (L <= mid) res += que(u << 1, l, mid);
        if (mid < R) res += que(u << 1 | 1, mid + 1, r);
        return res;
    }
    inline void invert(int l, int r) { if (l <= r) L = l, R = r, X = -1, upd(1, 1, n); }
    inline void overwrite(int l, int r, int x) { if (l <= r) L = l, R = r, X = x, upd(1, 1, n); }
    inline int sum(int l, int r) { if (l > r) return 0; L = l, R = r; return que(1, 1, n); }
};
int main() {
    freopen("clamor.in", "r", stdin);
    freopen("clamor.out", "w", stdout);
    int n, q;
    scanf("%d%d %s", &n, &q, s+1);
    SegTr odd(n + 1 >> 1, 1), even(n >> 1, 0);
    
    /* (((()()()()()))) 最后一定是形如这样的
       因为 (( 或者 )) 都可以自由移动
       有结论：中间零散的对数-1 = abs(奇数位-偶数位)
    */
    for (int op, l, r, x; q--; ) {
        scanf("%d%d%d", &op, &l, &r); if (op == 2) scanf("%d", &x);
        int os = (l - (l&1) >> 1) + 1, oe = r + (r&1) >> 1,
            es = (l + (l&1) >> 1)    , ee = r - (r&1) >> 1;
        // assert(os <= oe); assert(es <= ee);
        switch (op) {
        case 1: odd.invert(os, oe), even.invert(es, ee); break;
        case 2: odd.overwrite(os, oe, x), even.overwrite(es, ee, x); break;
        case 3:
            if (r - l & 1 ^ 1) { puts("0"); break; }
            int osum = odd.sum(os, oe), esum = even.sum(es, ee);
            int mid = std::abs(osum - esum);
            int left = r - l + 1 - mid * 2;
            // fprintf(stderr, "Query [%d,%d], left=%d, mid=%d, osum=%d, esum=%d, ", l, r, left, mid, osum, esum);
            if (left % 4) { puts("0"); break; }
            if (left == 0) printf("%d\n", l&1 ? odd.sum(os,os) : even.sum(es,es));
            else if (mid == 0) printf("%d\n", left / 2);
            else printf("%d\n", left / 2 + (l&1 ? osum > esum : osum < esum)); // 排除形如 (()())，同时不能漏掉 ((()))
  
        }
    }
    return 0;
} 