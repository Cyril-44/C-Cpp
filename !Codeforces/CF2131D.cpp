#include <cstdio>
#include <algorithm>
#include <cstring>
#include <vector>
constexpr int N = 200005;
std::vector<int> g[N];
int main() {
    int t, n;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        for (int i = 1; i <= n; i++) g[i].clear();
        for (int i = 1, u, v; i < n; i++) {
            scanf("%d%d", &u, &v);
            g[u].emplace_back(v);
            g[v].emplace_back(u);
        }
        int cnt = 0;
        for (int i = 1; i <= n; i++)
            if (g[i].size() == 1) ++cnt;
        int ans = cnt;
        for (int i = 1; i <= n; i++) {
            int now = cnt;
            if (g[i].size() == 1) --now;
            for (int j : g[i])
                if (g[j].size() == 1) --now;
            ans = std::min(ans, now);
        }
        printf("%d\n", ans);
    }
    return 0;
}