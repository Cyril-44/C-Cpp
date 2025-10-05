#include <bits/stdc++.h>
using namespace std;
constexpr int N = 250005;
int a[N], f[N];
int g[N];
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T, n, q;
    cin >> T;
    while (T--) {
        cin >> n >> q;
        for (int i = 1; i <= n; i++) cin >> a[i];
        for (int i = 1; i <= n; i++) {
            f[i] = 32 - __builtin_clz(a[i] - 1);
            g[i] = ((a[i] - 1) & (a[i] - 2)) == 0;
        }
        for (int i = 1; i <= n; i++) f[i] += f[i-1], g[i] += g[i-1];
        for (int l, r; q--; ) {
            cin >> l >> r;
            cout << f[r] - f[l-1] - (g[r] - g[l-1] > 0) << '\n';
        }
    }
    return 0;
}