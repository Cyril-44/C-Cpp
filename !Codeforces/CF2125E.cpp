#include <cstdio>
const int N = 645, M = 200005, MOD = 998244353;
int f[N][M];
int main() {
    int t, n, x;
    scanf("%d", &t);
    for (int j = 1; j <= 200000; j++)
        f[1][j] = j;
    for (int i = 2; i <= 640; i++) {
        for (int j = 1; j <= 200000; j++) {
            f[i][j] = (f[i-1][j-1] + f[i][j-1]) % MOD;
            if (j > i-1) f[i][j] = ((f[i][j] - f[i-1][j-i+1]) % MOD + MOD) % MOD;
        }
        for (int j = 1; j <= 200000; j++)
            (f[i][j] += f[i][j-1]) %= MOD;
    }
    while (t--) {
        scanf("%d%d", &n, &x);
        if (n <= 640) printf("%d\n", f[n][x]);
        else puts("0");
    }
    return 0;
}
