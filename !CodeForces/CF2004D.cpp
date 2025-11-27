#include <stdio.h>
#include <string.h>
const int N = 200005;
int a[N][2], pre[N], nxt[N];
inline int op(const char &c) {
    switch(c) {
        case 'B': return 1;
        case 'G': return 2;
        case 'R': return 3;
        case 'Y': return 4;
    }
    return 0;
}
inline bool connected(const int &i, const int &j) {
    return a[i][0] == a[j][0] || a[i][0] == a[j][1] ||
           a[i][1] == a[j][0] || a[i][1] == a[j][1];
}
inline int abs(const int &x) { return x < 0 ? -x : x; }
inline void umax(int &x, const int &y) { x = x > y ? x : y; }
inline void umin(int &x, const int &y) { x = x < y ? x : y; }
int main() {
    int t, n, q, x, y, rcd[5][5], ans;
    char c1, c2;
    scanf("%d", &t);
    while (t--) {
        scanf("%d%d", &n, &q);
        for (int i = 1; i <= n; i++) {
            scanf(" %c%c", &c1, &c2);
            a[i][0] = op(c1);
            a[i][1] = op(c2);
        }
        memset(rcd, 0, sizeof rcd);
        for (int i = 1; i <= n; i++) {
            pre[i] = 0;
            // Search for (x0, ?)
            for (int j = 1; j <= 4; j++)
                if (j != a[i][0] && j != a[i][1] && connected(rcd[a[i][0]][j], i))
                    umax(pre[i], rcd[a[i][0]][j]);
            // Search for (x1, ?)
            for (int j = 1; j <= 4; j++)
                if (j != a[i][0] && j != a[i][1] && connected(rcd[a[i][1]][j], i))
                    umax(pre[i], rcd[a[i][1]][j]);
            // Record current stat
            rcd[a[i][0]][a[i][1]] = rcd[a[i][1]][a[i][0]] = i;
        }
        memset(rcd, 0, sizeof rcd);
        for (int i = n; i >= 1; --i) {
            nxt[i] = n+1;
            // Search for (x0, ?)
            for (int j = 1; j <= 4; j++)
                if (j != a[i][0] && j != a[i][1] && connected(rcd[a[i][0]][j], i))
                    umin(nxt[i], rcd[a[i][0]][j]);
            // Search for (x1, ?)
            for (int j = 1; j <= 4; j++)
                if (j != a[i][0] && j != a[i][1] && connected(rcd[a[i][1]][j], i))
                    umin(nxt[i], rcd[a[i][1]][j]);
            // Record current stat
            rcd[a[i][0]][a[i][1]] = rcd[a[i][1]][a[i][0]] = i;
        }
        while (q--) {
            scanf("%d%d", &x, &y);
            if (connected(x, y)) printf("%d\n", abs(x - y));
            else {
                ans = 0x3fffffff;
                if (pre[x]) umin(ans, abs(x-pre[x]) + abs(pre[x]-y));
                if (nxt[x] ^ n+1) umin(ans, abs(x-nxt[x]) + abs(nxt[x]-y));
                printf("%d\n", (ans ^ 0x3fffffff) ? ans : -1);
            }
        }
    }
    return 0;
}