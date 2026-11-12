#include <bits/stdc++.h>
constexpr int N = 2005, MOD = 998244353;
#define For(i, s, t) for (int i = (s); i <= (t); i++)
#ifdef CLANGD
#define N 2
#endif
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
} fac[N*2], ifac[N*2], C[N*2][N*2];
char s[N];
inline int mapping(char ch) {
    switch (ch) {
    case 'A': return 0;
    case 'G': return 1;
    case 'C': return 2;
    case 'U': return 3;
    }
    return -1;
}
int n, h[4], v[4];
using Vec = std::vector<Mint>;
Vec operator*(const Vec &x, const Vec &y) {
    Vec res(x.size() + y.size() - 1);
    for (int i = 0; i < (int)x.size(); i++)
        for (int j = 0; j < (int)y.size(); j++)
            res[i+j] += x[i] * y[j];
    return res;
}
Vec& operator*=(Vec &x, const Vec &y) { return x = x * y; }
Vec& operator*=(Vec &x, Mint y) { for (auto &i : x) i *= y; return x; }
int main() {
    freopen("rna.in", "r", stdin);
    freopen("rna.out", "w", stdout);
    for (int i = 0; i <= 4000; i++) {
        C[i][0] = C[i][i] = 1;
        for (int j = 1; j < i; j++)
            C[i][j] = C[i-1][j-1] + C[i-1][j];
    }
    fac[0] = 1;
    for (int i = 1; i <= 4000; i++) fac[i] = fac[i-1] * i;
    ifac[4000] = fac[4000].pow(MOD-2);
    for (int i = 4000; i >= 1; i--) ifac[i-1] = ifac[i] * i;
    
    scanf("%s%d%d%d%d", s+1, v, v+1, v+2, v+3);
    int m = v[0] + v[1] + v[2] + v[3];
    n = strlen(s+1);
    if (s[1] == '#') n = 0;
    for (int i = 1; i <= n; i++)
        ++h[mapping(s[i])];
    
    Vec base{1};
    for (int x = 0; x < 4; x++) if (h[x]) {
        Vec vec(v[x] + 1);
        for (int i = 0; i <= v[x]; i++) {
            vec[i] = (h[x] ? C[i + h[x] - 1][i] : 1) * ifac[v[x]-i];
            if (i & 1) vec[i] = -vec[i];
        }
        base *= vec;
    }
    else base *= ifac[v[x]];
    for (int i = 0; i < (int)base.size(); i++)
        base[i] *= ifac[n+i];
    base *= fac[n + m];
    printf("%d\n", std::accumulate(base.begin(), base.end(), Mint()));
}