#include <bits/extc++.h>
using namespace __gnu_pbds;
constexpr int N = 1000005;
using PQ = priority_queue<int, std::greater<int>>;
PQ pq[N];
PQ::point_iterator pit[N];
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int i = 1, x; i <= n; i++) {
        scanf("%d", &x);
        pit[i] = pq[i].push(x);
    }
    for (int opt, x, y, z; m--; ) {
        scanf("%d%d", &opt, &x);
        switch (opt) {
        case 0:
            scanf("%d", &y);
            pq[x].erase(pit[y]);
            break;
        case 1:
            printf("%d\n", pq[x].top());
            break;
        case 2:
            scanf("%d", &y);
            pq[x].join(pq[y]);
            break;
        case 3:
            scanf("%d%d", &y, &z);
            pq[x].modify(pit[y], z);
        }
    }
    return 0;
}