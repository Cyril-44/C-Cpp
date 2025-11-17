#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <vector>
#include <tuple>
constexpr int N = 300005;
std::vector<int> g[N];
std::vector<std::tuple<int,int,int>> queries[N];
int ans[N];
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int u, v, i = n; --i; ) {
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    for (int i = 1, u, l, r; i <= m; i++) {
        scanf("%d%d%d", &u, &l, &r);
        queries[u].emplace_back(i, l, r);
    }
    
    return 0;
}
