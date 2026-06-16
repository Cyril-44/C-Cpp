#include <cstdio>
#include <algorithm>
constexpr int N = 16;
using Status = unsigned long long;
Status s[N], t[N];
int n, m;
inline Status read() {
    char ch = getchar();
    while (ch != '0' && ch != '1') ch = getchar();
    Status res = 0;
    for (int i = 0; i < m; i++, ch = getchar())
        res |= Status(ch & 1) << i;
    return res;
}
bool dfs(int u, Status st) {
    if (u >= n) return __builtin_popcountll(st) > m;
    Status all[4]{st, st ^ t[u], st ^ s[u] ^ t[u]};
    if (__builtin_popcountll(st^s[u]) > __builtin_popcountll(st^s[u]^t[u])) {
        if (dfs(u+1, st ^ s[u]) && dfs(u+1, st ^ s[u] ^ t[u])) return true;
        if (dfs(u+1, st) && dfs(u+1, st ^ t[u])) return true;
    } else {
        if (dfs(u+1, st) && dfs(u+1, st ^ t[u])) return true;
        if (dfs(u+1, st ^ s[u]) && dfs(u+1, st ^ s[u] ^ t[u])) return true;
    }
    return false;
}
bool dfs2(int u, bool f, Status st) {
    if (u >= n) return __builtin_popcountll(st) > m;
    if (!f) return dfs2(u, true, st) || dfs2(u, true, st ^ s[u]);
    else return dfs2(u+1, false, st) && dfs2(u+1, false, st ^ t[u]);
}

int main() {
#ifndef DEBUG
    freopen("stone.in", "r", stdin);
    freopen("stone.out", "w", stdout);
#endif
    int T;
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d", &n, &m);
        Status initst = read();
        for (int i = 0; i < n; i++)
            s[i] = read(), t[i] = read();
        m /= 2;
        puts(dfs(0, initst) ? "light" : "dark");
    }
    return 0;
}