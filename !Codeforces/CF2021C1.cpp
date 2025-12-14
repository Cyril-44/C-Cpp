#include <stdio.h>
#include <string.h>
const int N = 200005;
int a[N], b[N], c[N];
bool h[N];
int main() {
    int t, n, m, q;
    scanf("%d", &t);
    while (t--) {
        scanf("%d%d%d", &n, &m, &q);
        memset(h, 0, n+1);
        for (int i = 0; i < n; i++)
            scanf("%d", &a[i]);
        c[0] = 0;
        for (int i = 0; i < m; i++) {
            scanf("%d", &b[i]);
            if (!h[b[i]]) {
                h[b[i]] = true;
                c[++c[0]] = b[i];
            }
        }
        bool flg = true;
        for (int i = 1; i <= c[0]; i++)
            if (a[i-1] ^ c[i]) flg = false;
        puts(flg ? "Ya" : "TIDAK");
    }
    return 0;
}