#include <cstdio>
#include <cstring>
#define For(i, s, t) for (int i = (s); i <= (t); i++)
constexpr int N = 25;
#ifdef DEBUG
#define debug(...) fprintf(stderr, __VA_ARGS__)
#else
#define debug(...) ((void)0)
#endif
unsigned m;
int n;
struct ModInt {
    inline ModInt(int x = 0) : val(x) {}
    template<typename T> inline explicit operator T() const { return static_cast<T>(val); }
    inline ModInt& operator+=(ModInt o) { if ((val += o.val) >= m) val -= m; return *this; }
    inline ModInt& operator-=(ModInt o) { if ((val -= o.val) < 0) val += m;  return *this; }
    inline ModInt& operator*=(ModInt o) { val = 1ull * val * o.val % m;      return *this; }
    inline ModInt operator+(ModInt o) const { return ModInt(*this) += o; }
    inline ModInt operator-(ModInt o) const { return ModInt(*this) -= o; }
    inline ModInt operator*(ModInt o) const { return ModInt(*this) *= o; }
    private: int val;
} f[N][N][N]; // f[i][j][k] 表示到第i个位置，t出现了j次，p[i]=k。
inline int solve(int t) {
    memset(f, 0, sizeof f);
    f[0][0][0] = 1;
    For(j, 0, n) {
        For(i, 1, n) {
            For(k, 1, n) {
                if (k < t) {
                    if (!j)    f[i][j][k] = f[i-1][j][k] * (k-1) + f[i-1][j][k-1] + f[i-1][j][k];
                } else if (k == t) {
                    if (j > 0) f[i][j][k] = f[i-1][j][k] * (k-1) + f[i-1][j-1][k] + f[i-1][j-1][k-1];
                } else if (k > t) {
                    if (j > 0) f[i][j][k] = f[i-1][j][k] * (k-1) + f[i-1][j-1][k] + f[i-1][j][k-1];
                }
            }
        }
        debug("j=%d\n", j);
        debug("k:\t");
        For(k, 1, n) debug("%3d%c", k, k==n?'\n':',');
        For(i, 1, n) {
            debug("i%d\t", i);
            For(k, 1, n) debug("%3d%c", int(f[i][j][k] - f[i][j][k-1]), k==n?'\n':' ');
        }
    }
    ModInt occ[N];
    For(j, 1, n) For (k, 1, n) occ[j] += f[n][j][k];
    For(i, 1, n) debug("%d%c", int(occ[i]), i==n?'\n':' ');
    ModInt ans;
    for (int k = 1; k <= n; k++)
        ans += occ[k] * k * k;
    return int(ans);
}
int main() {
    int T; scanf("%d", &T);
    while (T--) {
        scanf("%d%u", &n, &m);
        for (int i = 1; i <= n; i++)
            printf("%d ", solve(i));
        putchar('\n');
    }
    return 0;
}