#include <bits/stdc++.h>
constexpr int N = 105;
int f[N][N];
int C[N][N], F[N];

int main(int argc, char** argv) {
    int n = atoi(argv[1]);
    for (int i = 0; i <= n; i++) {
        C[i][0] = C[i][i] = 1;
        for (int j = 1; j < i; j++)
            C[i][j] = C[i-1][j-1] + C[i-1][j];
    }
    F[0] = 1;
    for (int i = 1; i <= n; i++) F[i] = F[i-1] * i;
    f[0][0] = 1;
    for (int i = 1; i <= n; i++)
        for (int j = i; j <= n; j++)
            for (int k = 1; k <= n; k++)
                f[j][k] += C[j][i] * f[j-i][k-1] * F[i];
    int ans = 0;
    for (int k = n-1; k >= 1; k--)
        f[n][k] -= f[n][k+1];
    for (int k = 1; k <= n; k++)
        ans += k * f[n][k];
    printf("%d\n", ans);
    return 0;
}