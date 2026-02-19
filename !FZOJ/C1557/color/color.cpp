#include <stdio.h>
#include <algorithm>
#include <setjmp.h>
constexpr int N = 1000005, K = 2005, MOD = (int)1e9 + 7;
inline void mul(int &x, int y) { x = (long long)x * y % MOD; }
inline void add(int &x, int y) { if ((x += (unsigned)y) >= MOD) x -= MOD; }
std::pair<int,int> lm[K];
int mpow[N];
inline int qpow(int b, unsigned n) {
    int res = 1;
    while (n) {
        if (n & 1) mul(res, b);
        mul(b, b); n >>= 1;
    }
    return res;
}
int fac[N], ifac[N];
int main() {
    int n, m, l;
    scanf("%d%d%d", &n, &m, &l);
    for (int i = 1; i <= l; i++)
        scanf("%d%d", &lm[i].first, &lm[i].second);
    { // Pre
        mpow[0] = 1;
        for (int i = 1; i <= m; i++)
            mul(mpow[i] = mpow[i-1], i);
        fac[0] = 1;
        for (int i = 1; i <= n || i <= l; i++)
            mul(fac[i] = fac[i-1], i);
        ifac[n] = qpow(fac[n], MOD - 2);
        for (int i = std::max(n, l)-1; i >= 0; i--)
            mul(ifac[i] = ifac[i+1], i+1);
        int uselessCnt = 0;
        for (int i = 1; i <= l; i++)
            for (int j = 1; j <= l; j++)
                if (lm[i].first <= lm[j].first && lm[j].second <= lm[i].second) {
                    lm[i].first = lm[i].second = N;
                    ++uselessCnt;
                    break;
                }
        std::sort(lm+1, lm+1 + l);
        l -= uselessCnt;
        for (int i = 2; i <= l; i++)
            if (lm[i].first > lm[i-1].second)
                mul(ans, mpow[lm[i].first - lm[i-1].second - 1]);
        mul(ans, mpow[lm[1].first - 1]);
        mul(ans, mpow[n - lm[l].second]);
    }
    if (n < K) {
        static int f[K][K]; // 到 i, 最后一个未匹配的二元组在 j
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= i; j++)

        }
    }
    
    return 0;
}