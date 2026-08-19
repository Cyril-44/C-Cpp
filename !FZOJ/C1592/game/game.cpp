#include <cstdio>
constexpr int N = 1000004, MOD = 998244353;
int a[N];
struct Mint {
    template<typename T> explicit operator T() const { return static_cast<T>(val); }
    Mint& operator+=(Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint a, Mint b) { return a += b; }
    friend Mint operator-(Mint a, Mint b) { return a -= b; }
    friend Mint operator*(Mint a, Mint b) { return a *= b; }
    Mint(int o = 0) : val(o) {}
private: int val;
} f[N], g[N], inv[N];
bool np[N];
int p[N], phi[N];
int main() {
    constexpr int N = (int)1e6;
    phi[1] = 1;
    for (int i = 2; i <= N; i++) {
        if (!np[i]) p[++p[0]] = i, phi[i] = i-1;
        for (int j = 1; j <= p[0] && i * p[j] <= N; j++) {
            np[i * p[j]] = true;
            if (i % p[j] == 0) {
                phi[i * p[j]] = phi[i] * p[j];
                break;
            }
            phi[i * p[j]] = phi[i] * phi[p[j]];
        }
    }
    inv[1] = 1;
    for (int i = 2; i <= N; i++)
        inv[i] = (MOD - MOD / i) * inv[MOD % i];
    int l, T, n;
    scanf("%d%d", &l, &T);
    f[l+1] = l+1;
    for (int i = l+2; i <= N; i++)
        f[i] = f[i-1] + f[i-1 - l] + 1;
    for (int i = l+1; i <= N; i++)
        for (int j = i, k = 1; j <= N; j += i, k++)
            g[j] += phi[k] * f[i];
    for (int i = 1; i <= N; i++)
        g[i] *= inv[i];
    while (T--) {
        scanf("%d", &n);
        printf("%d %d\n", f[n], g[n]);
    }
    return 0;
}