#include <stdio.h>
#include <string.h>
#include <assert.h>
constexpr int N = 200005;
int a[N], ts=0, n;
int nxt[N], h[N];
#ifndef ONLINE_JUDGE
# define push(x) (a[++tot] = (x), fprintf(stderr, "Pushed %d\n", x))
#else
# define push(x) (a[++tot] = (x))
#endif
void dfs(int l, int r) {
    fprintf(stderr, "%d %d %d\n", l, nxt[l], r);
    if (l > r || l >= n) return;
    if (l == r || nxt[l] > r || !nxt[l]) return;
    push(nxt[l]);
    if (l+1 != nxt[l]) {
        push(l+1);
        dfs(l+1, nxt[l] - 1);
    }
    dfs(nxt[l], r);
}
int main() {
#ifndef ONLINE_JUDGE
    freopen("paper.in", "r", stdin);
    freopen("paper.out", "w", stdout);
    freopen("paper.err", "w", stderr);
#endif
    scanf("%d", &n);
    for (int i = 1; i < n; i++)
        scanf("%d", &a[i]);
    for (int i = n; i >= 1; i--) {
        nxt[i] = h[a[i]];
        h[a[i]] = i;
    }
    memset(a, 0, sizeof a);
    push(1);
    dfs(1, n-1);
    a[n] = n;
    for (int i = 1; i <= n; i++)
        printf("%d ", a[i]);
    return 0;
}
