#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
#include <limits>
using std::min;
using std::max;
struct DpNode {
    long long x, f;
    inline DpNode() : x(0), f(0) {}
    inline DpNode(const long long &a, const long long &b) : x(a), f(b) {}
};
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    std::vector<std::vector<int>> a(n+1, std::vector<int>(m+1));
    std::vector<int> p(n+m);
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            scanf("%d", &a[i][j]);
    for (int i = 1; i < n+m; i++)
        scanf("%d", &p[i]);
    std::vector<std::vector<DpNode>> f(n+1, std::vector<DpNode>(m+1));
    f[1][1].f = max(a[1][1] - p[1], 0);
    f[1][1].x = max(p[1] - a[1][1], 0);
    for (int i = 1; i <= n; i++)
        f[i][0].x = std::numeric_limits<long long>::max(), f[i][0].f = -(std::numeric_limits<long long>::max());
    for (int j = 1; j <= m; j++)
        f[0][j].x = std::numeric_limits<long long>::max(), f[0][j].f = -(std::numeric_limits<long long>::max());
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
        if (i!=1 || j!=1) {
            long long f1 = f[i-1][j].f + a[i][j] - p[i+j-1];
            long long f2 = f[i][j-1].f + a[i][j] - p[i+j-1];
            if (i == 1) {
                if (f2 > 0) f[i][j] = {f[i][j-1].x, f2};
                else f[i][j] = {f[i][j-1].x-f2, 0};
                continue;
            }
            if (j == 1) {
                if (f1 > 0) f[i][j] = {f[i-1][j].x, f1};
                else f[i][j] = {f[i-1][j].x-f1, 0};
                continue;
            }
            if (f1 < 0 && f2 < 0) {
                f[i][j].x = min(f[i-1][j].x - f1, f[i][j-1].x - f2);
                f[i][j].f = 0;
            }
            else {
                if (f[i-1][j].x < f[i][j-1].x) f[i][j] = DpNode(f[i-1][j].x, f1);
                else f[i][j] = DpNode(f[i][j-1].x, f2);
            }
        }
    printf("%lld\n", f[n][m].x);
    return 0;
}