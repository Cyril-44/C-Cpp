#include <bits/stdc++.h>
using namespace std;
int t, n, m, f[15][2], g[15][2], ok;
char s[15], ans[15];
void work() {
    memset(f, 0x3f, sizeof(f));
    memset(g, -0x3f, sizeof(g));
    if (s[n] == '?')
        f[n][0] = f[n][1] = g[n][0] = g[n][1] = 0;
    else
        f[n][s[n] - '0'] = g[n][s[n] - '0'] = 0;
    for (int i = n - 1; i >= 1; i--) {
        if (s[i] == '?' || s[i] == '0') {
            f[i][0] = min(f[i + 1][0], f[i + 1][1] + 1);
            g[i][0] = max(g[i + 1][0], g[i + 1][1] + 1);
        }
        if (s[i] == '?' || s[i] == '1') {
            f[i][1] = min(f[i + 1][1], f[i + 1][0] + 1);
            g[i][1] = max(g[i + 1][1], g[i + 1][0] + 1);
        }
    }
    if (min(f[1][0], f[1][1]) > m || max(g[1][0], g[1][1]) < m) {
        ok = 0;
        return;
    }
    int cnt = 0;
    for (int i = 1; i <= n; i++) {
        if (s[i] == '0' || s[i] == '1') {
            ans[i] = s[i];
            if (i > 1) { cnt += ans[i] != ans[i - 1]; }
        } else {
            ans[i] = '0';
            if (i > 1) cnt += ans[i] != ans[i - 1];
            if (cnt + f[i][0] > m || cnt + g[i][0] < m) {
                if (i > 1) cnt -= ans[i] != ans[i - 1];
                ans[i] = '1';
                if (i > 1) cnt += ans[i] != ans[i - 1];
            }
        }
    }
    ans[n + 1] = 0;
    printf("%s\n", ans + 1);
}
int main() {
    freopen("memory.in", "r", stdin);
    freopen("memory.out", "w", stdout);
    cin >> t;
    while (t--) {
        scanf("%d%d%s", &n, &m, s + 1);
        if (m >= n) {
            puts("-1");
            continue;
        }
        int st = 0, ed = 0;
        for (int i = 1; i <= n; i++) {
            if (s[i] != '?' && !st)
                st = i;
            else if (s[i] != '?')
                ed = i;
        }
        int lst = st, cnt = 0;
        for (int i = st + 1; i <= ed; i++) {
            if (s[i] != '?') {
                cnt += s[i] != s[lst];
                lst = i;
            }
        }
        if (st == 1 && ed == n && (cnt - m) % 2) {
            puts("-1");
            continue;
        }
        if (!ed) {
            for (int i = 1; i <= n - m; i++) { putchar('0'); }
            for (int i = n - m + 1, j = 1; i <= n; i++, j ^= 1) { printf("%d", j); }
            putchar('\n');
            continue;
        }
        if (s[1] == '?') {
            if (s[n] == '?') {
                s[1] = '0';
                s[n] = '0' + ((m ^ cnt ^ abs(s[st] - s[1]) ^ (s[ed] - '0')) & 1);
                ok = 1;
                work();
                if (!ok) {
                    s[1] = '1';
                    s[n] = '0' + ((m ^ cnt ^ abs(s[st] - s[1]) ^ (s[ed] - '0')) & 1);
                    ok = 1;
                    work();
                    if (!ok) {
                        puts("-1");
                        continue;
                    }
                }
            } else {
                s[1] = '0' + ((m ^ cnt ^ (s[st] - '0')) & 1);
                ok = 1;
                work();
                if (!ok) {
                    puts("-1");
                    continue;
                }
            }
        } else {
            if (s[n] == '?') {
                s[n] = '0' + ((m ^ cnt ^ abs(s[st] - s[1]) ^ (s[ed] - '0')) & 1);
                ok = 1;
                work();
                if (!ok) {
                    puts("-1");
                    continue;
                }
            } else {
                ok = 1;
                work();
                if (!ok) {
                    puts("-1");
                    continue;
                }
            }
        }
    }
    return 0;
}