#include <stdio.h>
const int N = 500005;
int uv[N][2], p[N];
inline void in(int &x) {
    char ch = getchar();
    while (ch < '0' || ch > '9')
        ch = getchar();
    for (x = 0; ch >= '0' && ch <= '9'; ch = getchar())
        x = (x << 3) + (x << 1) + (ch ^ '0');
}
namespace SubT1 {
    inline void work(const int &n) {
        
    }
}
namespace SubT2 {
    int fact[N];
    inline long long qpow(long long b, int n, int i) {
        long long res = 1;
        while (n) {
            if (n & 1) res = res * b % p[i];
            b = b * b % p[i], n >>= 1;
        }
        return res;
    }
    inline long long C(int n, int m, int ii) {
        long long bdiv = 1, div = 1;
        for (int i = n; i > n-m; i--)
            bdiv = bdiv * i % p[ii];
        for (int i = 1; i <= div; i++)
            div = div * i % p[ii];
        return bdiv * qpow(div, p[ii]-2, ii);
    }
    inline void work(const int &n, const int &k) {
        for (int i = 1; i <= n; i++) {
            long long ans = 0;
            for (int j = 0; j <= k; j++) { // j条重边
                ans = (ans + qpow(n - i, j, i)) % p[i];
                if (k-j <= n-i-1) // C(n-i-1>>1, k-j)
                    ans = (ans + C(n-i-1 >> 1, k - j, i)) % p[i];
            }
            printf("%lld\n", ans);
        }
    }
}
int main() {
    int n, k, u, v;
    register bool flgB = true;
    in(n), in(k);
    for (int i = 1; i < n; i++) {
        in(u), in(v);
        if (u+1 ^ v) flgB = false;
    }
    for (int i = 1; i <= n; i++)
        in(p[i]);
    if (flgB) SubT2::work(n, k);
    if (k == 1) SubT1::work(n);
    return 0;
}