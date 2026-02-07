#include <stdio.h>
#include <algorithm>
constexpr int N = 2000005;
int h[N], f[N];
int main() {
    int n;
    scanf("%d", &n);
    int mx = 0;
    for (int i = 1; i <= n; i++) {
        int ai;
        scanf("%d", &ai);
        ++h[ai];
        mx = std::max(mx, ai);
    }
    for (int i = 1; i <= mx; i++) {
        f[0] += h[i], f[i] -= h[i];
    }
    mx += 1000000;
    for (int i = 1; i <= mx; i++)
        f[i] += f[i-1];
    for (int i = 0; i <= mx; i++) {
        if (f[i] >= 10) f[i + 1] += f[i] / 10, f[i] %= 10;
    }
    while (mx > 0 && !f[mx]) --mx;
    for (int i = mx; i >= 0; i--)
        putchar(f[i] | '0');
    putchar('\n');
    return 0;
}