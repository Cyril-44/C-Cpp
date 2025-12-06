#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <queue>
constexpr int N = 10005;
struct Range {
    int l, r;
    inline bool operator< (const Range &rhs) const {
        return r < rhs.r || r == rhs.r && l < rhs.l;
    }
} rg[N];
inline void solveSingleTestCase() {
    int n, a;
    scanf("%d%d", &n, &a);
    for (int i = 1; i <= n; i++)
        scanf("%d%d", &rg[i].l, &rg[i].r);
    std::sort(rg+1, rg+1 + n);
    int pt = 1;
    for (int i = 1, j = 1; j <= n; i = j) {
        std::priority_queue<int> pq;
        while (j <= n && rg[j].r < rg[i].r + a) pq.push(rg[j++].l);
        // from rg_i.r to rg_{j-1}.r
        int now = rg[j-1].r;
        while (!pq.empty()) {
            now -= a;
            if (now >= pq.top()) pq.pop();
            else return puts("no"), void();
        }
    }
    puts("yes");
}
int main() {
    freopen("block.in", "r", stdin);
    freopen("block.out", "w", stdout);
    int T;
    scanf("%d", &T);
    while (T--) solveSingleTestCase(); 
    return 0;
}