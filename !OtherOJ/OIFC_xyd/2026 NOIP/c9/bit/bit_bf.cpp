#include <bits/stdc++.h>
constexpr int N = 5005;
std::bitset<N> f[N];
struct {int u, v;} e[N];
int main() {
    int n, q;
    scanf("%d%d", &n, &q);
    for (int i = 1; i < n; i++)
        scanf("%d%d", &e[i].u, &e[i].v);
    for (int i = 1; i <= n; i++)
        f[i].set(i);
    for (int op, l, r; q--; ) {
        scanf("%d%d", &op, &l);
        if (op == 1) {
            scanf("%d", &r);
            int res = 0;
            for (int i = l; i <= r; i++)
                res += f[i].count();
            printf("%d\n", res);
        } else {
            f[e[l].u] = f[e[l].v] = f[e[l].u] | f[e[l].v];
        }
    }
    return 0;
}