#include <cstdio>
#include <cstring>
#include <algorithm>
using std::max;
const int N = 305;
typedef long long LL;
bool f[N][N];
struct Node {
    LL k;
    int v;
    inline bool operator< (const Node &x) const {
        return k < x.k;
    }
} a[N];
LL g[N][N][2], s[N];
// g[i][j][0/1] 表示 i-j 的树 是 0: (i-1)的右子树 1: (j+1)的左子树
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        scanf("%lld%d", &a[i].k, &a[i].v);
    std::sort(a+1, a+1 + n);
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            f[i][j] = i != j && std::__gcd(a[i].k, a[j].k) != 1;
    memset(g, -0x3f, sizeof g);
    for (int i = 1; i <= n; i++) {
        f[0][i] = f[i][n+1] = true;
        g[i][i-1][1] = g[i+1][i][0] = 0;
        s[i] = s[i-1] + a[i].v;
    }
    for (int k = 0; k < n; k++)
        for (int l = 1; l+k <= n; l++) {
            int r = l + k, sum = s[r] - s[l-1];
            for (int i = l; i <= r; i++) {
                if (f[l-1][i]) g[l][r][0] = max(g[l][r][0], g[l][i-1][1] + g[i+1][r][0] + sum);
                if (f[i][r+1]) g[l][r][1] = max(g[l][r][1], g[l][i-1][1] + g[i+1][r][0] + sum);
            }
        }
    if (g[1][n][0] < 0) puts("-1");
    else printf("%lld\n", g[1][n][0]);
    return 0;
}