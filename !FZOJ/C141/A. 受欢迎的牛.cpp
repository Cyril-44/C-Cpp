#include <bits/stdc++.h>
using namespace std;
int n, m, dfn[10005], low[10005], idx, st[10005], top, ans;
int nn, pos[10005], cnt[10005], chu[10005];
bool ist[10005];
vector<int> g[10005];
void dfs(int x) {
    dfn[x] = low[x] = ++idx;
    st[++top] = x;
    ist[x] = 1;
    for (int y : g[x]) {
        if (!dfn[y]) {
            dfs(y);
            low[x] = min(low[x], low[y]);
        } else if (ist[y]) {
            low[x] = min(low[x], low[y]);
        }
    }
    if (dfn[x] == low[x]) {
        nn++;
        int y;
        do {
            y = st[top];
            pos[y] = nn;
            ist[y] = 0;
            cnt[nn]++;
            top--;
        } while (dfn[y] != low[y]);
    }
}
int main() {
    cin >> n >> m;
    for (int i = 1; i <= m; i++) {
        int x, y;
        scanf("%d%d", &x, &y);
        g[x].push_back(y);
    }
    for (int i = 1; i <= n; i++) {
        if (!dfn[i]) {
            dfs(i);
        }
    }
    for (int x = 1; x <= n; x++) {
        for (int y : g[x]) {
            if (pos[x] != pos[y]) {
                chu[pos[x]]++;
            }
        }
    }
    for (int x = 1; x <= nn; x++) {
        if (chu[x] == 0) {
            if (ans) {
                cout << 0;
                return 0;
            }
            ans = cnt[x];
        }
    }
    cout << ans;
    return 0;
}
/*
3 3
1 2
2 1
3 2
*/
