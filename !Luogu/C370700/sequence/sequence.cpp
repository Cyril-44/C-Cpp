#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll INF = (1LL << 60);
constexpr int N = 505;
int a[N]; ll b[N];
int main() {
    cin.tie(nullptr) -> ios::sync_with_stdio(false);
    int Cid, T;
    cin >> Cid >> T;
    while (T--) {
        int n; cin >> n;
        for (int i = 1; i <= n; i++) cin >> a[i];
        for (int i = 1; i <= n; i++) cin >> b[i];
        int s = n + 1;
        vector<ll> f(s, -INF), nf(s);
        vector<ll> dp(s * s, -INF), ndp(s * s);
        f[0] = 0;
        for (int i = 1; i <= n; i++) {
            int x = a[i];
            ll w = b[i];
            nf = f;
            ndp = dp;
            for (int mx = 0; mx <= n; mx++) {
                if (f[mx] == -INF) continue;
                if (x < mx) {
                    nf[mx] = max(nf[mx], f[mx] + w);
                } else {
                    int p = x * s + x;
                    ndp[p] = max(ndp[p], f[mx] + w);
                }
            }
            for (int mn = 1; mn <= n; mn++) {
                for (int mx = mn; mx <= n; mx++) {
                    ll v = dp[mn * s + mx];
                    if (v == -INF) continue;
                    if (x < mn) {
                        nf[mx] = max(nf[mx], v + w);
                    } else {
                        int p = mn * s + max(mx, x);
                        ndp[p] = max(ndp[p], v + w);
                    }
                }
            }
            f.swap(nf);
            dp.swap(ndp);
        }
        ll ans = 0;
        for (int mx = 0; mx <= n; mx++)
            ans = max(ans, f[mx]);
        cout << ans << '\n';
    }
    return 0;
}
