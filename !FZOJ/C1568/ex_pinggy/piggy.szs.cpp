#include <algorithm>
#include <cstdio>
using namespace std;
#define N 3005
inline int read() {
    int a = 0, f = 1;
    char c = getchar();
    while (c < '0' || c > '9') {
        if (c == '-') f = -1;
        c = getchar();
    }
    while (c >= '0' && c <= '9') {
        a = a * 10 + c - '0';
        c = getchar();
    }
    return a * f;
}
int ls[N], n, mod;
int jc[N], c[N][N];
int main() {
    n = read(), mod = read();
    for (int i = 0; i <= n; i++) {
        c[i][0] = c[i][i] = 1;
        for (int j = 1; j < i; j++) c[i][j] = (c[i - 1][j - 1] + c[i - 1][j]) % mod;
    }
    jc[0] = 1;
    for (int i = 1; i <= n; i++) jc[i] = jc[i - 1] * 1ll * i % mod;
    for (int m = 1; m <= n; m++)
        for (int i = 0, k1 = 1; i <= m - 1; i++, k1 *= -1)
            (ls[m] += (mod + k1 * c[m - 1][i] * 1ll * jc[m - i] % mod) % mod) %= mod;
    int ans = 0;
    for (int m = 1; m <= n; m++) {
        int s2 = n * 1ll * c[n - 1][m - 1] % mod;
        for (int k1 = -1, j = 1; j <= m; j++, k1 *= -1) {
            int s1 = 0;
            for (int k = 0; k <= (n - m) / j; k++) (s1 += c[n - j * k - 1][m - 1]) %= mod;
            (s2 += (mod + k1 * c[m][j] * 1ll * s1 % mod) % mod) %= mod;
        }
        ans = (ans + s2 * 1ll * ls[m] % mod) % mod;
    }
    ans = (jc[n] * 1ll * n % mod - ans + mod) % mod;
    printf("%d\n", ans);
    return 0;
}]