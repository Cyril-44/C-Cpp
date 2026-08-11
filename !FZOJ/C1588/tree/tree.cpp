#include <cstdio>
#include <cstring>
#include <algorithm>
constexpr int N = 305, MOD = (int)1e9 + 7;
struct Mint {
    Mint& operator+=(Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint a, Mint b) { return a += b; }
    friend Mint operator-(Mint a, Mint b) { return a -= b; }
    friend Mint operator*(Mint a, Mint b) { return a *= b; }
    Mint(int o = 0) : val(o) {}
private: int val;
} f[2][N][N];
int l[N], r[N];
// f[i][j][k] 表示已经钦定了前 i 个点的形态，有 j 个儿子的位置，有 k 个父亲待钦定
int main() {
    int Tid, T;
    scanf("%d%d", &Tid, &T);
    while (T--) {
        int n;
        scanf("%d", &n);
        for (int i = 1; i <= n; i++)
            scanf("%d%d", &l[i], &r[i]);
        memset(f[1], 0, sizeof(f) / 2);
        if (l[1] <= 0 && 0 <= r[1]) f[1][0][1] += 1;
        if (l[1] <= 1 && 1 <= r[1]) f[1][1][1] += 2;
        if (l[1] <= 2 && 2 <= r[1]) f[1][2][1] += 1;
        for (int i = 2; i <= n; i++) {
            memset(f[i&1], 0, sizeof(f) / 2);
            for (int j = 0, _jup = std::min((i-1)*2,  n); j <= _jup; j++)
                for (int k = 0; k < i; k++) { // 要的儿子都 > 本身，儿子 < 待钦定的父亲
                    if (l[i] <= 0 && 0 <= r[i]) {
                        f[i&1][j][k+1] += f[i&1^1][j][k];
                        if (j) f[i&1][j-1][k] += f[i&1^1][j][k] * j;
                    }
                    if (l[i] <= 1 && 1 <= r[i]) {
                        f[i&1][j][k] += f[i&1^1][j][k] * j * 2;
                        f[i&1][j+1][k+1] += f[i&1^1][j][k] * 2;
                    }
                    if (l[i] <= 2 && 2 <= r[i]) {
                        f[i&1][j+2][k+1] += f[i&1^1][j][k];
                        f[i&1][j+1][k] += f[i&1^1][j][k] * j;
                        if (k) f[i&1][j][k-1] += f[i&1^1][j][k] * j * 2 * (k-1);
                        f[i&1][j+1][k] += f[i&1^1][j][k] * k * 2;
                    }
                }
        }
        printf("%d\n", f[n&1][0][1]);
    }
    return 0;
}