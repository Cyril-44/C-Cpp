#include <bits/stdc++.h>
constexpr int N = 400005;
std::tuple<int,int,int,int> e[N];
int ans[N], id[N], n, m;
int main() {
    scanf("%d%d", &n, &m);
    for (int i = 1, u, v; i < n; i++) {
        scanf("%d%d", &u, &v);
        e[i] = {u, v, 0, 0};
    }
    std::fill(ans+1, ans+1+n, 1);
    for (int i = 1; i <= m; i++)
        scanf("%d", &id[i]);
    for (int i = m; i >= 1; i--) {
        auto &[u, v, wu, wv] = e[id[i]];
        int nowu = ans[v] - wv, nowv = ans[u] - wu;
        ans[u] += nowu - wu, ans[v] += nowv - wv;
        wu = nowu, wv = nowv;
    }
    for (int i = 1; i <= n; i++)
        printf("%d ", ans[i]);
    putchar('\n');
}