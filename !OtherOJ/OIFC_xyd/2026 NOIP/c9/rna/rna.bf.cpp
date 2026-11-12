#include <bits/stdc++.h>
#define For(i, s, t) for (int i = (s); i <= (t); i++)
constexpr int N = 31, MOD = 998244353;
#ifdef CLANGD
#define N 3
#endif
struct Mint {
    Mint& operator+=(const Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(const Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(const Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint x, const Mint y) { return x += y; }
    friend Mint operator-(Mint x, const Mint y) { return x -= y; }
    friend Mint operator*(Mint x, const Mint y) { return x *= y; }
    Mint(int v=0) : val(v) {}
private: int val;
} f[2][N][N][N][N][N];
char s[N];
int main() {
    int n, a, g, c, u;
    scanf("%s%d%d%d%d", s+1, &a, &g, &c, &u);
    n = strlen(s+1);
    if (s[1] == '#') n = 0;
    auto fnow = f[0], fnxt = f[1];
    fnow[0][0][0][0][0] = 1;
    for (int k = n + a + g + c + u; k--; ) {
        For(i,0,n) For(j,0,a) For(k,0,g) For(l,0,c)
            memset(fnxt[i][j][k][l], 0, sizeof(Mint) * (u+1));
        For(i,0,n) For(j,0,a) For(k,0,g) For(l,0,c) For(m,0,u) {
            fnxt[i+1][j][k][l][m] += fnow[i][j][k][l][m];
            if (s[i+1] != 'A') fnxt[i][j+1][k][l][m] += fnow[i][j][k][l][m];
            if (s[i+1] != 'G') fnxt[i][j][k+1][l][m] += fnow[i][j][k][l][m];
            if (s[i+1] != 'C') fnxt[i][j][k][l+1][m] += fnow[i][j][k][l][m];
            if (s[i+1] != 'U') fnxt[i][j][k][l][m+1] += fnow[i][j][k][l][m];
        }
        std::swap(fnow, fnxt);
    }
    printf("%d\n", fnow[n][a][g][c][u]);
    return 0;
}