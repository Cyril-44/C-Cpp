#if 1
#include <vector>
void energy(int n, std::vector<int> v);
void rotate(std::vector<int> t, int x);
#elif defined(ONLINE_JUDGE)
#include "rotate.h"
#else
#include "P12543.h"
#endif
#include <algorithm>
#include <numeric>
void energy(int n, std::vector<int> v) {
    std::vector<int> id(n);
    std::iota(id.begin(), id.end(), 0);
    std::sort(id.begin(), id.end(), [&](int x, int y) { return v[x] < v[y]; });
    for (int i = 0; i < n/2; i++) rotate({id[i+n/2]}, 25000 - (v[id[i+n/2]] - v[id[i]]));
}
/* #include <queue>
static constexpr int N = 100005, D = 50000;
static struct Node {
    int lp, rp, l, r, sz, deg;
} a[N];
void energy(int n, std::vector<int> v) {
    std::vector<int> id(n), exp(n);
    std::iota(id.begin(), id.end(), 0);
    std::sort(id.begin(), id.end(), [&](int x, int y) { return v[x] < v[y]; });
    auto prev = [&](int x) { return x ? x - 1 : n - 1; };
    auto next = [&](int x) { return x == n - 1 ? 0 : x + 1; };
    auto gap = [&](int x) { return a[x].deg - a[a[x].lp].deg + (a[x].l > a[x].r) * D; };
    std::priority_queue<std::pair<int,int>> pq;
    for (int i = 0; i < n; i++) {
        exp[i] = D * (i+1) / n - D * i / n;
        a[i] = {prev(i), next(i), prev(i), i, 1, v[id[i]]};
        pq.emplace(gap(i), i);
    }
    while (pq.size() > 1) {
        auto [deg, ptr] = pq.top(); pq.pop();
        Node &n1 = a[a[ptr].lp], &n2 = a[ptr];
        if (n1.sz < n2.sz) {
        }
    }
} */
