#pragma once
#include <bits/stdc++.h>
using namespace std;
using int64 = long long;

enum WrongFlag {
    OMIT_X = 1,
    INCLUDE_CHILDREN = 2,
    ROOT_DISTANCE_ABS = 4,
    ROUTE_THROUGH_X = 8
};

inline int runNaiveWrong(int bug) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;
    vector<int> parent(n + 1), depth(n + 1);
    vector<int64> rootDistance(n + 1), value(n + 1);
    for (int i = 2; i <= n; ++i) {
        int w;
        cin >> parent[i] >> w;
        depth[i] = depth[parent[i]] + 1;
        rootDistance[i] = rootDistance[parent[i]] + w;
    }

    auto isDescendant = [&](int x, int u) {
        while (u && u != x) u = parent[u];
        return u == x;
    };
    auto correctDistance = [&](int u, int v) {
        int a = u, b = v;
        while (depth[a] > depth[b]) a = parent[a];
        while (depth[b] > depth[a]) b = parent[b];
        while (a != b) a = parent[a], b = parent[b];
        return rootDistance[u] + rootDistance[v] - 2 * rootDistance[a];
    };
    auto selected = [&](int x, int u) {
        if (!isDescendant(x, u)) return false;
        if ((bug & OMIT_X) && x == u) return false;
        if (!(bug & INCLUDE_CHILDREN) && parent[u] == x) return false;
        return true;
    };

    while (m--) {
        int op, x;
        cin >> op >> x;
        if (op == 1) {
            int y;
            cin >> y;
            for (int u = 1; u <= n; ++u) if (selected(x, u)) {
                int64 d;
                if (bug & ROOT_DISTANCE_ABS)
                    d = llabs(rootDistance[u] - rootDistance[y]);
                else if (bug & ROUTE_THROUGH_X)
                    d = correctDistance(x, y) + correctDistance(x, u);
                else
                    d = correctDistance(u, y);
                value[u] += d;
            }
        } else {
            int64 answer = 0;
            for (int u = 1; u <= n; ++u)
                if (selected(x, u)) answer += value[u];
            cout << answer << '\n';
        }
    }
    return 0;
}
