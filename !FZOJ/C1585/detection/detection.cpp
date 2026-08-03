#include <cstdio>
#include <cstring>
#include <vector>
constexpr int N = 61;
int f[N][N][N][N];
inline void umin(int &x, int y) { if (x > y) x = y; }
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++) {
            char ch;
            scanf(" %c", &ch);
            f[i][i][j][j] = ch == 'T';
        }
    for (int lx = 1; lx <= n; lx++)
        for (int ly = 1; ly <= n; ly++)
            if (lx != 1 || ly != 1)
                for (int il = 1, ir = lx; ir <= n; il++, ir++)
                    for (int jl = 1, jr = ly; jr <= n; jl++, jr++) {
                        f[il][ir][jl][jr] = std::max(lx, ly);
                        for (int i = il; i < ir; i++)
                            umin(f[il][ir][jl][jr], f[il][i][jl][jr] + f[i+1][ir][jl][jr]);
                        for (int j = jl; j < jr; j++)
                            umin(f[il][ir][jl][jr], f[il][ir][jl][j] + f[il][ir][j+1][jr]);
                    }
    printf("%d\n", f[1][n][1][n]);
    return 0;
}