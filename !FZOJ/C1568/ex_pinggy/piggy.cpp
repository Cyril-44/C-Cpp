#include <cstdio>
constexpr int N = 3005;
int C[N][N], f[N], f1[N], g[N][N], Mod;
inline void add(int &x, int y) { if ((x += y) >= Mod) x -= Mod; }
inline void sub(int &x, int y) { if ((x -= y) < 0) x += Mod; }
inline int sub_cp(int x, int y) { return sub(x, y), x; }
int main() {
    int n;
    scanf("%d%d", &n, &Mod);
    for (int i = 0; i <= n; i++) {
        C[i][0] = C[i][i] = 1;
        for (int j = 1; j < i; j++)
            add(C[i][j] = C[i-1][j], C[i-1][j-1]);
    }
    f1[1] = 1;
    for (int i = 2; i <= n; i++)
        f1[i] = ((i-1ull) * f1[i-1] + (i-2ull) * f1[i-2]) % Mod;
    for (int j = 1; j <= n; j++)
        for (int k = 1; k <= n; k++) {
            const int upp = (n-1) / k;
            bool sb = false;
            for (int i = 0; i <= upp; i++, sb ^= 1)
                if (sb) sub(g[j][k], 1ull * C[n-i*k-1][j-1] * C[j][i] % Mod);
                else    add(g[j][k], 1ull * C[n-i*k-1][j-1] * C[j][i] % Mod);
        }
    int ans = 0;
    for (int j = 1; j <= n; j++) {
        for (int i = 1; i <= n; i++)
            add(f[j], 1ull * sub_cp(g[i][j], g[i][j-1]) * f1[i] % Mod);
        add(ans, 1ull * j * f[j] % Mod);
    }
    printf("%d\n", ans);
    return 0;
}