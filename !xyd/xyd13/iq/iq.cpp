#include <stdio.h>
#include <string.h>
#include <algorithm>
#define For(i,s,t) for (int (i) = (s); (i) <= (t); ++(i))
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
inline std::pair<int,int> gor(int r, int L, int R) {
    while (L < R) {
        --R, ++r;
        while (L < R && a[L] == r) ++L;
    }
    return std::make_pair(r, L);
}
inline std::pair<int,int> gol(int l, int L, int R) {
    while (L < R) {
        ++L, --l;
        while (L < R && a[R] == l) --R;
    }
    return std::make_pair(l, R);
}
inline void solve() {
    int n, k, p0;
    scanf("%d%d%d", &n, &k, &p0);
    For (i, 1, n) in(a[i]);
    std::sort(a+1, a+1 + n);
    int pp0 = std::lower_bound(a+1, a+1+n, p0) - a,
        pp1 = std::upper_bound(a+1, a+1+n, p0+k) - a-1;
    
}
int main() {
    int T;
    scanf("%d", &T);
    while (T--) solve();
    return 0;
}
