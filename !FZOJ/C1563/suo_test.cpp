#include <cstdio>
#include <algorithm>
constexpr int N = 10;
int ans[N], m, n, a[N];
inline void check() {
    int h[N]{}, p[N]{};
    for (int i = 1; i <= n; i++) {
        p[i] = std::max(p[i-1], a[i]);
        if (p[i] > p[i-1] + 1) return;
        ++h[a[i]];
    }
    for (int i = 1; i <= n; i++) printf("%d%c", a[i], i==n?'\n':' ');
    for (int i = 1; i <= n; i++) (ans[i] += h[i] * h[i]) %= m;
}
void dfs(int p) {
    if (p > n) return check();
    for (int i = 1; i <= n; i++)
        a[p] = i, dfs(p+1);
}
int main() {
    scanf("%d%d", &n, &m);
    dfs(1);
    for (int i = 1; i <= n; i++)
        printf("%d ", ans[i]);
    return 0;
}