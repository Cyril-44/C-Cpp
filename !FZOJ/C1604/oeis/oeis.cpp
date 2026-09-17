#include <cstdio>
constexpr int N = 10000004, MOD = (int)1e9 + 7;
struct Mint {
    Mint& operator+=(Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    Mint operator/=(Mint o) { return *this *= o ^ (MOD-2); }
    friend Mint operator+(Mint x, Mint y) { return x += y; }
    friend Mint operator-(Mint x, Mint y) { return x -= y; }
    friend Mint operator*(Mint x, Mint y) { return x *= y; }
    friend Mint operator/(Mint x, Mint y) { return x /= y; }
    Mint operator^(int n) const {
        Mint res = 1, b = *this;
        for (; n; b *= b, n >>= 1)
            if (n & 1) res *= b;
        return res;
    }
    Mint(int v=0) : val(v) {}
private: int val;
} fac[N], ifac[N], ipw2[N];
inline Mint ways(int x) { return fac[2*x] * ipw2[x]; }
int main() {
    int n;
    scanf("%d", &n);
    if (n == 1) { puts("1"); return; }
    fac[0] = 1;
    for (int i = 1; i <= n*2; i++) fac[i] = fac[i-1] * i;
    ifac[2*n] = fac[2*n] ^ (MOD-2);
    for (int i = n*2; i >= 1; i--) ifac[i-1] = ifac[i] * i;
    ipw2[0] = 1, ipw2[1] = Mint(1) / 2;
    for (int i = 2; i <= n*2; i++) ipw2[i] = ipw2[i-1] * ipw2[1];
    Mint cnt = 0;
    for (int x = 1; x < n; x++) cnt += (n-x) * ((Mint(2*x-1)^(n-x-1)) + (Mint(2*x)^(n-x-1))) * ways(x) * fac[n] * ifac[x];
    printf("%d\n", cnt / ways(n));
    return 0;
}
