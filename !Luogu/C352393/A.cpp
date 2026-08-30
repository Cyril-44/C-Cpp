#include <cstdio>
#include <algorithm>
constexpr int N = 100004;
int a[N];
int main() {
    int T, n;
    scanf("%d", &T);
    while (T--) {
        scanf("%d", &n);
        for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
        printf("%d\n", std::max(a[1], a[n]));
    }
    return 0;
}