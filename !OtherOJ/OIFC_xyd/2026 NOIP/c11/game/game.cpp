#include <bits/stdc++.h>
constexpr int N = 1000005;
int h[N];
int main() {
    freopen("game.in", "r", stdin);
    freopen("game.out", "w", stdout);
    int Cid, Q;
    scanf("%d%d", &Cid, &Q);
    if (Cid <= 6) {
        int mx = 0;
        for (int op, x, y; Q--; ) {
            scanf("%d%d", &op, &x);
            if (op == 1) {
                scanf("%d", &y);
                h[x] = y;
                mx = std::max(mx, x);
            } else {
                if (x <= h[1] + h[2] / 2) printf("%d\n", x);
                else if (x <= h[1] + h[2]) printf("%d\n", 3 * x - 2 * (h[1] + h[2] / 2) - h[2] % 2);
                else {
                    int64_t ans = 3 * (h[1] + h[2]) - 2 * (h[1] + h[2] / 2) - h[2] % 2;
                    x -= h[1] + h[2];
                    for (int i = 3; x; i++) {
                        if (x >= h[i]) {
                            x -= h[i];
                            ans += 1ll * i * h[i];
                        } else {
                            ans += 1ll * i * x;
                            x = 0;
                        }
                    }
                    printf("%lld\n", ans);
                }
            }
        }
    } else if (Cid <= 8) {
        for (int op, x; Q--; ) {
            scanf("%d%d", &op, &x);
            if (op == 1) scanf("%*d");
            else printf("%d\n", x);
        }
    } else if (Cid <= 11) {
        for (int op, x, y; Q--; ) {
            scanf("%d%d", &op, &x);
            if (op == 1) scanf("%d", &y), h[x] = y;
            else if (x <= h[1] + h[2] / 2) printf("%d\n", x);
            else printf("%d\n", 3 * x - 2 * (h[1] + h[2] / 2) - h[2] % 2);
        }
    }
}