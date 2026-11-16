#include <bits/stdc++.h>
using namespace std;
using int64 = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<int> parent(n + 1);
    vector<vector<pair<int, int>>> graph(n + 1);
    for (int i = 2; i <= n; ++i) {
        int w;
        cin >> parent[i] >> w;
        graph[parent[i]].push_back({i, w});
        graph[i].push_back({parent[i], w});
    }

    // Precompute every tree distance in O(n^2).
    vector<vector<int64>> distance(n + 1, vector<int64>(n + 1));
    for (int source = 1; source <= n; ++source) {
        vector<tuple<int, int, int64>> stack{{source, 0, 0}};
        while (!stack.empty()) {
            auto [u, p, d] = stack.back();
            stack.pop_back();
            distance[source][u] = d;
            for (auto [v, w] : graph[u])
                if (v != p) stack.push_back({v, u, d + w});
        }
    }

    // Mark every ancestor/descendant pair in O(n^2) total, even on a chain.
    vector<vector<unsigned char>> belongs(n + 1, vector<unsigned char>(n + 1));
    for (int u = 1; u <= n; ++u)
        for (int x = u; x; x = parent[x])
            if (parent[u] != x) belongs[x][u] = 1;

    vector<int64> value(n + 1);
    while (m--) {
        int op, x;
        cin >> op >> x;
        if (op == 1) {
            int y;
            cin >> y;
            for (int u = 1; u <= n; ++u)
                if (belongs[x][u]) value[u] += distance[u][y];
        } else {
            int64 answer = 0;
            for (int u = 1; u <= n; ++u)
                if (belongs[x][u]) answer += value[u];
            cout << answer << '\n';
        }
    }
}
