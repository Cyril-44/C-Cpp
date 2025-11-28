#include <stdio.h>
const int N = 200005;
int p[N];
namespace Dat3 {
    const int N = 2005;
    int a[N][N];
    inline void work(int n) {
        for (int i = 1; i <= n; i++)
            a[i][p[i]] = 1;
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++)
                a[i][j] += a[i-1][j] + a[i][j-1] - a[i-1][j-1];
        long long ans = 0;
        for (int i = n>>1; i <= n; i++)
            for (int j = n>>1; j <= n; j++)
                switch (a[i][j] - a[i - (n>>1)][j] - a[i][j - (n>>1)] + a[i - (n>>1)][j - (n>>1)]) {
                case 1: ++ans; break;
                case 0: ans += (n/2) * (n/2-1); break;
                default: break;
                }
        printf("%d\n", ans);
    }
}
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        scanf("%d", &p[i]);
    Dat3::work(n);
    return 0;
}