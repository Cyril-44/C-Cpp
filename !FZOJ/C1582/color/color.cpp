#include <cstdio>
#include <cstring>
#include <algorithm>
constexpr int N = 1005, MOD = 998244353;
struct Mint {
    Mint(int x=0) : val(x) {}
    template<class T> explicit operator T() const { return static_cast<T>(val); }
    Mint& operator+=(Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint x, Mint y) { return x += y; }
    friend Mint operator-(Mint x, Mint y) { return x -= y; }
    friend Mint operator*(Mint x, Mint y) { return x *= y; }
private: int val;
} pr[N][N], pc[N][N];
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++)
        for (int j = 0; j <= m; j++)
            scanf("%d", &pr[i][j]);
    for (int j = 1; j <= m; j++)
        for (int i = 0; i <= n; i++)
            scanf("%d", &pc[i][j]);
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            pr[i][j] += pr[i][j-1], pc[i][j] += pc[i-1][j];
    Mint ans = 2;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            ans += (i+1<=n ? Mint(1) - pr[i+1][j-1] : Mint(1)) * (j+1<=m ? Mint(1) - pc[i-1][j+1] : Mint(1));
    printf("%d\n", ans);
    return 0;
}