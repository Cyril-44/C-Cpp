#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
#include <queue>
#include <numeric>
constexpr int N = 2022, F = 515;
int a[N];
/*
                          __ 515 ---+---(a1)-------- a1
                         /__ 515    |___(515-a1) --- a2
 S --> a1 --> a2 --> a3 {___ 515
                         \__ 515
                          \_ 515
*/
std::vector<std::pair<int,int>> g[N + 5 * N];
struct Comp{ inline bool operator()(int x, int y) { return a[x] < a[y]; } };
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    a[0] = F*n - std::accumulate(a+1, a+1+n, 0);
    std::priority_queue<int, std::vector<int>, Comp> q1, q2;
    for (int i = 0; i <= n; i++)
        if (a[i] != 0)
            if (a[i] < F) q1.push(i);
            else q2.push(i);
    int m = 0;
    auto addedg = [&](int fr, int to, int w) {
        if (to == 0) to = -1;
        g[fr].emplace_back(to, w);
        ++m;
    };
    addedg(n+1, n+2, n*F);
    addedg(n+2, n+3, n*F);
    addedg(n+3, n+4, n*F);
    for (int i = 1; i <= n; i++)
        addedg(n+4, n+4+i, F);
    for (int i = 1; i <= n; i++) {
        if (q1.empty()) {
            int u = q2.top(); q2.pop();
            if (a[u] -= F) a[u] < F ? q1.push(u) : q2.push(u);
            addedg(n+4+i, u, F);
        } else if (q2.empty()) {
            int u = q1.top(); q1.pop();
            int v = q1.top(); q1.pop();
            if (a[v] -= F - a[u]) a[v] < F ? q1.push(v) : q2.push(v);
            addedg(n+4+i, u, a[u]);
            addedg(n+4+i, v, F - a[u]);
        } else {
            int u = q1.top(); q1.pop();
            int v = q2.top(); q2.pop();
            if (a[v] -= F - a[u]) a[v] < F ? q1.push(v) : q2.push(v);
            addedg(n+4+i, u, a[u]);
            addedg(n+4+i, v, F - a[u]);
        }
    }
    printf("%d\n", n+4);
    for (int u = n+1; u <= n+4+n; u++) {
        int type = g[u].size() == 1 ? 1 : g[u].size() == 2 ? 3 : 2;
        printf("%d %d ", u, type);
        if (type == 2) printf("%zu ", g[u].size());
        for (const auto& [v, w] : g[u]) printf("%d ", v);
        if (type == 2) printf("%d", g[u][0].second);
        else for (const auto &[v, w] : g[u]) printf("%d ", w);
        putchar('\n');
    }
    return 0;
}