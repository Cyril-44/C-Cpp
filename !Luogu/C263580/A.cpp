#include <cstdio>
#include <algorithm>
#include <cstring>
const int N = 250005;
int a[N];
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) scanf("%d", a + i);
    int ans(0);
    a[0] = -1;
    for (int i = 1; i <= n; i++) {
        if (a[i] - a[i-1] == 1) {
            puts("-1");
            return 0;
        }
        ans += (a[i] - a[i-1] - 1) / 2 + 1;
    }
    printf("%d\n", ans);
    return 0;
}