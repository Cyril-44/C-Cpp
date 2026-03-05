#include <bits/stdc++.h>

using namespace std;

#define ll long long

const int N = 300005;

ll ans;
int n, mod;
ll dep[N], f[N], cnt[N], p[N], ic[N];

ll ksm(ll a, int b) {
    ll res = 1;
    while (b) {
        if (b & 1) res = res * a % mod;
        a = a * a % mod;
        b >>= 1;
    }
    return res;
}

void add(ll &a, ll b) {
    a += b;
    if (a >= mod) a -= mod;
}

int main() {
    freopen("math.in", "r", stdin);
    freopen("math.out", "w", stdout);
    scanf("%d%d", &n, &mod);
    dep[1] = 1, cnt[1] = 1;
    for (int i = 1; i <= n; i++) {        
        ic[i] = ksm(cnt[i], mod - 2);
        dep[i] = dep[i] * ic[i] % mod;
        dep[i]++;
        // printf("%lld ", dep[i]);
        for (int j = i + i; j <= n; j += i) {
            (dep[j] += dep[i]) %= mod;
            cnt[j]++;
        }
    }
    for (int i = n; i >= 1; i--) {
        p[i] = cnt[i];
        ll s1 = 0, s2 = 0;
        for (int j = i; j <= n; j += i) {
            p[j] = p[j] * ic[j] % mod;
            add(s1, p[j]);
            for (int k = j + j; k <= n; k += j) {
                add(p[k], p[j]);
            }
            (s2 += p[j] * p[j] % mod * f[j]) %= mod;
        }
        f[i] = (s1 * s1 - s2 + mod) % mod;
        for (int j = i; j <= n; j += i) p[j] = 0;
    }
    for (int i = 1; i <= n; i++) {
        ans = (ans + dep[i] * n - f[i] * dep[i] % mod + mod) % mod;
    }
    printf("%lld\n", ans * 2 % mod);
    return 0;
}