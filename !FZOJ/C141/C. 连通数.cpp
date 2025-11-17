#include <bits/stdc++.h>
using namespace std;
int n, dfn[2005], low[2005], idx, st[2005], top, ans;
int nn, pos[2005], cnt[2005], ru[2005];
bool ist[2005];
vector<int> g[2005], g2[2005];
char s[2005];
bitset<2005> f[2005];
queue<int> q;
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
    cin >> n;
    for (int i = 1; i <= n; i++) {
        scanf("%s", s + 1);
        for (int j = 1; j <= n; j++) {
            if (s[j] == '1') {
                g[i].push_back(j);
            }
        }
    }
    for (int i = 1; i <= n; i++) {
        if (!dfn[i]) {
            dfs(i);
        }
    }
    for (int x = 1; x <= n; x++) {
        for (int y : g[x]) {
            if (pos[x] != pos[y]) {
                g2[pos[y]].push_back(pos[x]);
                ru[pos[x]]++;
            }
        }
    }
    for (int i = 1; i <= nn; i++) {
        f[i][i] = 1;
        if (!ru[i]) {
            q.push(i);
        }
    }
    while (!q.empty()) {
        int x = q.front();
        q.pop();
        for (int y : g2[x]) {
            ru[y]--;
            f[y] |= f[x];
            if (!ru[y]) {
                q.push(y);
            }
        }
    }
    for (int i = 1; i <= nn; i++) {
        for (int j = 1; j <= nn; j++) {
            ans += f[i][j] * cnt[i] * cnt[j];
        }
    }
    cout << ans;
    return 0;
}
/*
6
010000
001000
100100
000010
001001
000010
*/
