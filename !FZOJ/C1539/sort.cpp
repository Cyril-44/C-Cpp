// OJ 4860
#include <stdio.h>
#include <algorithm>
#include <functional>
constexpr int N = 505;
int a[N], b[N];
int ans[N][3];
int from[N];
inline void solve() {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    for (int i = 1; i <= n; i++)
        scanf("%d", &b[i]);
    int tot = 1;
    ans[1][0] = 1, ans[1][1] = n, ans[1][2] = 0;
    for (int i = 1; i <= n; i++)
        a[i] = i, from[i] = i;
    for (int i = 1; i <= n; i++) {
        if (b[i] == a[i]) continue;
        ++tot;
        ans[tot][0] = i, ans[tot][1] = from[b[i]];
        if (a[i] < b[i]) {
            ans[tot][2] = 1;
            std::sort(a+i, a+from[b[i]]+1, std::greater<int>());
        } else {
            ans[tot][2] = 0;
            std::sort(a+i, a+from[b[i]]+1);
        }
        for (int j = i; j <= ans[tot][1]; j++)
            from[a[j]] = j;
    }
    printf("%d\n", tot);
    for (int i = 1; i <= tot; i++)
        printf("%d %d %c\n", ans[i][0], ans[i][1], ans[i][2] ? 'D' : 'I');
}
int main(int argc, char** argv) {
    int T = 1;
    if (argc == 2) T = std::atoi(argv[1]);
    while (T--) solve();
    return 0;
}