#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
template <class T> void in(T &x) {
    char c = getchar();
    T f = 1;
    x = 0;
    while (c < '0' || c > '9') {
        if (c == '-') f = -1;
        c = getchar();
    }
    while (c >= '0' && c <= '9') {
        x = x * 10 + c - '0';
        c = getchar();
    }
    x *= f;
}
const int N = 100010;
vector<int> g[N], h[N], ii[N];
int dfn[N], lst[N], pre[N], nxt[N], ck;
int n, col[N], u, v;
ll ans[N];
void calc(int u, int fa) {
    dfn[u] = lst[u] = ++ck;
    int tmp = pre[col[u]];
    if (tmp)
        h[nxt[tmp]].push_back(u); //,printf("%d %d %d\n",tmp,nxt[tmp],u);
    else
        ii[col[u]].push_back(u);
    pre[col[u]] = u;
    for (int v : g[u]) {
        if (v == fa) continue;
        nxt[u] = v;
        calc(v, u);
        lst[u] = max(lst[u], lst[v]);
        int sz = lst[v] - dfn[v] + 1;
        for (int w : h[v]) sz -= lst[w] - dfn[w] + 1;
        sz = n - sz;
        ans[dfn[v]] += sz;
        ans[lst[v] + 1] -= sz;
        for (int w : h[v]) ans[dfn[w]] -= sz, ans[lst[w] + 1] += sz;
    }
    ans[dfn[u]] += n;
    ans[dfn[u] + 1] -= n;
    pre[col[u]] = tmp;
    if (!fa) {
        for (int i = 1; i <= 100000; i++) {
            int sz = n;
            for (int v : ii[i]) sz -= lst[v] - dfn[v] + 1;
            sz = n - sz;
            ans[1] += sz;
            ans[n + 1] -= sz;
            for (int v : ii[i]) {
                ans[dfn[v]] -= sz;
                ans[lst[v] + 1] += sz;
            }
        }
    }
}
int main() {
    in(n);
    for (int i = 1; i <= n; i++) in(col[i]);
    for (int i = 1; i < n; i++) {
        in(u);
        in(v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    calc(1, 0);
    for (int i = 1; i <= n; i++) ans[i] += ans[i - 1];
    for (int i = 1; i <= n; i++) printf("%lld\n", ans[dfn[i]]);
    return 0;
}