#include <bits/stdc++.h>
constexpr int N = 505, MOD = (int)1e9 + 7;
int v[N];

struct ModInt {
    inline ModInt(int x = 0) : val(x) {}
    template<class T> inline explicit operator T() const { return static_cast<T>(val); }
    inline ModInt operator+(ModInt o) const { return ModInt(*this) += o; }
    inline ModInt operator-(ModInt o) const { return ModInt(*this) -= o; }
    inline ModInt operator*(ModInt o) const { return ModInt(1ull * val * o.val % MOD); }
    inline ModInt& operator+=(ModInt o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    inline ModInt& operator-=(ModInt o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    inline ModInt& operator*=(ModInt o) { return *this = *this * o; }
    private: int val;
} f[2][N][N];
// f[i][j][k] 表示已经考虑了 v[1..i]，可以往下接的点(v[i]>i)总共有 j 个，前 i 个中有 k 个
bool flg[N]; // 要记录一下有哪些点无法取前面的父亲
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) scanf("%d", &v[i]);
    for (int i = 1; i <= n; i++) {
        if (v[i] && v[v[i]] && v[v[i]] <= v[i])
            { puts("0"); return 0; }
        flg[v[i]] = true;
    }
    for (int i = 0; i <= n; i++) f[0][i][0] = 1;
    for (int i = 1; i <= n; i++) {
        memset(f[i&1], 0, sizeof f >> 1);
        for (int j = 0; j <= n; j++)
            for (int k = 0; k <= j; k++) {
                if (v[i]) {
                    if (v[i] <= i) f[i&1][j][k] = f[i&1^1][j][k];
                    else if (k > 0) f[i&1][j][k] = f[i&1^1][j][k-1];
                } else {
                    if (!flg[i]) f[i&1][j][k] = f[i&1^1][j][k] * k; // v[i] <= i
                    if (k > 0) f[i&1][j][k] += f[i&1^1][j][k-1] * (j - k + 1); // v[i] > i
                }
            }
        /* for (int j = 0; j <= n; j++)
            for (int k = 0; k <= n; k++)
                printf("%d%c", int(f[i&1][j][k]), " \n"[k==n]); */
    }
    ModInt ans(0);
    for (int i = 0; i <= n; i++) {
        ans += f[n&1][i][i];
        // printf("%d\n", int(f[n&1][i][i]));
    }
    printf("%d\n", int(ans));
    return 0;
}