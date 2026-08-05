#include <bits/stdc++.h>
constexpr int N = 505;
int f[N][N];
inline void umin(int &x, int y) { if (x > y) x = y; }
int main() {
    int T, p, k, q;
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d%d", &p, &k, &q);
        memset(f, 0x3f, sizeof f);
        for (int i = 0; i < p; i++) f[i][i] = 0;
        for (int x = 0; x < p; x++)
            for (int i = 0; i < k; i++)
                umin(f[x][(k*x+i) % p], i);
        for (int k = 0; k < p; k++)
            for (int i = 0; i < p; i++)
                for (int j = 0; j < p; j++)
                    if (k != i && k != j && i != j)
                        umin(f[i][j], f[i][k] + f[k][j]);
        while (q--) {
            int u, v;
            scanf("%d%d", &u, &v);
            printf("%d\n", f[u][v]);
        }
    }
    return 0;
}