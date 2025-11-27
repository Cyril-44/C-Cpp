#include <bits/stdc++.h>
using namespace std;
const int mod = 1e9 + 7;
int n, m, totv[10005], b[10005], ans[10005][5005], f[2][10005][2][2];
int t1[10005], t2[10005], flag[10005], cntt[10005], nn, ff[2][10005];
int main() {
    freopen("magic.in", "r", stdin);
    freopen("magic.out", "w", stdout);
    cin >> n >> m;
    for (int i = 1; i <= n; i++) {
        scanf("%d", &totv[i]);
        t1[totv[i]] = i;
    }
    for (int i = 1; i <= n; i++) {
        scanf("%d", &b[i]);
        t2[b[i]] = i;
    }
    f[1][1][0][0] = f[1][1][1][1] = 1;
    ans[1][1] = 2;
    for (int i = 2; i <= n; i++) {
        memset(f[i & 1], 0, sizeof(f[i & 1]));
        for (int j = (i + 1) / 2, k = 1; j <= i; j++, k++) {
            f[i & 1][j][0][0] = (f[(i - 1) & 1][j - 1][0][0] + f[(i - 1) & 1][j - 1][0][1]) % mod;
            f[i & 1][j][0][1] = (f[(i - 1) & 1][j][0][0] + f[(i - 1) & 1][j - 1][0][1]) % mod;
            f[i & 1][j][1][0] = (f[(i - 1) & 1][j - 1][1][0] + f[(i - 1) & 1][j][1][1]) % mod;
            f[i & 1][j][1][1] = (f[(i - 1) & 1][j - 1][1][0] + f[(i - 1) & 1][j - 1][1][1]) % mod;
            ans[i][k] = ((f[i & 1][j][0][0] + f[i & 1][j][0][1]) % mod +
                         (f[i & 1][j][1][0] + f[i & 1][j][1][1]) % mod) %
                        mod;
        }
    }
    for (int i = 1; i <= n; i++) {
        if (flag[i])
            continue;
        int cnt = 0;
        cnt++;
        flag[i] = 1;
        for (int x = t1[b[i]]; x != i; x = t1[b[x]]) {
            cnt++;
            flag[x] = 1;
        }
        cntt[++nn] = cnt;
    }
    sort(cntt + 1, cntt + nn + 1);
    ff[0][0] = 1;
    int sumr = 0, suml = 0, anss = 0;
    for (int i = 1; i <= nn; i++) {
        sumr += cntt[i];
        suml += (cntt[i] + 1) / 2;
        memset(ff[i & 1], 0, sizeof(ff[i & 1]));
        for (int j = suml; j <= sumr; j++) {
            for (int k = suml - (cntt[i] + 1) / 2; k <= sumr - cntt[i]; k++) {
                ff[i & 1][j] = (ff[i & 1][j] + 1ll * ff[(i - 1) & 1][k] *
                                                   ans[cntt[i]][j - k - (cntt[i] + 1) / 2 + 1] % mod) %
                               mod;
            }
        }
    }
    for (int i = m; i <= n; i++) {
        anss = (anss + ff[nn & 1][i]) % mod;
    }
    cout << anss;
    return 0;
}
/*
f[i][j]=seg f[i-1][k]*ans[a[i]][j-k]

f[i]=seg f[i-1]*seg ans[a[i]][j]
*/