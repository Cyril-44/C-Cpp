#include <bits/stdc++.h>
constexpr int N = 3000005;
int64_t exgcd(int64_t a, int64_t b, int64_t &x, int64_t &y) {
    if (b == 0) {
        x = 1, y = 0;
        return a;
    }
    int64_t g = exgcd(b, a%b, y, x);
    y -= a / b * x;
    return g;
}
int main() {
    freopen("or.in", "r", stdin);
    freopen("or.out", "w", stdout);
    int T, n;
    scanf("%d", &T);
    while (T--) {
        scanf("%d", &n);
        switch (n) {
        case 1: puts("1"); break;
        case 2: puts("5 12"); break;
        case 3: puts("1 2 2"); break;
        case 4: puts("22 12 3 18"); break;
        default:
            for (int m = 0; m < 60; m++) {
                int64_t s = (1<<m+1) - n - 2;
                if (s <= 0) continue;
                int64_t x, y;
            }
        }
    }
}