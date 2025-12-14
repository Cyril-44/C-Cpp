#include <cstdio>
#include <cstring>
#include <algorithm>
using std::max;
const int N = 2005;
typedef long long LL;
struct Node {
    int n, p;
    inline bool operator< (const Node &x) const {
        return n < x.n;
    }
} a[N];
LL f[N][N];
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
        scanf("%d", &a[i].n), a[i].p = i+1;
    std::sort(a, a + n);
    for (int k = 0; k < n; k++)
        for (int l = 1, r; l+k <= n; l++) {
            r = l + k;
            f[l][r] = max(f[l][r-1] + a[k].n * llabs(a[k].p - r), f[l+1][r] + a[k].n * llabs(a[k].p - l));
        }
    printf("%lld\n", f[1][n]);
    return 0;
}
