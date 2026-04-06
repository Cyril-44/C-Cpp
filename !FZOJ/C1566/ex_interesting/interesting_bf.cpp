#include <cstdio>
#include <algorithm>
const int N = 500005;
int l[N], r[N], len[N], a[N];
bool vis[N], flg[N];
inline void solveSingle() {
    int n, m;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("%d%d", &l[i], &r[i]);
        a[(i << 1) - 1] = l[i];
        a[i << 1] = r[i];
        vis[i] = 0;
        flg[(i << 1) - 1] = flg[i << 1] = 1;
    }
    std::sort(a + 1, a + (n << 1) + 1);
    m = std::unique(a + 1, a + (n << 1) + 1) - a - 1;
    for (int i = 1; i <= n; i++) {
        len[i] = r[i] - l[i];
        l[i] = std::lower_bound(a + 1, a + m + 1, l[i]) - a;
        r[i] = std::lower_bound(a + 1, a + m + 1, r[i]) - a;
    }
    for (int i = 1; i <= n; i++) {
        int id = -1;
        for (int j = 1; j <= n; j++) {
            if (vis[j]) continue;
            if (id == -1)
                id = j;
            else if (len[id] > len[j])
                id = j;
        }
        printf("%d ", id);
        vis[id] = 1;
        for (int j = l[id]; j < r[id]; j++) {
            if (flg[j]) {
                int ll = a[j + 1] - a[j];
                for (int k = 1; k <= n; k++)
                    if (l[k] <= j && r[k] > j) len[k] -= ll;
                flg[j] = 0;
            }
        }
    }
    putchar('\n');
}
int main() {
    int t, sid;
    scanf("%d%d", &t, &sid);
    while (t--) solveSingle();
    return 0;
}