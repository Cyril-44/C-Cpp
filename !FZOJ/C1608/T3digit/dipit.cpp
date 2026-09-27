#include <bits/stdc++.h>
#define ll long long
using namespace std;
const int mod = 1e9 + 7;
int T, B, n, d, a[100005], inv10, d_;
__int128 dt[100005][20];
ll ipw[100005], f[100005], g[100005], hsh[100005], tf[1000005], tg[1000005];
char s[100005];
void exgcd(ll a, ll b, ll &x, ll &y) {
    if (!b) {
        x = 1, y = 0;
        return;
    } else {
        exgcd(b, a % b, y, x);
        y -= a / b * x;
    }
}
void init() {
    if (d == 1) {
        for (int i = 0; i <= n; i++) ipw[i] = 1;
        return;
    }
    ipw[0] = 1;
    ll x, y;
    exgcd(10, d, x, y);
    inv10 = (x % d + d) % d;
    for (int i = 1; i <= n; i++) { ipw[i] = ipw[i - 1] * inv10 % d; }
}
int main() {
    cin >> T;
    while (T--) {
        scanf("%s%d", s + 1, &d);
        d_ = d;
        n = strlen(s + 1);
        ll d10 = 1;
        int cnt2 = 0, cnt5 = 0;
        while (d % 2 == 0) {
            d10 *= 2;
            d /= 2;
            cnt2++;
        }
        while (d % 5 == 0) {
            d10 *= 5;
            d /= 5;
            cnt5++;
        }
        B = max({cnt2, cnt5, 7});
        init();
        for (int i = 1; i <= n; i++) a[i] = s[i] - '0';
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= B && i + j - 1 <= n; j++) { dt[i][j] = dt[i][j - 1] * 10 + a[i + j - 1]; }
        }
        memset(f, 0, (n + 1) * sizeof(ll));
        memset(g, 0, (n + 1) * sizeof(ll));
        memset(tf, 0, d * sizeof(ll));
        memset(tg, 0, d * sizeof(ll));
        ll hshh = 0;
        for (int i = 1; i <= n; i++) {
            hshh = (hshh * 10 + a[i]) % d;
            hsh[i] = hshh * ipw[i - 1] % d;
        }
        ll cntf = 1;
        f[0] = g[0] = 1;
        for (int i = 1; i <= n; i++) {
            for (int j = i - 1, k = 1; k <= B && j >= 0; j--, k++) {
                if (dt[j + 1][k] % d_ == 0) {
                    f[i] = (f[i] + g[j]) % mod;
                    g[i] = (g[i] + g[j] - f[j] + mod) % mod;
                }
            }
            if (i >= B && dt[i - (B - 1)][B] % d10 == 0) {
                f[i] = (f[i] + tg[hsh[i]]) % mod;
                g[i] = (g[i] + tg[hsh[i]] - tf[hsh[i]] + mod) % mod;
            }
            if (i >= B) {
                tf[hsh[i - B]] = (tf[hsh[i - B]] + f[i - B]) % mod;
                tg[hsh[i - B]] = (tg[hsh[i - B]] + g[i - B]) % mod;
            }
            g[i] = (g[i] + cntf) % mod;
            cntf = (cntf + f[i]) % mod;
        }
        printf("%lld\n", g[n]);
    }
    return 0;
}