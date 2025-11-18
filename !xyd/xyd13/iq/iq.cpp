#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <tuple>
#define For(i,s,t) for (int i = (s); (i) <= (t); ++(i))
constexpr int N = 1000005;
int a[N];
namespace FastI {
    char buf[1 << 21], *p1(nullptr), *p2(nullptr);
    inline char get() {
        if (p1 == p2) p2 = (p1=buf) + fread(buf, 1, sizeof buf, stdin);
        return *p1++;
    }
    inline void in(int &x) {
        char ch = get();
        while (ch < '0' || ch > '9') ch = get();
        for (x = ch ^ '0', ch = get(); ch >= '0' && ch <= '9'; ch = get())
            x = (x << 3) + (x << 1) + (ch ^ '0');
    }
} using FastI::in;
using Query_t = std::tuple<int, int, int>;
inline Query_t gor(int r, int L, int R, int lmt = (int)2e9) {
    std::pair<int,int> res(r, L);
    int cnt = 0;
    while (L <= R && lmt--) {
        --R, ++r;
        while (L <= R && a[L] == r) ++L;
        if (res.second ^ L) res = std::make_pair(r, L), cnt = 0;
        else ++cnt;
    }
    return Query_t(res.first, res.second, cnt);
}
inline Query_t gol(int l, int L, int R, int lmt = (int)2e9) {
    std::pair<int,int> res(l, R);
    int cnt = 0;
    while (L <= R && lmt--) {
        ++L, --l;
        while (L <= R && a[R] == l) --R;
        if (res.second ^ R) res = std::make_pair(l, R), cnt = 0;
        else ++cnt;
    }
    return Query_t(res.first, res.second, cnt);
}
inline void solve() {
    int n, k, p0;
    in(n), in(k), in(p0);
    For (i, 1, n) in(a[i]);
    std::sort(a+1, a+1 + n);
    int pL = std::lower_bound(a+1, a+1+n, p0) - a - 1,
        pR = std::upper_bound(a+1, a+1+n, p0+k) - a;
    auto [r1 , R1,  ext1] = gor(p0 + k, pR, n       );
    auto [l1a, L1a, _1a ] = gol(r1 - k, 1 , pL      );
    auto [l1b, L1b, _1b ] = gol(p0    , 1 , pL, ext1);
    auto [l2 , L2 , ext2] = gol(p0    , 1 , pL      );
    auto [r2a, R2a, _2a ] = gor(l2 + k, pR, n       );
    auto [r2b, R2b, _2b ] = gor(p0 + k, pR, n , ext2);
#ifdef DEBUG
    fprintf(stderr, "Right Attepmt: add[%d,%d] from[%d,%d] goto[%d,%d]ext=%d then[%d,%d], (%d,%d) -> (min(%d,%d),%d)\n", l1b, l1b+k, p0, p0+k, r1-k-ext1, r1-ext1, ext1, l1a, l1a+k, pL, pR, L1a, L1b, R1);
    fprintf(stderr, " Left Attempt: add[%d,%d] from[%d,%d] goto[%d,%d]ext=%d then[%d,%d], (%d,%d) -> (%d,max(%d,%d))\n", r2b-k, r2b, p0, p0+k, l2+ext2, l2+k+ext2, ext2, r2a-k, r2a, pL, pR, L2, R2a, R2b);
#endif
    printf("%d\n", std::max(R1 - std::min(L1a, L1b) - 1, std::max(R2a, R2b) - L2 - 1));
}
int main() {
    freopen("iq.in", "r", stdin);
    freopen("iq.out", "w", stdout);
    int T;
    in(T);
    while (T--) solve();
    return 0;
}
