#include <bits/stdc++.h>
constexpr int M = 200005;
int64_t b[M];
int m;
inline int64_t min(int64_t x, int64_t y) { return x < y ? x : y; }
inline int64_t max(int64_t x, int64_t y) { return x > y ? x : y; }
int64_t dfs(int i, std::vector<int64_t> v, bool tie) {
    if (i > m) return std::accumulate(v.begin(), v.end(), 0ll);
    std::vector<int64_t> a[2]; a[0].reserve(v.size()), a[1].reserve(v.size());
    for (auto x : v) a[x % b[i] == 0].push_back(x);
    if (a[0].empty() || a[1].empty()) {
        if (tie) return 0;
        return (i&1 ? min : max)(0, dfs(i+1, a[a[0].empty()], true));
    }
    return (i&1 ? min : max)(dfs(i+1, a[0], false), dfs(i+1, a[1], false));
}
int main() {
    int n;
    scanf("%d%d", &n, &m);
    std::vector<int64_t> v;
    v.reserve(n);
    while (n--) {
        int64_t ai;
        scanf("%lld", &ai);
        if (ai) v.push_back(ai);
    }
    for (int i = 1; i <= m; i++)
        scanf("%lld", &b[i]);
    printf("%lld\n", dfs(1, v, 0));
}