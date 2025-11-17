#include <bits/stdc++.h>
using namespace std;
int n, dfn[105], low[105], idx, st[105], top, ans;
int nn, pos[105], cnt[105], ru[105], chu[105];
bool ist[105];
vector<int> g[105];
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
        int x;
        scanf("%d", &x);
        while (x) {
            g[i].push_back(x);
            scanf("%d", &x);
        }
    }
    for (int i = 1; i <= n; i++) {
        if (!dfn[i]) {
            dfs(i);
        }
    }
    if (nn == 1) {
        printf("1\n0");
        return 0;
    }
    for (int x = 1; x <= n; x++) {
        for (int y : g[x]) {
            if (pos[x] != pos[y]) {
                chu[pos[x]]++;
                ru[pos[y]]++;
            }
        }
    }
    int cnt1 = 0, cnt2 = 0;
    for (int x = 1; x <= nn; x++) {
        if (!ru[x])
            cnt1++;
        if (!chu[x])
            cnt2++;
    }
    cout << cnt1 << endl << max(cnt1, cnt2);
    return 0;
}
/*
6
1 3
2 3
1 4
2 4

4
3 4 0
3 4 0
0
0
*/
