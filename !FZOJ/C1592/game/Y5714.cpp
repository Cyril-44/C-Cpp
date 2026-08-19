#include <bits/stdc++.h>
#define ll long long
using namespace std;
const int N = 1000000, mod = 998244353;
int t, n, m, f[N + 5], g[N + 5], ans;
ll inv[N + 5];
void add(int &x, int k) {
    x = (x + k >= mod ? x + k - mod : x + k);
}
int main() {
    inv[0] = inv[1] = 1;
    for (int i = 2; i <= N; i++) { inv[i] = (mod - mod / i) * inv[mod % i] % mod; }
    cin >> m >> t;
    f[m + 1] = m + 1;
    for (int i = m + 2; i <= N; i++) { f[i] = (f[i - 1] + f[i - m - 1] + 1) % mod; }
    for (int i = m + 1; i <= N; i++) {
        add(g[i], f[i]);
        for (int j = i * 2; j <= N; j += i) { add(g[j], mod - g[i]); }
    }
    while (t--) {
        scanf("%d", &n);
        ans = 0;
        for (int i = 1; i * i <= n; i++) {
            if (n % i == 0) {
                add(ans, g[i] * inv[i] % mod);
                if (i * i < n) add(ans, g[n / i] * inv[n / i] % mod);
            }
        }
        printf("%d %d\n", f[n], ans);
    }
    return 0;
}