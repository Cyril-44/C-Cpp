#include <cstdio>
#include <cstring>
#include <algorithm>
constexpr int N = 1005, MOD = (int)1e9 + 7;
char s[N];
int mod10[N];
struct Mint {
    Mint& operator+=(Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint x, Mint y) { return x += y; }
    friend Mint operator-(Mint x, Mint y) { return x -= y; }
    friend Mint operator*(Mint x, Mint y) { return x *= y; }
    Mint(int v=0) : val(v) {}
private: int val;
} f[N][2];
int main() {
    int T, d;
    scanf("%d", &T);
    while (T--) {
        scanf(" %s%d", s+1, &d);
        int n = strlen(s+1);
        mod10[0] = 1 % d;
        for (int i = 1; i <= n; i++)
            mod10[i] = mod10[i-1] * 10 % d;
        memset(f, 0, sizeof f);
        f[0][1] = 1;
        for (int i = 1; i <= n; i++) {
            int mod = (s[i] - '0') % d;
            for (int j = i-1; j >= 0; j--) {
                if (mod == 0) f[i][1] += f[j][0] + f[j][1];
                else f[i][0] += f[j][1];
                mod = (mod + mod10[i-j] * (s[j] - '0')) % d;
            }
        }
        printf("%d\n", f[n][0] + f[n][1]);
    }
    return 0;
}