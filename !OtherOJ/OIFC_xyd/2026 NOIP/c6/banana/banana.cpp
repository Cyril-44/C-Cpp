#include <bits/stdc++.h>
constexpr int MOD = 998244853;
constexpr int N = 1000005;
namespace FastI {
    char buf[1 << 20], *p1, *p2;
    inline char gc() {
        if (p1 == p2) {
            p2 = (p1=buf) + fread(buf, 1, sizeof buf, stdin);
            if (p1 == p2) return EOF;
        }
        return *p1++;
    }
    template<class T> inline void in(T &x) {
        char ch = gc();
        while (ch < '0') ch = gc();
        for (x = 0; ch >= '0'; ch = gc())
            x = (x << 3) + (x << 1) + (ch ^ '0');
    }
} using FastI::in;
template<int MOD>
struct Mint {
    Mint& operator+=(const Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(const Mint o) { if ((val -= o.val) < MOD) val += MOD; return *this; }
    Mint& operator*=(const Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint x, const Mint y) { return x += y; }
    friend Mint operator-(Mint x, const Mint y) { return x -= y; }
    friend Mint operator*(Mint x, const Mint y) { return x *= y; }
    Mint pow(int n) const {
        Mint res = 1, b = *this;
        for (; n; n >>= 1, b *= b)
            if (n & 1) res *= b;
        return res;
    }
    Mint inv() const { return pow(MOD-2); }
    Mint(int v=0) : val(v) {}
    Mint(uint64_t v) : val(v % MOD) {}
    template<typename T> explicit operator T() const { return static_cast<T>(val); }
private: int val;
};
Mint<MOD> fac[N];
Mint<MOD-1> facm[N];
int main() {
    freopen("banana.in", "r", stdin);
    freopen("banana.out", "w", stdout);
    { const int n = 1e6;
        fac[0] = 1;
        for (int i = 1; i <= n; i++) fac[i] = fac[i-1] * i;
        facm[0] = 1;
        for (int i = 1; i <= n; i++) facm[i] = facm[i-1] * i;
    }
    int T, n; in(T);
    while (T--) {
        in(n);
        auto prod = fac[n] * fac[n];
        if (n & 1) printf("%d %d\n", fac[n] * prod, prod.pow(int(facm[n])));
        else {
            auto half = prod * Mint<MOD>(2).pow(n).inv();
            printf("%d %d\n",
                fac[n/2].pow(2) * half + (fac[n] - fac[n/2].pow(2)) * prod,
                half.pow(int(facm[n/2].pow(2))) * prod.pow(int(facm[n] - facm[n/2].pow(2))));
        }
    }
}