#include <stdio.h>
#include <string.h>

typedef long long x1;
x1 x2;
char x3[5010], x4[12];
int x5;
const x1 x6 = 998244353;

typedef struct {
    x1 x7[12][12];
} x8;

x8 x9, x10, x11;

x8 x12(x8 a, x8 b) {
    x8 res;
    memset(res.x7, 0, sizeof(res.x7));
    for (int i = 1; i <= x5 + 1; i++)
        for (int j = 1; j <= x5 + 1; j++)
            for (int k = 1; k <= x5 + 1; k++)
                res.x7[i][j] = (res.x7[i][j] + a.x7[i][k] * b.x7[k][j]) % x6;
    return res;
}

x8 x13(x8 a, x1 b) {
    x8 res = x9;
    while (b) {
        if (b & 1)
            res = x12(res, a);
        a = x12(a, a);
        b >>= 1;
    }
    return res;
}

x1 x14[5010][12];

int main() {
    scanf("%lld%s%s", &x2, x3 + 1, x4 + 1);
    x5 = strlen(x4 + 1);
    int x15 = strlen(x3 + 1);
    for (int i = 1; i <= x5 + 1; i++)
        x9.x7[i][i] = 1;
    for (int i = 1; i <= x5; i++) {
        memset(x14, 0, sizeof(x14));
        x14[0][i] = 1;
        x3[0] = x4[i];
        for (int j = 1; j <= x15; j++) {
            for (int p = 1; p <= x5; p++)
                if (x4[p] == x3[j]) {
                    int cur = p == 1 ? x5 : p - 1;
                    for (int q = 0; q < j; q++)
                        if (x3[q] == x4[cur])
                            x14[j][p == x5 ? p + 1 : p] = (x14[j][p == x5 ? p + 1 : p] + x14[q][cur]) % x6;
                }
        }
        for (int j = 1; j <= x5 + 1; j++) {
            for (int p = 0; p <= x15; p++) {
                x10.x7[i][j] = (x10.x7[i][j] + x14[p][j]) % x6;
            }
        }
    }
    x10.x7[x5 + 1][x5 + 1] = 1;
    x11.x7[1][x5] = 1;
    x10 = x13(x10, x2);
    x11 = x12(x11, x10);
    printf("%lld\n", x11.x7[1][x5 + 1]);
    return 0;
}
