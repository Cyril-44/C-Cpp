#include <cstdio>
#include <algorithm>
#include <cstring>
#include <queue>
const int N = 100005;
struct Node {
    int l, r, x;
    inline bool operator<(const Node &rls) const { // for heap
        return x < rls.x;
    }
} a[N];
int main() {
    int t, n, k;
    scanf("%d", &t);
    while (t--) {
        scanf("%d%d", &n, &k);
        for (int i = 1; i <= n; i++)
            scanf("%d%d%d", &a[i].l, &a[i].r, &a[i].x);
        std::sort(a+1, a+1+n, [](const Node &x, const Node &y) {return x.l < y.l;});
        std::priority_queue<Node> q;
        Node tp;
        for (int p = 1; p <= n; ) {
            while (p <= n && a[p].l <= k) q.push(std::move(a[p++]));
            if (q.empty()) break;
            while (!q.empty()) {
                tp = q.top(); q.pop();
                if (tp.r >= k && tp.x > k) {
                    k = tp.x;
                    break;
                }
            }
        }
        printf("%d\n", k);
    }
    return 0;
}