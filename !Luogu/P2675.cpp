#include <bits/stdc++.h>
constexpr int MOD = 10007;
struct Mint {
    Mint& operator+=(const Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator*=(const Mint o) { val = 1u * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint x, const Mint y) { return x += y; }
    friend Mint operator*(Mint x, const Mint y) { return x *= y; }
    Mint(int v=0) : val(v % MOD) {}
    Mint pow(int n) {
        Mint res = 1, b = *this;
        for (; n; n >>= 1, b *= b)
            if (n & 1) res *= b;
        return res;
    }
    template<class T> explicit operator T() const { return static_cast<T>(val); }
private: short val;
};
std::vector<Mint> C[MOD];
Mint com(int n, int m) {
    if (n < m) return 0;
    if ((int)C[n].size() > m) return C[n][m];
    if (C[n].empty()) C[n].push_back(1);
    int idx = C[n].size();
    C[n].resize(m + 1);
    for (int i = idx; i <= m; i++) C[n][i] = C[n][i-1] * Mint(i).pow(MOD-2) * (n-i+1);
    return C[n][m];
}
Mint lucas(int n, int m) {
    if (n < m) return 0;
    if (n < MOD) return com(n, m);
    return lucas(n / MOD, m / MOD) * com(n % MOD, m % MOD);
}
int main() {
    int n; scanf("%d", &n);
    --n;
    int val = 1, delt = 2;
    Mint ans = 0;
    for (int i = 0; i <= n; i++) {
        ans += lucas(n, i) * val;
        val += delt;
        if (val == n+2) val = n+1, delt = -2;
        else if (val == n+3) val = n, delt = -2;
    }
    printf("%d\n", int(ans));
}