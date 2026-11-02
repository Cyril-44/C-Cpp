#include <bits/stdc++.h>
constexpr int N = 1000005;
char s[N];
int main() {
    freopen("retrace.in", "r", stdin);
    freopen("retrace.out", "w", stdout);
    int T, Tid, n, c;
    scanf("%d%d", &T, &Tid);
    while (T--) {
        scanf("%d%d %s", &n, &c, s+1);
        if (n & 1) { puts("N0"); continue; }
        puts("YE5");
        int x = 0, y = 0;
        int mxx = 0, mxy = 0;
        for (int i = 1; i <= n; i++) {
            switch (s[i]) {
                case 'U': ++y; break;
                case 'D': --y; break;
                case 'L': --x; break;
                case 'R': ++x; break;
            }
            switch (c) {
                case 1: case 2: if (y < 0) mxy = std::max(mxy, -y); break;
                case 3: case 4: if (y > 0) mxy = std::max(mxy, y);
            }
            switch (c) {
                case 1: case 4: if (x < 0) mxx = std::max(mxx, -x); break;
                case 2: case 3: if (x > 0) mxx = std::max(mxx, x);
            }
        }
        printf("%d\n", std::max((mxx+1) / 2 + (mxy+1) / 2, (std::abs(x)+1) / 2 + (std::abs(y)+1) / 2));
        puts(s+1);
    }
}