#include <bits/stdc++.h>

using namespace std;

#define uint unsigned int

uint tag[805];
int ans[805][805];
pair<int, int> q[805];

struct Tree {
    vector<int> e[805];

    inline void add(int a, int b) { e[a].push_back(b); e[b].push_back(a); }

    inline void dfs(const int &u, const uint &val, const int &fa) {
        int t = -1, h = 0;
        q[++t] = {u, fa};
        while (h <= t) {
            auto now = q[h++];
            int u = now.first;
            tag[u] += val;
            for (int v : e[u]) if (v != now.second) {
                q[++t] = {v, u};
            }
        }
    }
}t[305];

mt19937 ci(1145141919);

int main() {
    int n, k;
    scanf("%d%d", &n, &k);
    for (int i = 1; i <= k; i++) {
        for (int j = 1; j < n; j++) {
            int a, b;
            scanf("%d%d", &a, &b);
            // a = 1, b = j + 1;
            t[i].add(a, b);
        }
    }

    for (int i = 1; i <= n; i++) {
        memset(tag, 0, sizeof(tag));
        for (int j = 1; j <= k; j++) {
            for (int &rt : t[j].e[i]) {
                t[j].dfs(rt, ci(), i);
            }
        }
        for (int j = 1; j < n; j++) {
            for (int k = j + 1; k <= n; k++) {
                if (tag[j] != tag[k]) {
                    ans[j][k]++;
                }
            }
        }
        ans[i][i]++;
    }
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (i > j) printf("%d ", ans[j][i]);
            else printf("%d ", ans[i][j]);
        }
        printf("\n");
    }
    return 0;
}