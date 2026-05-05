#include <cstdio>
#include <vector>
constexpr int N = 505, MOD = 998244353;
int a[N][N];
struct Mint {
    inline Mint(int x = 0) : val(x) {}
    template<typename T> inline explicit operator T() const { return static_cast<T>(val); }
    inline Mint& operator+=(const Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    inline Mint& operator-=(const Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    inline Mint& operator*=(const Mint o) { return *this = *this * o; }
    inline Mint operator+(const Mint o) const { return Mint(*this) += o; }
    inline Mint operator-(const Mint o) const { return Mint(*this) -= o; }
    inline Mint operator*(const Mint o) const { return Mint(1ull * val * o.val % MOD); }
    private: int val;
};
std::vector<Mint> f[N];
Mint qpow(Mint b, int n) {
    Mint res = 1;
    while (n) {
        if (n & 1) res *= b;
        b *= b, n >>= 1;
    }
    return res;
}
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++) {
            scanf("%d", &a[i][j]);
            f[a[i][j]].emplace_back(1);
        }
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++) {
            f[a[i][j]].emplace_back(0);
            for (int k = f[a[i][j]].size() - 2; k >= 0; k--)
                f[a[i][j]][k + 1] += f[a[i][j]][k], f[a[i][j]][k] *= (m - 1);
        }
    Mint ans = qpow(m, n) * m;
    for (int i = 1; i <= m*n; i++)
        for (int j = 2; j < (int)f[i].size(); j++)
            ans -= f[i][j] * (j-1);
    printf("%d\n", int(ans));
    return 0;
}