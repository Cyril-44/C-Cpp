#include <cstdio>
#include <algorithm>
#include <queue>
#include <cstring>
#include <cassert>
#include <set>
#include <stack>

using namespace std;

#define ll long long

const int N = 1000005;
const int mod = 1e9 + 7;

int n, m, k;

struct Node {
    int l, r;
}p[N], q[N];
int f[2005];
ll fac[N], ifac[N], inv[N], pw[N], g[N], iv[N];

ll F(int l, int r) {
    // printf("FD: %d %d\n", l, r);
    return fac[r] * ifac[l] % mod;
}

int main() {
    scanf("%d%d%d", &n, &m, &k);
    fac[0] = fac[1] = inv[0] = inv[1] = ifac[0] = ifac[1] = 1;
    pw[0] = 1, pw[1] = m;
    for (int i = 2; i < N; i++) {
        fac[i] = fac[i - 1] * i % mod;
        inv[i] = (mod - mod / i) * inv[mod % i] % mod;
        ifac[i] = ifac[i - 1] * inv[i] % mod;
        pw[i] = pw[i - 1] * m % mod;
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

    f[0] = 1;
    ll ans = pw[n];
    for (int i = 1; i <= k; i++) {
        for (int x = 0; x < i; x++) {
            if (p[x].r < p[i].l) {
                f[i] = (f[i] - f[x] * pw[p[i].l - p[x].r - 1] % mod * F(m - (p[i].r - p[i].l + 1), m)) % mod;
            } else {
                f[i] = (f[i] - f[x] * F(m - (p[i].r - p[i].l + 1), m - (p[x].r - p[i].l + 1))) % mod;
            }
        }
        (ans += f[i] * pw[n - p[i].r]) %= mod;
    }
    (ans += mod) %= mod;
    printf("%lld\n", ans);
    return 0;
}