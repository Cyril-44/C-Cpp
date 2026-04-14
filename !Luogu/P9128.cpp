#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
constexpr int N = 200005;
constexpr int64_t INF = 2e18;
int dep[N], size[N], a[N];
int64_t f[N][2], suma[N];
std::vector<int> g[N];
/*
邻项交换排序好题。
注意到最终走的节点序列中，交换两个相邻元素，不影响其他元素的贡献。
直接邻项交换即可。
T = 1 需要记录 f[u][1] 表示最多将一个 dep 最深的点移动到最后。
*/
int main() {
    int n, T;
    scanf("%d%d", &n, &T);
    for (int i = 2, v; i <= n; i++) {
        scanf("%d%d", &v, &a[i]);
        g[v].push_back(i);
        dep[i] = dep[v] + 1;
    }
    const int mxdep = *std::max_element(dep+1, dep+1+n);
    for (int u = n; u >= 1; u--) {
        size[u] = 1;
        suma[u] = a[u];
        if (!g[u].empty()) {
            for (int v : g[u]) size[u] += size[v];
            for (int v : g[u]) suma[u] += suma[v];
            std::sort(g[u].begin(), g[u].end(), [](int x, int y) {
                // fx + fy + 2 sum[y] sz[x] < fy + fx + 2 sum[x] sz[y]
                return suma[y] * size[x] < suma[x] * size[y];
            });
            for (int i = 0, v, t = 1; i < (int)g[u].size(); i++) {
                v = g[u][i];
                f[u][0] += t * suma[v] + f[v][0];
                t += 2 * size[v];
            }
            f[u][1] = INF;
            int64_t sufSuma = 0;
            for (int i = (int)g[u].size() - 1, v, t = 2 * size[u] - 1; i >= 0; i--) {
                v = g[u][i]; t -= 2 * size[v];
                if (f[v][1] != INF)
                    f[u][1] = std::min(f[u][1],  // v 放到最后，那 v 后面的每一个 v 都提前 2*sz[v]
                        f[u][0] - sufSuma * 2 * size[v] - t * suma[v] - f[v][0]
                                + (size[u] * 2 - size[v] * 2 - 1) * suma[v] + f[v][1]
                    );
                // printf("f[%d][1] tries   %ld - 2*%ld*%d - %d*%ld - %ld + (%d*2-%d*2-1)*%ld + %ld\n", u, f[u][0], sufSuma, size[v], t, suma[v], f[v][0], size[u], size[v], suma[v], f[v][1]);
                sufSuma += suma[v];
            }
        }
        else {
            f[u][0] = 0;
            f[u][1] = dep[u] == mxdep ? 0 : INF;
        }
    }
    // for (int i = 1; i <= n; i++)
    //     printf("%ld %ld\n", f[i][0], f[i][1]);
    printf("%d ", 2*n - 2 - T*mxdep);
    printf("%ld\n", f[1][T]);
    return 0;
}