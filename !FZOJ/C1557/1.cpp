#include <bits/stdc++.h>
using namespace std;
using ll = long long;
constexpr int N = 1000005, MOD = 1e9 + 7;
int n, m, k;
struct Node {int l, r;}p[N], q[N];
int f[2005][2005];
ll fac[N], ifac[N], inv[N], power[N], g[N], iv[N];
ll F(int l, int r) { return fac[r] * ifac[l] % MOD;}
ll C(int a, int b) {
    if (a < 0 || b < 0 || a < b || k > 10) return 0;
    return fac[a] * ifac[b] % MOD * ifac[a - b] % MOD;
}
int main() {
    scanf("%d%d%d", &n, &m, &k);
    fac[0] = fac[1] = inv[0] = inv[1] = ifac[0] = ifac[1] = 1;
    power[0] = 1, power[1] = m;
    for (int i = 2; i <= n; i++) {
        fac[i] = fac[i - 1] * i % MOD;
        inv[i] = (MOD - MOD / i) * inv[MOD % i] % MOD;
        ifac[i] = ifac[i - 1] * inv[i] % MOD;
        power[i] = power[i - 1] * m % MOD;
    }
    for (int i = 1; i <= k; i++) {
        scanf("%d%d", &p[i].l, &p[i].r);
        if (p[i].l == p[i].r) {
            printf("0\n");
            return 0;
        }
    }
    sort(p + 1, p + 1 + k, [](Node a, Node b) {
        return a.l ^ b.l ? a.l < b.l : a.r > b.r;
    });
    int cnt = 0, tr = n + 1;
    for (int i = k; i >= 1; i--) {
        if (p[i].r - p[i].l + 1 > m) continue;
        if (p[i].r < tr) {
            tr = p[i].r;
            q[++cnt] = p[i];
        }
    }
    memcpy(p, q, sizeof(p)), k = cnt;
    reverse(p + 1, p + 1 + cnt);
    f[0][0] = 1;
    g[0] = power[n];
    for (int i = 1; i <= k; i++) {
        for (int j = 1; j <= i; j++) {
            for (int x = 0; x < i; x++) {
                if (p[x].r < p[i].l) {
                    f[i][j] = (f[i][j] + f[x][j - 1] * power[p[i].l - p[x].r - 1] % MOD * F(m - (p[i].r - p[i].l + 1), m)) % MOD;
                } else {
                    f[i][j] = (f[i][j] + f[x][j - 1] * F(m - (p[i].r - p[i].l + 1), m - (p[x].r - p[i].l + 1))) % MOD;
                }
            }
            (g[j] += f[i][j] * power[n - p[i].r]) %= MOD;
        }
    }
    for (int i = k; i >= 0; i--) {
        for (int j = k; j > i; j--) {
            g[i] = (g[i] - g[j] * C(j, i) % MOD + MOD) % MOD;
        }
    }
    printf("%lld\n", g[0]);
    return 0;
}