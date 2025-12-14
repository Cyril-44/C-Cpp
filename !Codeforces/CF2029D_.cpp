#include <iostream>
#include <vector>
#include <set>
using namespace std;

void solve() {
    int t;
    cin >> t;
    while (t--) {
        int n, m;
        cin >> n >> m;
        vector<pair<int, int>> edges(m);
        set<pair<int, int>> edgeSet;
        for (int i = 0; i < m; ++i) {
            int u, v;
            cin >> u >> v;
            if (u > v) swap(u, v);
            edges[i] = {u, v};
            edgeSet.insert({u, v});
        }

        vector<tuple<int, int, int>> operations;
        for (int i = 0; i < m; ++i) {
            int u = edges[i].first;
            int v = edges[i].second;
            for (int w = 1; w <= n; ++w) {
                if (w != u && w != v) {
                    if (edgeSet.count({u, w}) == 0) {
                        operations.push_back({u, v, w});
                        edgeSet.insert({u, w});
                    } else {
                        operations.push_back({u, v, w});
                        edgeSet.erase({u, w});
                    }
                    if (edgeSet.count({v, w}) == 0) {
                        operations.push_back({u, v, w});
                        edgeSet.insert({v, w});
                    } else {
                        operations.push_back({u, v, w});
                        edgeSet.erase({v, w});
                    }
                    break;
                }
            }
        }

        cout << operations.size() << endl;
        for (auto &op : operations) {
            int a, b, c;
            tie(a, b, c) = op;
            cout << a << " " << b << " " << c << endl;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    solve();
    return 0;
}
