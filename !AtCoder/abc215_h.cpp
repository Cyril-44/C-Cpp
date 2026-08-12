#include <cstdio>
#include <cstring>
#include <algorithm>
constexpr int N = 20, M = 10005;
int a[N], b[M], sb[1 << N];
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int i = 0; i < n; i++)
        scanf("%d", &a[i]);
    for (int i = 1; i <= m; i++)
        scanf("%d", &b[i]);
    for (int i = 1; i <= m; i++) {
        int s = 0;
        for (int j = 0, v; j < n; j++)
            scanf("%d", &v), s |= v << j;
        sb[s] += b[i];
    }
    for (int i = 0; i < n; i++)
        for (int j = 0; j < (1<<n); j++)
            if (j >> i & 1) sb[j] += sb[j ^ (1<<i)];
    int ansx = 0x7fffffff;
    for (int s = 0; s < (1<<n); s++) {
        int sa = 0;
        for (int i = 0; i < n; i++)
            if (s >> i & 1) sa += a[i];
        ansx = std::min(ansx, sa - sb[s] + 1);
    }
    return 0;
}