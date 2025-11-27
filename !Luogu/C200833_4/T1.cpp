#include <stdio.h>
const int N = 105;
int dep[N][N];
inline void in(bool &x) {
    char ch = getchar();
    while (ch < '0' || ch > '9')
        ch = getchar();
    for (x = 0; ch >= '0' && ch <= '9'; ch = getchar())
        x = (x << 3) + (x << 1) + (ch ^ '0');
}
inline void solve(const int &n) {
    static bool a[N], f[N];
    for (int i = 1; i <= n; i++)
        in(a[i]), f[i] = true;
    int ans = 0;
    for (int i = 1; i <= n; i++) {
        if (f[i] && a[i]) ++ans;
        else {
            for (int j = 1; j <= dep[i][0]; j++)
                f[dep[i][j]] = false;
        }
    }
    printf("%d\n", ans);
}
int main() {
    int n, m, d, di;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &d);
        while (d--) {
            scanf("%d", &di);
            dep[di][++dep[di][0]] = i;
        }
    }
    scanf("%d", &m);
    while (m--) solve(n);
    return 0;
}