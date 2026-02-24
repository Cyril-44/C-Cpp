#include <cstdio>
#include <queue>
#include <algorithm>
using LL = long long;
using Pli = std::pair<LL, int>;
LL base[610];
LL calc(LL x) { return (x + 1) / 2 - (x / 3 + 1) / 2; }
int main() {
    std::priority_queue<Pli, std::vector<Pli>, std::greater<Pli>> pq;
    pq.emplace(1, 0);
    for (int x=0, y=0, pre=0, now=0; ; pre = now) {
        auto u = pq.top();
        while (!pq.empty() && pq.top() == u) pq.pop();
        u.second & 1 ?  y++ : x++;
        now = std::max(x, y);
        if (now != pre) base[++base[0]] = u.first;
        if (u.first > (LL)2e16) break;
        pq.emplace(u.first * 2, u.second + 1);
        pq.emplace(u.first * 3, u.second + 1);
    }
    freopen("stream.in", "r", stdin);
    freopen("stream.out", "w", stdout);
    int T; LL n;
    scanf("%d", &T);
    while (T--) {
        LL ans = 0;
        scanf("%lld", &n);
        for (int i = 1; base[i] <= n; i++) ans += calc(n / base[i]);
        printf("%lld\n", ans);
    }
}
