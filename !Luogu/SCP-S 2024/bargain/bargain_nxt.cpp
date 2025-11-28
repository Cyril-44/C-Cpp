#include <stdio.h>
#include <string.h>
const int N = 100005, B[] = {
    0, 1, 10, 100, 1000, 10000,
    100000, 1000000,
};
char s[N];
int f[N][8];
inline int min(const int &x, const int &y) {
    return x < y ? x : y;
}
int main() {
#ifndef ONLINE_JUDGE
    freopen("bargain.in", "r", stdin);
    freopen("bargain.out", "w", stdout);
#endif
    int t, n;
    scanf("%*d%d", &t);
    while (t--) {
        int val[10] = {};
        scanf("%s", s);
        n = strlen(s);
        for (int i = 1; i <= 9; i++)
            scanf("%d", &val[i]);
        for (int i = 0; i < 8; i++)
            f[n][i] = 0;
        for (int i = n-1; i >= 0; i--) {
            f[i][0] = 0;
            for (int j = 1; j < 8; j++)
                f[i][j] = min(f[i+1][j], f[i+1][j-1] - val[s[i] ^ '0'] + B[j] * (s[i] ^ '0'));
        }
        long long ans = 0;
        for (int i = 0; i < n; i++)
            for (int j = 1; j < 8; j++)
                if (f[i][j] < ans)
                    ans = f[i][j];
        for (int i = 0; i < n; i++)
            ans += val[s[i] ^ '0'];
        printf("%lld\n", ans);
    }
    return 0;
}