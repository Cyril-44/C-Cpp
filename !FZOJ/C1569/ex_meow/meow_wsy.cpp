#include <bits/stdc++.h>

using namespace std;

#define ll long long

const int N = 505;
const int mod = 1000000007;

int n, m, ans;
ll f[2][N][N];
int a[N];
bool tag[N];

signed main() {
    // freopen("1.in", "r", stdin);
    // freopen("meow.out", "w", stdout);
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &a[i]), tag[a[i]] = 1;
    }
    for (int i = 1; i <= n; i++) {
        if (a[i] != 0 && a[a[i]] != 0 && a[a[i]] <= a[i]) {
            printf("0\n");
            return 0;
        }
    }
    for (int i = 0; i <= n; i++) f[0][0][i] = 1;
    for (int i = 0; i < n; i++) {
        int ni = i & 1, ti = ni ^ 1;
        for (int j = 0; j <= n; j++) {
            for (int k = 0; k <= n; k++) {
                f[ti][j][k] = 0;
            }
        }
        if (a[i + 1]) {
            if (a[i + 1] < i) {
                for (int j = 0; j <= n; j++) {
                    for (int k = j; k <= n; k++) {
                        f[ti][j][k] = f[ni][j][k];
                    }
                }
            } else {
                for (int j = 0; j <= n; j++) {
                    for (int k = j; k <= n; k++) {
                        f[ti][j + 1][k] = f[ni][j][k];
                    }
                }
            }
            continue;
        }
        for (int j = 0; j <= n; j++) {
            for (int k = j; k <= n; k++) if (f[ni][j][k]) {
                (f[ti][j + 1][k] += f[ni][j][k] * (k - j)) %= mod;
                if (!tag[i + 1]) {
                    (f[ti][j][k] += f[ni][j][k] * j) %= mod;
                }
                // printf("!!: %d %d %d: %d\n", i, j, k, f[i & 1][j][k]);
            }
        }
    }
    int ans = 0;
    for (int i = 0; i <= n; i++) {
        (ans += f[n & 1][i][i]) %= mod;
        // printf("!!: %lld\n", f[n & 1][i][i]);
    }
    printf("%d\n", ans);
    return 0;
}