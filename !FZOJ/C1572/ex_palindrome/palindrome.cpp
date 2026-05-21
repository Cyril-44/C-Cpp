#include <cstdio>
#include <cstdint>
#include <cstring>
#include <numeric>
#include <algorithm>
constexpr int N = 6405, H = 80, MOD = (int)1e9 + 7;
int cnt[H+1], h[N];
struct Mint {
    inline Mint(int x = 0) : val(x) {}
    template<typename T> inline explicit operator T() const { return static_cast<T>(val); }
    inline Mint& operator+=(const Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    inline Mint& operator-=(const Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    inline Mint& operator*=(const Mint o) { return *this = *this * o; }
    inline Mint operator+(const Mint o) const { return Mint(*this) += o; }
    inline Mint operator-(const Mint o) const { return Mint(*this) -= o; }
    inline Mint operator*(const Mint o) const { return Mint(1ull * val * o.val % MOD); }
    private: int val;
} f[2][H+1][H+1][H+1], g[H+1][H+1][H+1];
/* 可通过打表找到规律，长度为 3 及以上的只有 ababab 或者 abababa 或者 aaaa 这种形式的，而且后两者是可以被长度为 2 的包住的。
 * f[now][i][j][k] 表示当前形如 "ababab" 的有 i 段，形如 "ababa"/“aaa" 的有 j 段，还有 k 组个数为 now 的数。
 * g[i][j][k] 是暂时的数组，用来储存只考虑 "aaa" 的段之后
 */
inline Mint qpow(Mint b, int n) {
    Mint res = 1;
    while (n) {
        if (n & 1) res *= b;
        b *= b, n >>= 1;
    }
    return res;
}
Mint C[H+1][H+1], pw2[N];
Mint A[H+1][H+1];
int main() {
    for (int i = 0; i <= H; i++) {
        C[i][0] = C[i][i] = 1;
        for (int j = 1; j < i; j++)
            C[i][j] = C[i-1][j-1] + C[i-1][j];
    }
    for (int i = 0; i <= H; i++) {
        A[i][0] = 1;
        for (int j = 0; j < H; j++)
            A[i][j+1] = A[i][j] * (H - j);
    }
    pw2[0] = 1;
    for (int i = 1; i <= H*H; i++) pw2[i] = pw2[i-1] + pw2[i-1];
    int n;
    scanf("%d", &n);
    for (int i = 1, bi; i <= n; i++)
        scanf("%d", &bi), ++cnt[bi];
    for (int i = 1; i <= H; i++) ++h[cnt[i]];
    int curtop = 0;
    auto fnow = f[0], fpre = f[1];
    int asum = 0, bsum = 0;
    for (int val = H*H+1; val > 2; val--) {
        if (h[val] == 0) continue;
        memset(g, 0, sizeof g);
        for (int i = 0; i <= asum; i++)
            for (int j = 0; j <= bsum; j++)
                for (int k = 0; k <= h[val]; k++)
                    for (int l = 0; l <= k; l++) // 构造 "aaa" 的数量
                        g[i][j + l][k - l] += fnow[i][j][k] * C[k][l];
        bsum += h[val];
        std::swap(fnow, fpre);
        memset(fnow, 0, sizeof f >> 1);
        for (int i = 0; i <= asum; i++)
            for (int j = 0; j <= bsum; j++)
                for (int k = 0; k <= h[val]; k++)
                    for (int l = k & 1, Up = std::min(k, h[val - 1]); l <= Up; l += 2) // 构造 "ababa" 的数量
                        fnow[i + (k-l)/2][j + l][k - l] += g[i][j][k] * C[k][l] * C[h[val-1]][l];
    }
    Mint ans;
    printf("%d %d\n", std::accumulate(cnt+1, cnt+1+H, 0, [](int acc, int x){return acc + x*(x+1)/2;}), int(ans));
    return 0;
}
/*
.l-date-time .date

*/