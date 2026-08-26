#include <cstdio>
#include <cstring>
#include <algorithm>
constexpr int N = 1005;
int f[N][N];

int dfs(int u, int v) {
    if (v > 1000) return 114514;
    if (~f[u][v]) return f[u][v];
    int tu = u+1, tv = v, gt = std::__gcd(tu, tv);
    tu /= gt, tv /= gt;
    int lu = u+1, lv = v+1, gl = std::__gcd(lu, lv);
    lu /= gl, lv /= gl;
    return f[u][v] = std::min(dfs(tu, tv), dfs(lu, lv)) + 1;
}
int main() {
    int n;
    scanf("%d", &n);
    memset(f, -1, sizeof f);
    f[1][1] = 0;
    for (int i = 1; i <= n; i++)
        printf("%d%c", dfs(0, i), " \n"[i==n]);
    return 0;
}