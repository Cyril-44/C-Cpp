#include <cstdio>
const int N = 500005;
int a[N];
bool v[N];
inline int length(int x) {
    int ret = 1;
    while (x /= 10) ++ret;
    return ret;
}
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    int cnt(0);
    for (int i = 1, x; i <= n; i++) {
        scanf("%d", &x);
        if (!v[x]) { v[x] = true, cnt += length(x) + 1; }
        a[x] = cnt - 1;
    }
    for (int i = 1; i <= m; i++)
        printf("%d ", a[i]);
    putchar('\n');
    return 0;
}