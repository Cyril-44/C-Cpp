#include <bits/stdc++.h>
using namespace std;
using int64 = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<int> parent(n + 1), depth(n + 1);
    vector<int64> rootDist(n + 1), value(n + 1);
    for (int i = 2; i <= n; ++i) {
        int w;
        cin >> parent[i] >> w;
        depth[i] = depth[parent[i]] + 1;
        rootDist[i] = rootDist[parent[i]] + w;
    }

    auto isDescendant = [&](int x, int u) {
        while (u && u != x) u = parent[u];
        return u == x;
    };
    auto distance = [&](int u, int v) {
        int a = u, b = v;
        while (depth[a] > depth[b]) a = parent[a];
        while (depth[b] > depth[a]) b = parent[b];
        while (a != b) a = parent[a], b = parent[b];
        return rootDist[u] + rootDist[v] - 2 * rootDist[a];
    };

    while (m--) {
        int op, x;
        cin >> op >> x;
        if (op == 1) {
            int y;
            cin >> y;
            for (int i = 1; i <= n; ++i)
                if (parent[i] != x && isDescendant(x, i))
                    value[i] += distance(i, y);
        } else {
            int64 answer = 0;
            for (int i = 1; i <= n; ++i)
                if (parent[i] != x && isDescendant(x, i))
                    answer += value[i];
            cout << answer << '\n';
        }
    }
}
