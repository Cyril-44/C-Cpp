#include <stdio.h>
#include <set>
const int N = 200005;
int a[N], b[N];
std::set<int> h[N];
int main() {
    int t, n, m, q, s, t;
    scanf("%d", &t);
    while (t--) {
        scanf("%d%d%d", &n, &m, &q);
        for (int i = 0; i < n; i++)
            h[i].clear();
        for (int i = 0; i < n; i++)
            scanf("%d", &a[i]);
        for (int i = 1; i <= m; i++) {
            scanf("%d", &b[i]);
            h[b[i]].insert(i);
        }
    }
    return 0;
}