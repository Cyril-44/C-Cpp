#include <stdio.h>
constexpr int N = 105;
int mp[N][N];
long long f[N][N];
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            scanf("%d", &mp[i][j]);
    f[1][1] = 1;
    mp[1][1] = false;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            if (mp[i][j]) f[i][j] = f[i-1][j] + f[i][j-1];
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            printf("%8lld%c", f[i][j], (j^n)?' ':'\n');
}