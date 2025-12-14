#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
#include <queue>
using std::max;
const int N = 1000005;
int fa[N], lvl[N], cnt[N], mxl;
std::vector<int> g[N];
inline int calc(const int &u) {
    if (g[u].empty()) return 0;
    std::vector<int> res;
    for (const int& v : g[u]) {
        res.emplace_back(calc(v));
    }
    if (g[u].size() == 1) return res[0] + 1;
    else if (g[u].size() == 2) return max(res[0] + 1, res[1] + 1);
    std::priority_queue<int, std::greater<int>, std::vector<int>> q(res);
    while (q.size() > 1) {
        int t1 = q.top(); q.pop();
        int t2 = q.top(); q.pop();
        q.push(t1 + 1);
    }
    return q.top();
}
int main() {
    int t, n;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        for (int i = 1; i <= n; i++)
            g[i].clear();
        for (int i = 2; i <= n; i++) {
            scanf("%d", &fa[i]);
            g[fa[i]].emplace_back(i);
        }
        lvl[1] = 0, mxl = 0;
        printf("%d\n", calc(1));
    }
    return 0;
}