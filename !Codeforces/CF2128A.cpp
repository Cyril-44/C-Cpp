#include <cstdio>
#include <algorithm>
#include <cstring>
const int N = 35;
int a[N];
bool f[N];
int main() {
    int t, n, c;
    scanf("%d", &t);
    while (t--) {
        scanf("%d%d", &n, &c);
        for (int i = 1; i <= n; i++) scanf("%d", a + i);
        std::sort(a+1, a+1 + n);
        memset(f, 0, sizeof f);
        for (int i = 1; i <= n; i++) {
            if (a[i] > c) continue;
            int cnt(0);
            while (a[i] <= c) a[i] <<= 1, ++cnt;
            while (f[--cnt]) if (cnt == 0) break;
            f[cnt] = true;
        }
        int ans = n;
        for (int i = 0; i <= 32; i++)
            if (f[i]) --ans;
        printf("%d\n", ans);
    }
    return 0;
}