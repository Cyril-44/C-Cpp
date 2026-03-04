#include <cstdio>
#include <cstring>
#include <vector>
constexpr int N = 500005;
std::vector<int> g[N];
int ldep[N], lson[N];
std::vector<int> cnt[N]; // 以 u 为根的子树，每一层的点数
void init(int u, int fa) {
    for (int v : g[u]) {
        if (v == fa) continue;
        if (ldep[v] >= ldep[u]) {
            ldep[u] = ldep[v] + 1;
            lson[u] = v;
        }
    }
    cnt[u].resize(ldep[u] + 1);
    cnt[u][0] = g[u].size() - (bool)(fa);
    for (int v : g[u]) {
        if (v == fa) continue;
        for (int i = 0; i ^ ldep[v]; i++)
            cnt[u][i+1] += cnt[v][i];
    }
}
int main() {
    freopen("otto.in", "r", stdin);
    freopen("otto.out", "w", stdout);
    int n, m, k;
    scanf("%d%d%d", &n, &m, &k);
    for (int i = 1, u, v; i < n; i++) {
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    return 0;
}