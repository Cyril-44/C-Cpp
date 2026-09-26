#include <cstdio>
#include <cmath>
#include <cstring>
#include <numeric>
constexpr int N = 604, MOD = (int)1e9 + 7;
int a[N];
inline bool isSquare(int i, int j) {
    int64_t prod = 1ll * a[i] * a[j], root = sqrtl(prod);
    return root * root == prod;
}
int fa[N], sz[N];
inline int find(int u) { return u == fa[u] ? u : (fa[u] = find(fa[u])); }
inline void unite(int x, int y) { fa[find(x)] = find(y); }
struct Mint {
    Mint& operator+=(Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint x, Mint y) { return x += y; }
    friend Mint operator-(Mint x, Mint y) { return x -= y; }
    friend Mint operator*(Mint x, Mint y) { return x *= y; }
    Mint(int v=0) : val(v) {}
private: int val;
} C[N][N], F[N], f[N][N], g[N][N], h[N][N];
int main() {
    for (int i = 0; i <= 600; i++) {
        C[i][0] = C[i][i] = 1;
        for (int j = 1; j < i; j++)
            C[i][j] = C[i-1][j-1] + C[i-1][j];
    }
    F[0] = 1;
    for (int i = 1; i <= 600; i++) F[i] = F[i-1] * i;
    int T, n;
    scanf("%d", &T);
    while (T--) {
        scanf("%d", &n);
        memset(f, 0, sizeof f);
        std::iota(fa+1, fa+1+n, 1);
        memset(sz, 0, sizeof sz);
        for (int i = 1; i <= n; i++) {
            scanf("%d", &a[i]);
            for (int j = 1; j < i; j++)
                if (isSquare(i, j))
                    unite(i, j);
        }
        for (int i = 1; i <= n; i++) ++sz[find(i)];
        f[0][0] = 1;
        for (int k = 1; k <= n; k++) {
            static Mint coef[N];              // 复用缓冲区，避免反复分配；MAXN 和 f/g 的边长一致即可
            if (sz[k] < 2) continue;
            memcpy(g, f, sizeof g);

            for (int x = 1; x < sz[k]; x++) {
                for (int y = 1; y <= x && x + y <= sz[k]; y++) {
                    // 只和 k,x,y 有关，原来在 O(n^2) 内层重复算，现在只算一次
                    Mint base = C[sz[k]][x + y] * F[x + y] * C[x - 1][y - 1];

                    // C[j+y][j] 只和 j,y 有关，和 i 无关，
                    // 提到 i 循环外面，每个 j 只算一次而不是对每个 i 都算
                    for (int j = 0; j + x < n; j++)
                        coef[j] = base * C[j + y][j];

                    for (int i = 0; i + x < n; i++) {
                        const Mint *fi = f[i];
                        Mint *gi = g[i + x];
                        for (int j = 0; j + x < n; j++)
                            gi[j + y] += fi[j] * coef[j];   // 现在只剩 1 次乘法
                    }
                }
            }
            memcpy(f, g, sizeof g);
        }
        Mint ans = F[n];
        for (int i = 1; i < n; i++) {
            Mint sum = 0;
            for (int j = 1; j <= i && i+j <= n; j++)
                sum += f[i][j] * C[n-i][j] * F[n-i-j];
            if (i & 1) ans -= sum;
            else ans += sum;
        }
        printf("%d\n", ans);
    }
}