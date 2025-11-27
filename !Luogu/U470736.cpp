#include <stdio.h>
const int N = 500005, MOD = 1e9+7;
int a[N];
inline void in(int &x) {
    char ch = getchar();
    while (ch < '0' || ch > '9')
        ch = getchar();
    for (x = 0; ch >= '0' && ch <= '9'; ch = getchar())
        x = (x << 3) + (x << 1) + (ch ^ '0');
}
namespace DatNle1k{
    const int N = 1005;
    int f[N][2], g[N][2];
    inline void work(const int &n) {
        f[0][0] = f[0][1] = 1;
        int ans1 = 2;
        for (int i = 1; i < n; i++) {
            f[i][0] = f[i][1] = 1;
            for (int j = 0; j < i; j++)
                if (a[i] < a[j])
                    f[i][1] = (f[i][1] + f[j][0]) % MOD;
                else if (a[i] > a[j])
                    f[i][0] = (f[i][0] + f[j][1]) % MOD;
            ans1 = ((ans1 + f[i][0]) % MOD + f[i][1]) % MOD;
        }
        g[n-1][0] = g[n-1][1] = 1;
        for (int i = n-2; i >= 0; i--) {
            g[i][0] = g[i][1] = 1;
            for (int j = n-1; j > i; j--)
                if (a[i] < a[j])
                    g[i][1] = (g[i][1] + g[j][0]) % MOD;
                else if (a[i] > a[j])
                    g[i][0] = (g[i][0] + g[j][1]) % MOD;
        }
        int ans2 = 0;
        for (int i = 1; i < n; i++)
            ans2 = (ans2 + (f[i-1][0] * 1ll * g[i][0]) + (f[i-1][1] * 1ll * g[i][1])) % MOD;
        printf("%d %d\n", (ans1 - n + MOD) % MOD, (ans2 - (n-1) + MOD) % MOD);
    }
}
int main() {
    int n;
    in(n);
    for (int i = 0; i < n; i++)
        in(a[i]);
    if (n == 1) puts("1 1");
    else if (n == 2) {
        if (a[0] ^ a[1]) puts("3 3");
        else puts("2 3");
    }
    else if (n <= 1000) DatNle1k::work(n);
    else puts("FK You");
    return 0;
}