#include <cstdio>
#include <vector>
#include <set>
constexpr int N = 1005;
std::vector<int> g[N];

int main() {
    int n, m, k, s, T, tc;
    scanf("%d", &tc);
    while (tc--) {
        scanf("%d%d%d%d%d", &n, &m, &k, &s, &T);
        for (int u, v; m--; ) {
            scanf("%d%d", &u, &v);
            g[u].push_back(v);
            g[v].push_back(u);
        }
        std::set<int> all;
        for (int i = 1, x; i <= T; i++) {
            scanf("%d", &x);
            for (int v : g[x]) all.insert(v);
        }
        printf("%zu\n", all.size());
    }
    return 0;
}