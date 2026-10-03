#include <cstdio>
#include <algorithm>
#include <cstring>
#ifdef CLANGD
constexpr int N = 15;
#else
constexpr int N = 500005;
#endif
struct Mint {
    constexpr static int MOD = (int)1e9 + 7;
    Mint& operator+=(Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint x, Mint y) { return x += y; }
    friend Mint operator-(Mint x, Mint y) { return x -= y; }
    friend Mint operator*(Mint x, Mint y) { return x *= y; }
    Mint (int v=0) : val(v) {}
 int val;
} bel[2][2], g[2][2], h[2][10][2];
char nl[N], nr[N];
#define For10(i, s) for (int i = (s); i < 10; i++)
#define For(i, s, t) for (int i = (s); i < (t); i++)
inline Mint calcSum(char *n) {
    auto fn = bel[0], fp = bel[1]; memset(fn, 0, sizeof(bel)/2);
    auto gn = g[0], gp = g[1]; memset(gn, 0, sizeof(g)/2);
    auto hn = h[0], hp = h[1]; memset(hn, 0, sizeof(h)/2);
    gn[1] = 1;
    for (int pos = 1; n[pos-1]; pos++) {
        int ni = n[pos-1] - '0';
        std::swap(fn, fp); memset(fn, 0, sizeof(bel)/2);
        std::swap(gn, gp); memset(gn, 0, sizeof(g)/2);
        std::swap(hn, hp); memset(hn, 0, sizeof(h)/2);
        Mint hs[11][2]{}; int htrans[2][2]{};
        auto trans = [&](int d, int on, int op) {
            fn[on] += fp[op] + hs[d+1][op];
            gn[on] += gp[op];
            ++htrans[on][op];
            hn[d][on] += gp[op];
        };
        for (int i = 9; i >= 0; i--) {
            hs[i][0] = hp[i][0] + hs[i+1][0];
            hs[i][1] = hp[i][1] + hs[i+1][1];
        }
        For(d, 0, ni) trans(d, 0, 1), trans(d, 0, 0);
        trans(ni, 0, 0), trans(ni, 1, 1);
        For10(d, ni+1) trans(d, 0, 0);
        for (int on:{0,1}) for (int op:{0,1})
            For10(d, 0) hn[d][on] += htrans[on][op] * hp[d][op];
    }
    return fn[0] + fn[1];
}
int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        scanf("%s%s", nl, nr);
        for (int p = strlen(nl)-1; p >= 0; p--)
            if (--nl[p] < '0') nl[p] += 10;
            else break;
        printf("%d\n", calcSum(nr) - calcSum(nl));
    }
    return 0;
}