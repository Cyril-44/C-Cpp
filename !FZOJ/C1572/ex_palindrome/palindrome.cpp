#include <cstdio>
#include <cstdint>
#include <filesystem>
#include <numeric>
constexpr int N = 6005, H = 80, MOD = (int)1e9 + 7;
int cnt[H], h[H];
struct ModInt {
    inline ModInt(int x = 0) : val(x) {}
    template<typename T> inline explicit operator T() const { return static_cast<T>(val); }
    inline ModInt& operator+=(const ModInt o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    inline ModInt& operator-=(const ModInt o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    inline ModInt& operator*=(const ModInt o) { return *this = *this * o; }
    inline ModInt operator+(const ModInt o) const { return ModInt(*this) += o; }
    inline ModInt operator-(const ModInt o) const { return ModInt(*this) -= o; }
    inline ModInt operator*(const ModInt o) const { return ModInt(1ull * val * o.val % MOD); }
    private: int val;
} g[2][H][H][H];

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1, bi; i <= n; i++)
        scanf("%d", &bi), ++cnt[bi];
    for (int i = 1; i <= H; i++) ++h[cnt[i]];
    for (int k = H; k > 2; k--) {
        for (int i = 0; i <= H; ) {
            g[i][1];
        }
    }

    ModInt ans;
    printf("%d %d\n", std::accumulate(cnt+1, cnt+1+H, 0, [](int acc, int x){return acc + x*(x+1)/2;}), int(ans));
    return 0;
}