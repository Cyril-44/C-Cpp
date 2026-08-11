#include <cstdio>
#include <set>
#include <algorithm>
#include <tuple>
constexpr int N = 500005;
int a[N], pre[N], nxt[N];
int main() {
    int tid, n, m;
    scanf("%d%d%d", &tid, &n, &m);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]), nxt[i] = i+1, pre[i] = i-1;
    nxt[n] = 1, pre[1] = n;
    std::set<std::tuple<int,int,int>> pq;
    auto ins = [&] (int p) { pq.emplace(a[p] + a[nxt[p]], std::max(a[p], a[nxt[p]]), p); };
    auto era = [&] (int p) { pq.erase({a[p] + a[nxt[p]], std::max(a[p], a[nxt[p]]), p}); };
    for (int i = 1; i <= n; i++) ins(i);
    auto del = [&] (int p) {
        era(pre[p]), era(p);
        pre[nxt[p]] = pre[p];
        nxt[pre[p]] = nxt[p];
        ins(pre[p]);
    };
    while (pq.size() > m) {
        auto [_, __, id] = *pq.rbegin();
        if (a[id] > a[nxt[id]]) del(id);
        else del(nxt[id]);
    }
    printf("%d\n", std::get<0>(*pq.rbegin()));
    return 0;
}