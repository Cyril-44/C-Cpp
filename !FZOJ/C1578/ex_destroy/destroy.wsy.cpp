#include <bits/stdc++.h>

using namespace std;

#define ll long long

const int N = 200005;
const int mod = 1000000007;

int n, max_dep, max_id;
vector<int> e[N];
int f[N], vis[N], a[N];

void dfs(int u, int fa, int dep = 1) {
    if (vis[u]) return;
    if (dep > max_dep) {
        max_dep = dep, max_id = u;
    }
    f[u] = fa;
    max_dep = max(max_dep, dep);
    for (int v : e[u]) if (v != fa && !vis[v]) {
        dfs(v, u, dep + 1);
    }
}

ll dp[105][105][105], c[105][105];
void work(int n) {
    // for (int i = 1; i <= n; i++) {
    //     printf("%d ", a[i]);
    // }
    // printf("\n");
    for (int i = 0; i <= n; i++) {
        c[i][0] = 1;
        for (int j = 1; j <= i; j++) {
            c[i][j] = (c[i - 1][j - 1] + c[i - 1][j]) % mod;
        }
    }
    for (int p = 0; p <= n; p++) {
        for (int i = 1; i <= n; i++) {
            dp[i][i][p] = (a[i] <= p);
        }
    }
    for (int i = 1; i <= n; i++) printf("%d\n", a[i]);
    for (int p = n; p >= 0; p--) {
        for (int i = n; i >= 1; i--) {
            if (a[i] > p) continue;
            for (int j = i + 1; j <= n; j++) {
                dp[i][j][p] = dp[i][j - 1][p + 1];
                for (int k = i + 1; k <= j; k++) {
                    if (a[k] <= p) {
                        if (k == j) (dp[i][j][p] += dp[i][k - 1][p]) %= mod;
                        else (dp[i][j][p] += dp[i][k - 1][p] * dp[k][j - 1][p + 1] % mod * c[j - i][k - i]) %= mod;
                    }
                }
                // printf("%d %d %d: %d\n", i, j, p, dp[i][j][p]);
            }
        }
    }
    printf("%d %lld\n", n, dp[1][n][0]);
}

int main() {
    scanf("%d", &n);
    for (int i = 1; i < n; i++) {
        int a, b;
        scanf("%d%d", &a, &b);
        e[a].push_back(b);
        e[b].push_back(a);
    }
    dfs(1, 0);

    int cnt = max_dep;

    vector<int> vec(cnt + 1);

    for (int i = max_id, now = max_dep; i; i = f[i], now--) {
        // printf("!!: %d\n", i);
        vec[now] = i;
    }
    for (int i = 1; i < cnt; i++) {
        vis[vec[i + 1]] = 1;
        max_dep = 0;
        dfs(vec[i], f[vec[i]], i);
        a[i + 1] = max(a[i], max_dep);
        vis[vec[i + 1]] = 0;
    }
    for (int i = 1; i <= cnt; i++) a[i] = max(0, a[i] - i + 1);
    work(cnt);
    return 0;
}