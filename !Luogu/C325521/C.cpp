#include <cstdio>
#include <vector>
#include <algorithm>
#include <queue>
constexpr int N = 200005;
std::vector<int> g[N];
int a[N];
bool vis[N];
int main() {
    int n, m, q, A;
    scanf("%d%d%d%d", &n, &m, &q, &A);
    for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
    while (m--) {
        int u, v;
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    std::priority_queue<std::pair<int,int>> pq;
    while (q--) {
        int x;
        scanf("%d", &x);
        pq.emplace(a[x], x);
    }
    vis[A] = true;
    a[A] = 0x7fffffff;
    while (!pq.empty()) {
        auto [_w, u] = pq.top(); pq.pop();
        if (vis[u]) continue;
        vis[u] = true;
        for (int v : g[u])
            if (a[v] <= a[u])
                pq.emplace(a[v] = a[u], v);
    }
    int cnt = 0;
    for (int v : g[A])
        if (vis[v])
            ++cnt;
    printf("%d\n", cnt);
    return 0;
}