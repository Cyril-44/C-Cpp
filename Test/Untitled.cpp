#include <bits/stdc++.h>
using namespace std;
int main() {
    int T;
    cin >> T;
    while (T--) {
        int n, m;
        cin >> n >> m;
        vector<vector<pair<int, int>>> adj(n + 1);
        vector<int> edges;
        for (int i = 0; i < m; ++i) {
            int u, v, w;
            cin >> u >> v >> w;
            adj[u].emplace_back(v, w);
            edges.push_back(w);
        }

        // Find shortest and longest path from 1 to n
        auto get_dist = [&](int delta) {
            vector<long long> dist(n + 1, LLONG_MAX);
            dist[1] = 0;
            priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
            pq.emplace(0, 1);
            while (!pq.empty()) {
                auto [d, u] = pq.top(); pq.pop();
                if (d > dist[u]) continue;
                for (auto [v, w] : adj[u]) {
                    long long nw = w + delta;
                    if (nw < 0) continue;
                    if (dist[v] > dist[u] + nw) {
                        dist[v] = dist[u] + nw;
                        pq.emplace(dist[v], v);
                    }
                }
            }
            return dist[n];
        };

        // Binary search for minimal delta so all edges >= 0
        int left = -100000, right = 100000, ans = -1;
        while (left <= right) {
            int mid = (left + right) / 2;
            long long d = get_dist(mid);
            if (d != LLONG_MAX) {
                ans = d;
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }
        cout << (ans == -1 ? 1 : ans) << endl;
    }
}