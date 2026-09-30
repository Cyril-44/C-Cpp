#include <cstdio>
constexpr int N = 1000005, MOD = 998244353;
struct Mint {
    Mint& operator+=(Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint x, Mint y) { return x += y; }
    friend Mint operator-(Mint x, Mint y) { return x -= y; }
    friend Mint operator*(Mint x, Mint y) { return x *= y; }
    Mint(int v=0) : val(v) {}
private: int val;
} f[N];
int main() {
    f[1] = 1;
    for (int i = 2; i < N; i++) f[i] = f[i-1] + f[i/2];
    int q;
    scanf("%d", &q);
    for (int n; q--; ) {
        scanf("%d", &n);
        printf("%d\n", f[n]);
    }
    return 0;
}