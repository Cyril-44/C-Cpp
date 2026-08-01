#include <cstdio>
#include <cstring>
#include <cstdint>
constexpr int N = 100005;
int64_t p[N];
bool t[N];
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 2, d; i <= n; i++) {
        scanf("%d", &d);
        p[i] = p[i-1] + d;
    }
    for (int i = 1, x; i <= n; i++)
        scanf("%d", &x), t[i] = x;
    int ans = 0;
    for (int i = 1, j; i <= n; i = j) {
        for (j = i+1; t[j]; j++);
        int64_t beg = p[i], end = p[j];
        int m = j - i;
        for (int k = i+1; k < j; k++)
            if ((__int128)m * (p[k] - beg) != (__int128)(k - i) * (end - beg))
                ++ans;
    }
    printf("%d\n", ans);
    return 0;
}