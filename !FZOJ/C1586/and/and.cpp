#include <bits/stdc++.h>
constexpr int N = 1000005;
struct Node {
    int l, r;
    uint64_t x;
} a[N];
int n, m;
inline void BF() {
    static int f[N];
    for (int i = 1; i <= m; i++) {
        bool ans = true;
        for (int d = 0; d < 60; d++) {
            memset(f, 0, sizeof f);
            for (int j = 1; j <= m; j++)
                if (j != i && (a[j].x >> d & 1))
                    ++f[a[j].l], --f[a[j].r+1];
            int tot = 0;
            for (int j = 1; j <= n; j++) f[j] += f[j-1];
            for (int j = 1; j <= n; j++)
                if (f[j])
                    if (f[j-1]) f[j] = tot;
                    else f[j] = ++tot;
            bool flg = true;
            for (int j = 1; j <= m; j++)
                if (j != i && !(a[j].x >> d & 1) && f[a[j].l] && f[a[j].l] == f[a[j].r])
                    { flg = false; break; }
            if (!flg) {
                ans = false; break;
            }
        }
        putchar(ans + '0');
    }
    putchar('\n');
}
int main() {
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= m; i++)
        scanf("%d%d%llu", &a[i].l, &a[i].r, &a[i].x);
    if (m == 1) puts("1");
    else if (m == 2) puts("11");
    else if (n == 1) {
        uint64_t pivot = (a[1].x == a[2].x ? a[1].x : a[1].x == a[3].x ? a[1].x : a[2].x);
        int unique = 0;
        for (int i = 1; i <= m; i++)
            if (a[i].x != pivot) {
                if (unique) { unique = -1; break; }
                unique = i;
            }
        if (unique == -1) {
            for (int i = 1; i <= m; i++) putchar('0');
            putchar('\n');
        }
        else if (!unique) {
            for (int i = 1; i <= m; i++) putchar('1');
            putchar('\n');
        }
        else {
            for (int i = 1; i <= m; i++) putchar((i == unique) + '0');
            putchar('\n');
        }
    }
    else if (n <= 1000 && m <= 1000) BF();
    else {
        
    }
    return 0;
}