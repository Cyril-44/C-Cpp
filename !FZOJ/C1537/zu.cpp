#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define For(i, s, t) for (int i = (s); i <= (t); ++i)
#define Forv(i, s, t, ...) for (int i = (s), __VA_ARGS__; i <= (t); ++i)
constexpr int N = 30, B = 10, L = 10;
int f[N+1][N+1];
constexpr int SPEC[10][3][6] = {{
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0}
    }, { // 1
        {1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 1}
    }, { // 2
        {1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 1, 1},
        {0, 0, 0, 0, 0, 1}
    }, { // 3
        {1, 1, 1, 1, 1, 1},
        {0, 0, 0, 1, 1, 1},
        {0, 0, 0, 0, 0, 1}
    }, { // 4
        {1, 1, 1, 1, 1, 1},
        {0, 1, 1, 1, 0, 1},
        {0, 0, 0, 1, 1, 1}
    }, { // 5
        {1, 1, 1, 1, 1, 1},
        {0, 1, 1, 1, 1, 1},
        {0, 0, 0, 1, 0, 1}
    }, { // 6
        {1, 1, 1, 1, 1, 1},
        {0, 0, 0, 1, 1, 1},
        {0, 0, 0, 1, 1, 1}
    }, { // 7
        {1, 1, 1, 1, 1, 1},
        {0, 0, 1, 1, 1, 1},
        {0, 0, 0, 0, 1, 1}
    }, { // 8
        {1, 1, 1, 1, 1, 1},
        {1, 1, 1, 0, 0, 1},
        {0, 0, 1, 1, 1, 1}
    }, { // 9
        {1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 0, 1},
        {0, 0, 0, 1, 1, 1}
}};
int main() {
    For (i, 1, N) f[N][i] = f[i][N] = 1;
    char s[L + 5];
    scanf("%s", s);
    int tot = strlen(s);
    for (int i = 1, px = 1, py = 1, nx, ny; i <= tot; px = nx, py = ny, ++i) { // 4*3: C(5,2)=10
        if (i & 1) nx = px + 3, ny = py + 2;
        else nx = px + 2, ny = py + 3;
        For (j, px, nx) For (k, py, ny) f[j][k] = 1;
        int now = s[tot - i] ^ '0';
        if (!now) continue;
        if (i & 1) {
            f[px][ny + 1] = f[px + 1][ny + 1] = 1;
            py = ny + 2;
            For (j, px, px+2) For (k, py, py+5)
                f[j][k] = SPEC[now][j-px][k-py];
            For (k, py+6, N) f[px+2][k] = 1;
            // if (i == 9) ++nx; // 往下弄一格
        } else {
            f[nx + 1][py] = f[nx + 1][py + 1] = 1;
            px = nx + 2;
            For (j, px, px+5) For (k, py, py+2)
                f[j][k] = SPEC[now][k-py][j-px];
            For (j, px+6, N) f[j][py+2] = 1;
            // if (i == 8) ++nx;
        }
        fprintf(stderr, "%d %d\n", px, py);
    }
    puts("30");
    For (i, 1, N) For (j, 1, N) printf("%d%c", f[i][j], j==N?'\n':' ');
}