#include <bits/stdc++.h>
constexpr int N = 300005;
struct Node {
    int at, hp;
} a[N], b[N];
int64_t fix[N];
int main() {
    int n, m, V;
    scanf("%d%d%d", &n, &m, &V);
    for (int i = 1; i <= n; i++)
        scanf("%d%d", &a[i].at, &a[i].hp);
    for (int i = 1; i <= m; i++)
        scanf("%d%d", &b[i].at, &b[i].hp);
    int k = 0, val = 0;
    int64_t curatt = 0;
    for (int q = 1; q <= m; q++) {
        while (val < b[q].at) curatt += fix[++val];
        while (k < n && curatt < b[q].hp) {
            ++k; curatt += ((a[k].hp-1) / b[q].at + 1ll) * a[k].at;
            for (int l = b[q].at, r; l <= a[k].hp-1; l = r+1) {
                r = (a[k].hp-1) / ((a[k].hp-1)/l);
                fix[r+1] += 1ll * a[k].at * ((a[k].hp-1)/(r+1) - (a[k].hp-1)/l);
            }
        }
        if (k == n && curatt < b[q].hp) puts("-1");
        else printf("%d\n", k);
    }
    return 0;
}