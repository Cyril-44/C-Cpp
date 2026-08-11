#include <bits/stdc++.h>
#define ll long long
using namespace std;
const int mod = 1e9 + 7;
int tid, t, n;
ll f[2][305][305];
struct node {
    int l, r;
} a[305];
void add(ll &x, int k) {
    x = (x + k >= mod ? x + k - mod : x + k);
}
int main() {
    cin >> tid >> t;
    while (t--) {
        cin >> n;
        for (int i = 1; i <= n; i++) { scanf("%d%d", &a[i].l, &a[i].r); }
        memset(f, 0, sizeof(f));
        for (int i = a[1].l; i <= a[1].r; i++) f[1][1][i] = (i == 1) + 1;
        for (int i = 2; i <= n; i++) {
            memset(f[i & 1], 0, sizeof(f[i & 1]));
            for (int j = 1; j < i; j++) {
                for (int k = 0; k <= min(i * 2 - 2, n); k++) {
                    for (int w = a[i].l; w <= a[i].r; w++) {
                        if (w == 0) {
                            add(f[i & 1][j + 1][k], f[i - 1 & 1][j][k]);
                            if (k) add(f[i & 1][j][k - 1], f[i - 1 & 1][j][k] * k % mod);
                        } else if (w == 1) {
                            add(f[i & 1][j + 1][k + 1], f[i - 1 & 1][j][k] * 2 % mod);
                            if (k) add(f[i & 1][j][k], f[i - 1 & 1][j][k] * 2 * k % mod);
                        } else {
                            add(f[i & 1][j + 1][k + 2], f[i - 1 & 1][j][k]);
                            add(f[i & 1][j][k + 1], f[i - 1 & 1][j][k] * k % mod);
                            add(f[i & 1][j][k + 1], f[i - 1 & 1][j][k] * j * 2 % mod);
                            add(f[i & 1][j - 1][k], f[i - 1 & 1][j][k] * (j - 1) % mod * k * 2 % mod);
                        }
                    }
                }
            }
        }
        cout << f[n & 1][1][0] << endl;
    }
    return 0;
}