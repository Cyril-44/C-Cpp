#include <cstdio>
#include <cstring>
#include <algorithm>
#include <numeric>
constexpr int N = 200005;
int col[N], a[N], id[N], cnt[N], h[N];
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    if (m > 10) return 0;
    for (int i = 1; i <= m; i++)
        scanf("%d", &col[i]);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    std::iota(id+1, id+1+m, 1);
    do {
        int i;
        for (i = 1; i <= m; i++) {
            if (++h[col[id[i]]] >= a[col[id[i]]]) {
                ++cnt[col[id[i]]];
                break;
            }
        }
        for (int j = 1; j <= i; j++) --h[col[id[j]]];
    } while (std::next_permutation(id+1, id+1+m));
    for (int i = 1; i <= n; i++)
        printf("%d\n", cnt[i]);
    return 0;
}