#include <cstdio>
#include <algorithm>
constexpr int N = 400004;
int a[N], h[N], n;
inline bool check(int n, int val) {
    int req = 1, all = n;
    for (int i = val-1, tp; i > 0; i--)
        tp = std::min(h[i], req), all -= tp, req += req - tp;
    return all >= req;
}
inline void bf(int n) {
    printf("%d", std::max(1, a[1]));
    int val = 1;
    ++h[a[1]];
    for (int i = 2; i <= n; i++) {
        ++h[a[i]];
        while (check(i, val+1)) ++val;
        printf(" %d", val);
    }
    putchar('\n');
}
int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    if (n <= 5000) bf(n);
    return 0;
}