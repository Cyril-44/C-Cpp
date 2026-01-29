#include <stdio.h>
#include <queue>
#include <algorithm>
constexpr int N = 500005, LG = 19;
int sum[N];
namespace ST {
int st[N][LG+1];
inline int max(int x, int y) { return sum[x] > sum[y] ? x : y; }
inline void init(int n) {
    for (int i = 1; i <= n; i++) st[i][0] = i;
    for (int k = 1; k <= LG; k++)
        for (int i = 1, j = (1 << k-1) + 1, up = n - (1 << k) + 1; i <= up; i++, j++)
            st[i][k] = max(st[i][k-1], st[j][k-1]);
}
inline int query(int l, int r) {
    int lg = 31 - __builtin_clz(r - l + 1);
    return max(st[l][lg], st[r - (1 << lg) + 1][lg]);
}
}
struct Elem { // 表示 (l, [r.first, r.second]] 的最大
    int l; std::pair<int,int> r;
    inline int operator()() const {
        return sum[ST::query(r.first, r.second)] - sum[l];
    }
};
inline bool operator<(const Elem &x, const Elem &y) { return x() < y(); }
int main() {
    int n, k, L, R;
    scanf("%d%d%d%d", &n, &k, &L, &R);
    for  (int i = 1; i <= n; i++) {
        scanf("%d", &sum[i]);
        sum[i] += sum[i-1];
    }
    ST::init(n);
    std::priority_queue<Elem> pq;
    for (int i = 0; i+L <= n; i++)
        pq.push({i, {i+L, std::min(i+R,n)}});
    long long ans = 0;
    while (k--) {
        auto tp = pq.top(); pq.pop();
        int pos = ST::query(tp.r.first, tp.r.second);
        ans += sum[pos] - sum[tp.l];
        if (pos - 1 >= tp.r.first) pq.push({tp.l, {tp.r.first, pos-1}});
        if (pos + 1 <= tp.r.second) pq.push({tp.l, {pos+1, tp.r.second}});
    }
    printf("%lld\n", ans);
    return 0;
}