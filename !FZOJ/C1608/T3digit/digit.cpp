#include <bits/stdc++.h>
constexpr int N = 100005, M = 1000005, MOD = (int)1e9 + 7;
struct Mint {
    Mint& operator+=(Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint x, Mint y) { return x += y; }
    friend Mint operator-(Mint x, Mint y) { return x -= y; }
    friend Mint operator*(Mint x, Mint y) { return x *= y; }
    Mint(int v=0) : val(v) {}
private: int val;
};
int pw10[N], ssum[N], d;
Mint fsum[M], f[N][2];
char s[N];
int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        scanf(" %s%d", s+1, &d);
        int n = strlen(s+1);
        pw10[0] = 1 % d;
        for (int i = 1; i <= n; i++)
            pw10[i] = pw10[i-1] * 10 % d;
        ssum[n+1] = 0;
        for (int i = n; i >= 1; i--)
            ssum[i] = (ssum[i+1] + (s[i] - '0') * pw10[n-i]) % d;
        memset(fsum, 0, sizeof(Mint) * (n+1));
        fsum[ssum[1]] = 1;
        Mint presum = 1;
        for (int i = 1; i <= n; i++) {
            f[i][0] = presum;
            f[i][1] = fsum[ssum[i+1]];
            fsum[ssum[i+1]] += f[i][0] + f[i][1];
            presum += f[i][1];
        }
        printf("%d\n", f[n][0] + f[n][1]);
    }
    return 0;
}