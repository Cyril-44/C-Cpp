#include <bits/stdc++.h>
using namespace std;

vector<vector<pair<int, int>>> g;
vector<int> fr, offset, cur;
vector<char> vis;
vector<int> vs, es, ops;

void dfs(int u, int incoming_edge) {
    while (cur[u] < (int)g[u].size()) {
        auto [to, id] = g[u][cur[u]++];
        if (vis[id]) continue;
        vis[id] = true;
        dfs(to, id);
    }
    vs.push_back(u);
    es.push_back(incoming_edge);
}

int main() {
    freopen("machine.in", "r", stdin);
    freopen("machine.out", "w", stdout);

    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) {
        int n;
        cin >> n;

        fr.assign(n + 1, 0);
        vector<int> b(n + 1);
        offset.assign(n + 1, 0);
        g.assign(n + 1, {});
        vis.assign(n + 1, false);
        cur.assign(n + 1, 0);

        for (int i = 1; i <= n; ++i) cin >> fr[i];
        for (int i = 1; i <= n; ++i) cin >> b[i];

        for (int i = 1; i <= n; ++i) {
            if (fr[i] == b[i]) continue;
            g[fr[i]].push_back({b[i], i});
            g[b[i]].push_back({fr[i], i});
            ++offset[fr[i]];
            ++offset[b[i]];
        }

        bool possible = true;
        for (int value = 1; value <= n; ++value) {
            if (offset[value] & 1) {
                possible = false;
                break;
            }
        }
        if (!possible) {
            cout << -1 << '\n';
            continue;
        }

        ops.clear();
        for (int i = 1; i <= n; ++i) {
            if (g[i].empty()) continue;

            vs.clear();
            es.clear();
            dfs(i, 0);

            reverse(vs.begin(), vs.end());
            reverse(es.begin(), es.end());
            for (size_t i = 1; i < es.size(); ++i) {
                int id = es[i];
                int source = vs[i - 1];
                if (fr[id] != source) ops.push_back(id);
            }
        }

        cout << ops.size() << '\n';
        for (size_t i = 0; i < ops.size(); ++i) {
            if (i) cout << ' ';
            cout << ops[i];
        }
        cout << '\n';
    }
    return 0;
}
