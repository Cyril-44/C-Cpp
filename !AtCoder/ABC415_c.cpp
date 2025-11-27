#include <cstdio>
#include <cstring>
const int N = 500005;
char s[N]; // 1->dangerous 0->safe
bool f[2][N]; // 0->dangerous 1->safe
int main() {
    int t, n;
    scanf("%d", &t);
    while (t--) {
        scanf("%d %s", &n, s+1);
        memset(f, 0, sizeof f);
        f[0][0] = 1;
        for (int i = 1; i <= n; i++) {
            memset(f[i&1], 0, sizeof f >> 1);
            for (int j = (1<<n)-1; j >= 0; j--)
                for (int k = 0; k < n; k++)
                    if (s[j | 1<<k] ^ '1')
                    f[i&1][j | 1<<k] |= f[i&1^1][j];
        }
        puts(f[n&1][(1<<n)-1] ? "Yes" : "No");
    }
    return 0;
}