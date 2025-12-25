#include <stdio.h>
#include <string.h>
#include <math.h>
#include <algorithm>
constexpr int N = 100005;
int facs[N];
inline void Main() {
    int h, n;
    scanf("%d%d", &h, &n);
    if (h % n) return puts("0"), void();
    int tot = 0;
    for (int i = 1, rg = sqrt(h); i <= rg; ++i)
        if (h % i == 0) facs[++tot] = i;
    for (int i = tot - (facs[tot] * facs[tot] == h); i >= 1; --i)
        facs[++tot] = h / i;
    printf("%d\n", (int)(std::upper_bound(facs+1, facs+1+tot, h / n) - facs - 1));
}
signed main() {
    int T; scanf("%d", &T);
    while (T--) Main();
    return 0;
}
