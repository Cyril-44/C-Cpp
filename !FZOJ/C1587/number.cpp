#include <cstdio>
#include <cstring>
constexpr int N = 1000005, MOD = 998244353;
struct Mint {
    Mint& operator+=(Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint a, Mint b) { return a += b; }
    friend Mint operator-(Mint a, Mint b) { return a -= b; }
    friend Mint operator*(Mint a, Mint b) { return a *= b; }
    Mint(int o = 0) : val(o) {}
private: int val;
};
Mint inv[N];
int main() { // (2^n - 1)(2^n - 2)(2^n - 4)...(2^n - 2^(n-1)) / n!
    int n;
    scanf("%d", &n);
    Mint ans = 1, pw2n = 1, pw2 = 1;
    for (int i = 0; i < n; i++) pw2n += pw2n;
    for (int i = 0; i < n; i++)
        ans *= pw2n - pw2, pw2 += pw2;
    inv[1] = 1;
    for (int i = 2; i <= n; i++)
        ans *= inv[i] = (MOD - MOD / i) * inv[MOD % i];
    printf("%d\n", ans);
    return 0;
}