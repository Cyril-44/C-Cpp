#include <bits/stdc++.h>
#define ll long long
using namespace std;
const int mod = 998244353;
int n, m, f[250005];
int main() {
    freopen("sale.in", "r", stdin);
    freopen("sale.out", "w", stdout);
    cin >> n >> m;
    cout << n * (n - 1) / 2 + 1 << endl;
    f[0] = 1;
    for (int i = 1; i <= n - 2; i++) {
        for (int j = m; j >= i; j--) { f[j] = (f[j] + f[j - i]) % mod; }
    }
    cout << f[m] << endl;
    return 0;
}