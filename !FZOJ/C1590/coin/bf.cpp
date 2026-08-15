#include <cstdio>
constexpr int N = 15;
int a[N], b[N], h[N], ans[N], n, m;
inline void umax(int &x, int y) { if (x < y) x = y; }
void dfs(int u, int cur) {
    umax(ans[u-1], cur);
    if (u > m) return;
    for (int i = 1; i <= n; i++) {
        switch (++h[i]) {
            case 1: dfs(u+1, cur + a[i]); break;
            case 2: dfs(u+1, cur + b[i]); break;
            case 3: dfs(u+1, cur + a[i]); break;
        }
        --h[i];
    }
}
int main() {
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++) scanf("%d", a+i);
    for (int i = 1; i <= n; i++) scanf("%d", b+i);
    dfs(1, 0);
    int xans = 0;
    for (int i = 1; i <= m; i++)
        // printf("%d%c", ans[i], " \n"[i==m]), 
        xans ^= ans[i];
    printf("%d\n", xans);
    return 0;
}