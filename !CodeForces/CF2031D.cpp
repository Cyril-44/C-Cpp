#include <cstdio>
#include <cstring>
#include <iostream>
using std::max;
const int N = 500005;
int a[N], f[N], tr[N], ans[N], n;
inline void upd(int p, const int &x) {
    for (; p <= n; p += p & -p)
        tr[p] = max(tr[p], x);
}
inline int query(int p) {
    int res = 0;
    for (; p > 0; p -= p & -p)
        res = max(res, tr[p]);
    return res;
}
int main() {
    int t;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        for (int i = 1; i <= n; i++)
            scanf("%d", &a[i]);
        for (int i = 1; i <= n; i++)
            f[i] = max(f[i-1], a[i]);
        f[n+1] = -1;
        memset(tr, 0, sizeof(int) * (n+1));
        int cur = -1;
        for (int i = n; i > 0; i--) {
            if (f[i] ^ f[i+1]) {
                cur = max(f[i], query(f[i] - 1));
            }
            ans[i] = cur;
            upd(a[i], cur);
        }
        for (int i = 1; i <= n; i++)
            printf("%d ", ans[i]);
        putchar('\n');
    }
    return 0;
}