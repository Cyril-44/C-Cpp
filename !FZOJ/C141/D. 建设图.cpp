#include <bits/stdc++.h>
using namespace std;
int n, m, dfn[100005], low[100005], idx, st[100005], top, ans;
int nn, pos[100005], cnt[100005], t[100005];
bool ist[100005];
vector<int> g[100005], g2[100005];
char s[100005];
void dfs(int x, int fa) {
    dfn[x] = low[x] = ++idx;
    st[++top] = x;
    ist[x] = 1;
    for (int y : g[x]) {
        if (!dfn[y]) {
            dfs(y, x);
            low[x] = min(low[x], low[y]);
        } else if (ist[y] && y != fa) {
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
        g[y].push_back(x);
    }
    dfs(1, 0);
    if (nn == 1) {
        cout << 0;
        return 0;
    }
    for (int x = 1; x <= n; x++) {
        for (int y : g[x]) {
            if (pos[x] != pos[y]) {
                g2[pos[x]].push_back(pos[y]);
                g2[pos[y]].push_back(pos[x]);
            }
        }
    }
    for (int x = 1; x <= nn; x++) {
        int cntt = 0;
        for (int y : g2[x]) {
            if (t[y] != x) {
                cntt++;
                t[y] = x;
            }
        }
        if (cntt == 1)
            ans++;
    }
    cout << ans - 1;
    return 0;
}
/*
6 8
1 2
2 3
3 1
3 4
4 5
5 3
6 5
4 6
010000
001000
100100
000010
001001
000010
*/
