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
inline std::pair<int,int> calcDiv(int& x, int d) {
    int cnt = 0, base = 1;
    while (x && x % d == 0) x /= d, base *= d, ++cnt;
    return {cnt, base};
}
int pw10m2[N], pw10m5[N], pw10[N], ssum[N];
Mint fsum1[M], fsum[M], f[N][2];
char s[N];
int main() {
    int T, md;
    scanf("%d", &T);
    while (T--) {
        scanf(" %s%d", s+1, &md);
        int n = strlen(s+1);
        auto [mx2, md2] = calcDiv(md, 2);
        auto [mx5, md5] = calcDiv(md, 5);
        int mx = std::max(mx2, mx5);
        pw10m2[0] = 1 % md2;
        for (int i = 1; i <= 20; i++)
            pw10m2[i] = pw10m2[i-1] * 10 % md2;
        pw10m5[0] = 1 % md5;
        for (int i = 1; i <= 20; i++)
            pw10m5[i] = pw10m5[i-1] * 10 % md5;
        pw10[0] = 1 % md;
        for (int i = 1; i <= n; i++)
            pw10[i] = pw10[i-1] * 10 % md;
        ssum[n+1] = 0;
        for (int i = n; i >= 1; i--)
            ssum[i] = (ssum[i+1] + (s[i] - '0') * pw10[n-i]) % md;
        memset(f, 0, sizeof(Mint) * 2 * (n+1));
        memset(fsum, 0, sizeof(Mint) * md);
        memset(fsum1, 0, sizeof(Mint) * md);
        f[0][1] = 1;
        Mint f1sum = 0;
        if (!mx) fsum[ssum[1]] = fsum1[ssum[1]] = f1sum = 1;
        for (int i = 1; i <= n; i++) {
            int v = 0, v2 = 0, v5 = 0;
            for (int j = 0; j < mx && j < i; j++) {
                v = (v + pw10[j] * (s[i-j] - '0')) % md;
                v2 = (v2 + pw10m2[j] * (s[i-j] - '0')) % md2;
                v5 = (v5 + pw10m5[j] * (s[i-j] - '0')) % md5;
                if (!v && !v2 && !v5) f[i][1] += f[i-j-1][0] + f[i-j-1][1];
                else f[i][0] += f[i-j-1][1];
            }
            if (!v2 && !v5) { // j >= mx 之后的 pw10m2 和 pw10m5 都是 0，没有算的必要。
                f[i][0] += f1sum - fsum1[ssum[i+1]];
                f[i][1] += fsum[ssum[i+1]];
            }
            else f[i][0] += f1sum;
            if (i - mx >= 0) {
                fsum[ssum[i-mx+1]] += f[i-mx][0] + f[i-mx][1];
                fsum1[ssum[i-mx+1]] += f[i-mx][1];
                f1sum += f[i-mx][1];
            }
        }
        printf("%d\n", f[n][0] + f[n][1]);
    }
    return 0;
}