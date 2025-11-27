#include <cstdio>
#include <algorithm>
#include <queue>
constexpr int M = 200005;
struct Node {
    int limit, reduce;
    inline bool operator<(const Node &x) const {
        return reduce > x.reduce || reduce == x.reduce && limit < x.limit;
    }
} bo[M];
int main() {
    long long n;
    int m;
    scanf("%lld%d", &n, &m);
    std::priority_queue<Node> q;
    for (int i = 1, a, b; i <= m; i++) {
        scanf("%d%d", &a, &b);
        bo[i] = {a, a - b};
        q.push(bo[i]);
    }
    long long ans(0);
    for (Node tp; n; ) {
        do {tp = q.top(); q.pop();}
        while (!q.empty() && tp.limit > n);
        if (q.empty()) break;
        ans += (n - tp.limit) / tp.reduce + 1;
        n = (n - tp.limit) % tp.reduce + tp.limit - tp.reduce;
    }
    printf("%lld\n", ans);
    return 0;
}