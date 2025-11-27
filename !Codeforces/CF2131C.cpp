#include <cstdio>
#include <algorithm>
#include <cstring>
constexpr int N = 200005;
int s[N], t[N];
int main() {
    int T, n, k;
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d", &n, &k);
        for (int i = 1; i <= n; i++)
            scanf("%d", s+i);
        for (int i = 1; i <= n; i++)
            scanf("%d", t+i);
        for (int i = 1; i <= n; i++) {
            s[i] = std::min(s[i] % k, k - s[i] % k);
            t[i] = std::min(t[i] % k, k - t[i] % k);
        }
        std::sort(s+1, s+1 + n);
        std::sort(t+1, t+1 + n);
        puts(memcmp(s+1, t+1, sizeof(int) * n) == 0 ? "Yes" : "No");
    }
    return 0;
}