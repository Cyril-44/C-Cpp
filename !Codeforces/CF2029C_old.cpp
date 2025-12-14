#include <stdio.h>
#include <string.h>
#include <iostream>
using std::max;
const int N = 300005;
int a[N], mx[N][2], n;
int _cnt[N<<1], tr[N<<1], *cnt = _cnt + N;
inline void add(int p) {
    for (p += n+1; p <= (n+1<<1); p += p & -p)
        ++tr[p];
}
inline int query(int p) {
    int res = 0;
    for (p += n+1; p > 0; p -= p & -p)
        res += tr[p];
    return res;
}
int main() {
    int t;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        memset(cnt - n, 0, sizeof(int) * (2 * n + 1));
        memset(tr, 0, sizeof(int) * 2 * (n + 1));
        int p = 0;
        for (int i = 1; i <= n; i++) {
            scanf("%d", &a[i]);
            a[i] -= p;
            if (a[i] ^ a[i-1]) ++p;
        }
        int curr = 0, top = 0;
        for (int i = 1; i <= n; i++) {
            if (a[i] > curr) ++curr;
            else if (a[i] < curr) --curr;
            if (curr > mx[top][0]) {
                mx[++top][0] = curr;
                mx[top][1] = i;
            }
        }
        if (top == n) {
            printf("%d\n", n-1);
            continue;
        }
        int ans = mx[top][0];
        for (int i = n; top && i > 1; i--) {
            add(a[i]);
            while (mx[top][1] >= i) --top;
            ans = max(ans, query(n) - query(mx[top][1]-i) - query(mx[top][1]-i-1) + mx[top][0]);
        }
        printf("%d\n", ans);
    }
    return 0;
}