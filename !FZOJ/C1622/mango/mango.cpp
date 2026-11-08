#include <bits/stdc++.h>
constexpr int N = 100005;

int main() {
    int64_t n; int m;
    scanf("%lld%d", &n, &m);
    std::map<int64_t, int> diff;
    std::map<int64_t, int64_t> vals;
    diff[1] = diff[n+1] = 0;
    for (int64_t l, r; m--; ) {
        scanf("%lld%lld", &l, &r);
        ++diff[l], --diff[r+1];
    }
    int64_t cursum = 0;
    for (auto i1 = diff.begin(), i2 = std::next(i1); i2 != diff.end(); ++i1, ++i2) {
        cursum += i1->second;
        const int64_t l = i1->first, r = i2->first - 1;
        for (int64_t i = l, j; i <= r; i = j+1) {
            const int64_t now = n / i;
            j = std::min(r, n / now);
            vals[cursum * now] += j - i + 1;
        }
    }
    std::priority_queue<std::pair<int64_t, int64_t>, std::vector<std::pair<int64_t, int64_t>>, std::greater<std::pair<int64_t, int64_t>>> pq;
    for (const auto &[p, c] : vals)
        if (p == 0) pq.emplace(p, 1);
        else pq.emplace(p, c);
    int64_t ans = 0;
    while (!(pq.size() == 1 && pq.top().second == 1)) {
        auto [p, c] = pq.top(); pq.pop();
        if (!c) continue;
        if (c == 1) {
            auto [pp, cc] = pq.top(); pq.pop();
            ans += p + pp;
            pq.emplace(p + pp, 1);
            if (cc > 1) pq.emplace(pp, cc - 1);
            n -= p;
        } else {
            ans += p * (c - c%2);
            pq.emplace(p << 1, c >> 1);
            n -= c - c/2;
        }
    }
    printf("%lld\n", ans);
}