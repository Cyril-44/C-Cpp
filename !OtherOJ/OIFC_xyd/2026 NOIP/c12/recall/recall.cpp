#include <bits/stdc++.h>
constexpr int N = 3005, B = 31, MOD = 998244353;
char s[N];
uint64_t hash[N], rhash[N], base[N];
struct Mint {
    Mint& operator+=(const Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(const Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(const Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint x, const Mint y) { return x += y; }
    friend Mint operator-(Mint x, const Mint y) { return x -= y; }
    friend Mint operator*(Mint x, const Mint y) { return x *= y; }
    Mint operator-() const { return val ? Mint(MOD - val) : Mint(); }
    Mint pow(int n) const {
        Mint res = 1, b = *this;
        for (; n; n >>= 1, b *= b)
            if (n & 1) res *= b;
        return res;
    }
    Mint(int v=0) : val(v) {}
private: int val;
} f[N][N][2];
inline uint64_t calc(int l, int r) {
    return hash[r] - hash[l-1] * base[r - l + 1];
}
inline uint64_t rcalc(int l, int r) {
    return rhash[l] - rhash[r+1] * base[r - l + 1];
}
int main() {
    int n, q;
    scanf("%d%d %s", &n, &q, s+1);
    base[0] = 1;
    for (int i = 1; i <= n; i++) base[i] = base[i-1] * B;
    for (int i = 1; i <= n; i++) hash[i] = hash[i-1] * B + (s[i] - 'a');
    for (int i = n; i >= 1; i--) rhash[i] = rhash[i+1] * B + (s[i] - 'a');
    for (int l = 1; l <= n; l++)
        for (int i = 1, j = l; j <= n; i++, j++) {
            f[i][j][0] = ;
        }
}