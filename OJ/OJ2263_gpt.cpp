#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <limits>

using namespace std;

const int INF = numeric_limits<int>::max();

vector<int> dijkstra(int start, const vector<vector<pair<int, int>>>& graph, int n) {
    vector<int> distances(n + 1, INF);
    distances[start] = 0;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({0, start});

    while (!pq.empty()) {
        auto [current_distance, current_node] = pq.top();
        pq.pop();

        if (current_distance > distances[current_node]) continue;

        for (const auto& [neighbor, weight] : graph[current_node]) {
            int distance = current_distance + weight;

            if (distance < distances[neighbor]) {
                distances[neighbor] = distance;
                pq.push({distance, neighbor});
            }
        }
    }

    return distances;
}

int max_common_stations(int n, const vector<tuple<int, int, int>>& edges, int u1, int v1, int u2, int v2) {
    vector<vector<pair<int, int>>> graph(n + 1);
    for (const auto& [u, v, l] : edges) {
        graph[u].emplace_back(v, l);
        graph[v].emplace_back(u, l);
    }

    auto dist_from_u1 = dijkstra(u1, graph, n);
    auto dist_from_v1 = dijkstra(v1, graph, n);
    auto dist_from_u2 = dijkstra(u2, graph, n);
    auto dist_from_v2 = dijkstra(v2, graph, n);
    
    int common_stations = 0;

    for (int i = 1; i <= n; ++i) {
        if (dist_from_u1[i] + dist_from_v1[i] == dist_from_u1[v1] &&
            dist_from_u2[i] + dist_from_v2[i] == dist_from_u2[v2]) {
            common_stations++;
        }
    }

    return common_stations;
}

int main() {
    int n, m;
    cin >> n >> m;

    vector<tuple<int, int, int>> edges(m);
    for (int i = 0; i < m; ++i) {
        int u, v, l;
        cin >> u >> v >> l;
        edges[i] = {u, v, l};
    }

    int u1, v1, u2, v2;
    cin >> u1 >> v1 >> u2 >> v2;

    cout << max_common_stations(n, edges, u1, v1, u2, v2) << endl;

    return 0;
}
