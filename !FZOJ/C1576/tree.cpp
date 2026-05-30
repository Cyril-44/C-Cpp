#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int u, v;
};

int n, m;
vector<Edge> edges;          
vector<int> color;           
int Ecur;                    
int Ctarget;                 

vector<vector<pair<int,int>>> adj; 
bool okColoring;
bool check_coloring() {
    adj.assign(n + 1, {});
    for (int i = 0; i < Ecur; ++i) {
        int u = edges[i].u;
        int v = edges[i].v;
        adj[u].push_back({v, i});
        adj[v].push_back({u, i});
    }
    vector<int> usedEdge(Ecur, 0);
    vector<int> usedColor(Ctarget + 1, 0);
    int distinctColors = 0;

    function<void(int)> dfs = [&](int u) {
        for (auto [v, id] : adj[u]) {
            if (usedEdge[id]) continue;
            usedEdge[id] = 1;

            int c = color[id];
            bool addNew = false;
            if (!usedColor[c]) {
                usedColor[c] = 1;
                distinctColors++;
                addNew = true;
            }

            if (distinctColors > 2) {
                if (addNew) {
                    usedColor[c] = 0;
                    distinctColors--;
                }
                usedEdge[id] = 0;
                throw false;
            }

            dfs(v);

            // 回溯
            if (addNew) {
                usedColor[c] = 0;
                distinctColors--;
            }
            usedEdge[id] = 0;
        }
    };

    try {
        for (int s = 1; s <= n; ++s) {
            dfs(s);
        }
    } catch (bool) {
        return false;
    }
    return true;
}

void dfs_color(int idx) {
    if (okColoring) return;
    if (idx == Ecur) {
        if (check_coloring()) okColoring = true;
        return;
    }
    for (int c = 1; c <= Ctarget; ++c) {
        color[idx] = c;
        dfs_color(idx + 1);
        if (okColoring) return;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m;
    edges.reserve(m);
    color.resize(m);

    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        edges.push_back({u, v});
        Ecur = i + 1;

        int ans = 1;
        for (int C = Ecur; C >= 1; --C) {
            Ctarget = C;
            okColoring = false;
            dfs_color(0);
            if (okColoring) {
                ans = C;
                break;
            }
        }
        cout << ans << "\n";
    }
    return 0;
}
