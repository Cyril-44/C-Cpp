#include <cstdio>
#include <vector>
#ifdef CLANGD
constexpr int N = 24;
#else
constexpr int N = 200004;
#endif
int a[N], b[N];
std::vector<int> pos[N];
int main() {
    int tid, T, n, k;
    scanf("%d%d", &tid, &T);
    while (T--) {
        scanf("%d%d", &n, &k);
        for (int i = 0; i <= n; i++) pos[i].clear();
        for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
        for (int i = 1; i <= n; i++) scanf("%d", &b[i]);
        for (int i = 1; i <= n; i++)
            if (a[i] == b[i] && a[i] <= n) pos[a[i]].push_back(i);
        if (pos[0].size() == n) {
            printf("%d\n", n-k);
            continue;
        }
        long long ans = -(1ll << 62);
        for (int i = 0; i <= n; i++) {
            int last = 0;
            pos[i].push_back(n+1);
            for (int p : pos[i]) {
                ans = std::max(ans, p - last - 1ll - 1ll * k * i);
                last = p;
            }
            if (pos[i].size() == 1) break;
        }
        printf("%lld\n", ans);
    }
    return 0;
}