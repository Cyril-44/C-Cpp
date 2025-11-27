#include <cstdio>
#include <cstring>
#include <algorithm>
using std::min;
using i64 = long long;
class Matrix {
    static constexpr int N = 101;
    i64 a[N][N];
public:
    inline Matrix() { memset(a, 0x3f, sizeof a); }
    inline Matrix(int) { memset(a, 0x3f, sizeof a); for (int i = 1; i < N; i++) a[i][i] = 0; }
    inline Matrix(i64 *arr[N]) { memcpy(a, arr, sizeof a); }
    inline const i64* operator[] (const size_t &rhs) const { return a[rhs]; }
    inline i64* operator[] (const size_t &rhs) { return a[rhs]; }
    inline Matrix operator* (const Matrix& rhs) const {
        Matrix ret;
        for (int k = 1; k < N; k++)
            for (int i = 1; i < N; i++)
                for (int j = 1; j < N; j++)
                    ret[i][j] = min(ret[i][j], a[i][k] + rhs[k][j]);
        return ret;
    }
    inline Matrix& operator^= (size_t n) {
        Matrix b(*this);
        *this = Matrix(1);
        while (n) {
            if (n & 1) *this = *this * b;
            b = b * b;
            n >>= 1;
        }
        return *this;
    }
};
i64 f[201][201];
int main() {
    int t, n, m, c;
    scanf("%d", &t);
    while (t--) {
        scanf("%d%d%d", &n, &m, &c);
        memset(f, 0x3f, sizeof f);
        for (int i = 1; i <= (n<<1); i++) f[i][i] = 0;
        for (int u, v, w; m--;) {
            scanf("%d%d%d", &u, &v, &w);
            f[u][v] = min(f[u][v], (i64) w);
            f[u][v+n] = min(f[u][v+n], (i64) -w);
            f[u+n][v+n] = min(f[u][v], (i64) w);
        }
        for (int k = 1; k <= (n<<1); k++)
            for (int i = 1; i <= (n<<1); i++)
                for (int j = 1; j <= (n<<1); j++)
                    f[i][j] = min(f[i][j], f[i][k] + f[k][j]);
        if (!c) { printf("%lld\n", f[1][n]); continue; }
        Matrix g;
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++)
                g[i][j] = f[i][j+n];
        g ^= c;
        printf("%lld\n", g[1][n]);
    }
    return 0;
}