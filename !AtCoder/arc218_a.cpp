#include <cstdio>
#include <vector>
#include <map>
#include <cassert>
constexpr int N = 505, MOD = 998244353;
int a[N][N];
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
} pwm[N];
std::vector<std::pair<Mint, int>> f[N*N]; // f[i][j] 表示 i 在最终集合中出现了 j 次 的集合数，因为压缩，first 表示 DP 值，second 表示跳过更新的轮数
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    pwm[0] = 1;
    for (int i = 1; i <= n; i++) pwm[i] = pwm[i-1] * m;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++) {
            scanf("%d", &a[i][j]);
            if (f[a[i][j]].empty()) f[a[i][j]].emplace_back(1, 0);
        }
    for (int i = 1; i <= n; i++) {
        std::map<int,int> mp;
        for (int j = 1; j <= m; j++) ++mp[a[i][j]];
        for (auto && [x, y] : mp) {
            std::vector<Mint> g(f[x].size() + 1);
            for (int k = 0; k < (int)f[x].size(); k++) {
                Mint pre = f[x][k].first * pwm[i - f[x][k].second - 1];
                g[k+1] += pre * y, g[k] += pre * (m - y);
            }
            f[x].emplace_back();
            for (int k = 0; k < (int)f[x].size(); k++)
                f[x][k] = {g[k], i};
        }
    }
    Mint ans = pwm[n] * n;
    for (int i = 1; i <= m*n; i++)
        for (int j = 2; j < (int)f[i].size(); j++) {
            Mint now = f[i][j].first * pwm[n - f[i][j].second];
            ans -= now * (j - 1);
        }
    printf("%d\n", int(ans));
    return 0;
}