#include <stdio.h>
#include <string.h>
#include <set>
#include <algorithm>
const int N = 100005;
std::set<int> g[N];
int ans[N << 2][3];
int main() {
    int t, n, m, k, u, v;
    scanf("%d", &t);
    while (t--) {
        scanf("%d %d", &n, &m);
        if (!m) {
            puts("0");
            continue;
        }
        for (int i = 1; i <= n; i++)
            g[i].clear();
        while (m--) {
            scanf("%d%d", &u, &v);
            g[u].insert(v);
            g[v].insert(u);
        }
        k = 0;
        int cnt = 0;
        for (int i = 1; i <= n; i++)
            cnt += g[i].size() > 1;
            for (int i = 1; i <= n; i++) {
                if (g[i].size() > 1) {
                    auto it = g[i].begin(), sec = ++g[i].begin();
                    int tmp1, tmp2;
                    for (; g[i].size() > 1; ) {
                        tmp1 = *it, tmp2 = *sec;
                        if (g[tmp1].count(tmp2)) {
                            g[tmp1].erase(tmp2);
                            g[tmp2].erase(tmp1);
                            if (g[tmp1].size() == 1) --cnt;
                            if (g[tmp2].size() == 1) --cnt;
                        }
                        else {
                            g[tmp1].insert(tmp2);
                            g[tmp2].insert(tmp1);
                            if (g[tmp1].size() == 2) ++cnt;
                            if (g[tmp2].size() == 2) ++cnt;
                        }
                        it = ++sec, ++sec;
                        ++k;
                        ans[k][0] = i, ans[k][1] = tmp1, ans[k][2] = tmp2;
                        g[tmp1].erase(i), g[tmp2].erase(i);
                        if (g[tmp1].size() == 1) --cnt;
                        if (g[tmp2].size() == 1) --cnt;
                        g[i].erase(tmp1), g[i].erase(tmp2);
                    }
                    --cnt;
                }
            }
        int a = 0, b = 0;
        for (int i = 1; i <= n; i++)
            if (g[i].size() && *g[i].begin() > i) {
                a = i;
                b = *g[i].begin();
                break;
            }
        if (a) {
            for (int i = 1; i <= n; i++) {
                if (!g[i].size()) {
                    ++k;
                    ans[k][0] = i, ans[k][1] = a, ans[k][2] = b;
                    b = i;
                }
                else if ((i ^ a) && (*g[i].begin() ^ a) && (i < *g[i].begin())) {
                    ++k;
                    ans[k][0] = b, ans[k][1] = i, ans[k][2] = *g[i].begin();
                }
            }
        }
        printf("%d\n", k);
        for (int i = 1; i <= k; i++)
            printf("%d %d %d\n", ans[i][0], ans[i][1], ans[i][2]);
    }
    return 0;
}