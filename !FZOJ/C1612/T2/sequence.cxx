#include <bits/stdc++.h>
#define ll long long
using namespace std;
const int mod = 998244353;
int n, m, g[200005][20], fl[20], f[200005];
int main() {
    cin >> n >> m;
    for (int i = 1; i <= m; i++) {
        g[i][1] = 1;
        for (int j = i * 2; j <= m; j += i) {
            for (int k = 2; k <= 18; k++) { g[j][k] = (g[j][k] - g[i][k - 1] + mod) % mod; }
        }
    }
    for (int j = 1; j <= 18; j++) {
        for (int i = 1; i <= m; i++) { fl[j] = (fl[j] + g[i][j]) % mod; }
    }
    f[0] = 1;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= i && j <= 18; j++) { f[i] = (f[i] + (ll)f[i - j] * fl[j]) % mod; }
    }
    cout << f[n] << endl;
    return 0;
}