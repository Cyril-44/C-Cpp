#include <algorithm>
#include <limits>
#include <stdio.h>
constexpr int N = 500005;
#define umax(x, y) ((x) = std::max((x), (y)))
int n;
namespace SegTr {
struct Node {
    int mx, hmx;   // hmx
                   // 下面全是懒标记
    int add, hadd; // hadd: 从上次pushdown到这次，最大的 add
    bool covered;  // 是否被赋值过（即当前区间是否值全部相等）
    int cov, hcov; // hcov: 同理 最大的赋值
    inline void pulladd(int tadd, int thadd) {
        umax(hmx, mx + thadd), mx += tadd;
        if (covered) // 直接区间一起加
            umax(hcov, cov + thadd), cov += tadd;
        else
            umax(hadd, add + thadd), add += tadd;
    }
    inline void pullcov(int tcov, int thcov) {
        umax(hmx, thcov), mx = tcov;
        umax(hcov, thcov), cov = tcov, covered = true;
    }
} tr[N << 2];
inline void pushdown(int u) {
    if (tr[u].add || tr[u].hadd) { // 默认先加再赋值 注意 add 可能叠加后为0 
        tr[u << 1].pulladd(tr[u].add, tr[u].hadd);
        tr[u << 1 | 1].pulladd(tr[u].add, tr[u].hadd);
        tr[u].add = tr[u].hadd = 0;
    }
    if (tr[u].covered) {
        tr[u << 1].pullcov(tr[u].cov, tr[u].hcov);
        tr[u << 1 | 1].pullcov(tr[u].cov, tr[u].hcov);
        tr[u].covered = false;
        tr[u].hcov = std::numeric_limits<int>::min();
    }
}
inline void pushup(int u) {
    tr[u].mx = std::max(tr[u << 1].mx, tr[u << 1 | 1].mx);
    tr[u].hmx = std::max(tr[u << 1].hmx, tr[u << 1 | 1].hmx);
}
void build(int u = 1, int l = 1, int r = n) {
    if (l == r) {
        scanf("%d", &tr[u].mx);
        tr[u].hmx = tr[u].mx;
        tr[u].hcov = std::numeric_limits<int>::min();
        return;
    }
    int mid = l + r >> 1;
    build(u << 1, l, mid);
    build(u << 1 | 1, mid + 1, r);
    pushup(u);
}
int L, R, X;
#define UPD(func)                                                                                                      \
    void func(int u, int l, int r) {                                                                                   \
        if (l > r) return fprintf(stderr, "Unexpected [%d,%d]\n", l, r), void();                                       \
        if (L <= l && r <= R) return tr[u].pull##func(X, X);                                                           \
        int mid = l + r >> 1;                                                                                          \
        pushdown(u);                                                                                                   \
        if (L <= mid) func(u << 1, l, mid);                                                                            \
        if (mid < R) func(u << 1 | 1, mid + 1, r);                                                                     \
        pushup(u);                                                                                                     \
    }                                                                                                                  \
    inline void upd##func(int l, int r, int op) {                                                                      \
        L = l, R = r, X = op;                                                                                          \
        func(1, 1, n);                                                                                                 \
    }
#define QUE(var)                                                                                                       \
    int q##var(int u, int l, int r) {                                                                                  \
        if (l > r) return fprintf(stderr, "Unexpected [%d,%d]\n", l, r), std::numeric_limits<int>::min();              \
        if (L <= l && r <= R) return tr[u].var;                                                                        \
        int res = std::numeric_limits<int>::min(), mid = l + r >> 1;                                                   \
        pushdown(u);                                                                                                   \
        if (L <= mid) res = std::max(res, q##var(u << 1, l, mid));                                                     \
        if (mid < R) res = std::max(res, q##var(u << 1 | 1, mid + 1, r));                                              \
        return res;                                                                                                    \
    }                                                                                                                  \
    inline int que##var(int l, int r) {                                                                                \
        L = l, R = r;                                                                                                  \
        return q##var(1, 1, n);                                                                                        \
    }
UPD(add) UPD(cov) QUE(mx) QUE(hmx)
#undef UPD
#undef QUE
} // namespace SegTr

int main() {
    scanf("%d", &n);
    SegTr::build();
    int e;
    scanf("%d", &e);
    char ch;
    for (int x, y, z; e--;) {
        scanf(" %c%d%d", &ch, &x, &y);
        if (ch == 'P' || ch == 'C') scanf("%d", &z);
        switch (ch) {
            case 'Q': printf("%d\n", SegTr::quemx(x, y)); break;
            case 'A': printf("%d\n", SegTr::quehmx(x, y)); break;
            case 'P': SegTr::updadd(x, y, z); break;
            case 'C': SegTr::updcov(x, y, z); break;
            default: throw;
        }
#ifdef DEBUG
        fprintf(stderr, "%d %d\n", SegTr::quemx(x, y), SegTr::quehmx(x, y));
#endif
    }
    return 0;
}