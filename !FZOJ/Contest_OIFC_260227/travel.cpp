#include <cstdio>
#include <cmath>
#include <algorithm>
inline int abs(int x) { return x < 0 ? -x : x; }
constexpr int N = 100005;
struct Node { int s, t; } a[N];
int main() {
    freopen("travel.in", "r", stdin);
    freopen("travel.out", "w", stdout);
    int T, n, m;
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d", &n, &m);
        for (int i = 1; i <= n; i++)
            scanf("%d%d", &a[i].s, &a[i].t);
        if (n == 2) {
            auto direct = [&]() {
                printf("%d 2\n", std::abs(a[1].s - a[1].t) + std::abs(a[2].s - a[2].t));
                printf("0 1 %d\n", a[1].t);
                printf("0 2 %d\n", a[2].t);                
            };
            if ((a[1].s > a[1].t) == (a[2].s > a[2].t)) {
                direct();
            } else {
                int flg = 0;
                if (a[1].s > a[1].t) std::swap(a[1], a[2]), flg = 1;
                int ans1 = std::abs(a[1].s-a[1].t) + std::abs(a[2].s-a[2].t),
                    ans2 = std::abs(a[1].s-a[2].t) + std::abs(a[1].t-a[2].s);
                int tot = 0;
                int ans[10][3];
                auto push = [&](int op, int x, int y) { if (flg) x = 3 - x; ++tot; ans[tot][0] = op, ans[tot][1] = x, ans[tot][2] = y; };    
                if (ans1 <= ans2) {
                    printf("%d 2\n", ans1);
                    push(0, 1, a[1].t);
                    push(0, 2, a[2].t);
                    for (int i = 1; i <= 2; i++)
                        printf("%d %d %d\n", ans[i][0], ans[i][1], ans[i][2]);
                }
                else {
                    printf("%d ", ans2);
                    push(0, 2, a[2].t);
                    if (a[1].s != a[2].t) push(0, 1, a[2].t);
                    push(1, 1, 2);
                    if (a[1].t != a[2].t) push(0, 1, a[1].t);
                    printf("%d\n", tot);
                    for (int i = 1; i <= tot; i++)
                        printf("%d %d %d\n", ans[i][0], ans[i][1], ans[i][2]);
                }
            }
        }
        if (m == 2) {
            
        }
    }
    return 0;
}